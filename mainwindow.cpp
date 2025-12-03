#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMessageBox>
#include <QDate>
#include <QHeaderView>
#include <QAbstractItemModel>
#include <QFileDialog>
#include <QPdfWriter>
#include <QPainter>
#include <QPageSize>
#include <QSqlQuery>
#include <QSqlError>
#include <QVBoxLayout>
#include <QRegularExpression>  // ✅ REQUIRED
#include <QTableWidgetItem>    // ✅ REQUIRED
#include <QDebug>              // ✅ REQUIRED

#include "exam.h"
#include "vehicule.h"          // ✅ REQUIRED



static bool isExactly8Digits(const QString &s) {
    const QString t = s.trimmed();
    if (t.size() != 8) return false;
    for (QChar c : t) if (!c.isDigit()) return false;
    return true;
}


static bool isLettersSpaces(const QString &s) {
    if (s.trimmed().isEmpty()) return false;
    for (QChar c : s) if (!(c.isLetter() || c.isSpace() || c == '-' || c == '\'')) return false;
    return true;
}

static QString normType(QString s) {
    s = s.trimmed().toLower();
    if (s == "code")      return "Code";
    if (s == "conduite")  return "Conduite";
    return {};
}

static QString normRes(QString s) {
    s = s.trimmed().toLower();
    if (s == "reussite" || s == "réussite") return QString::fromUtf8("Réussite");
    if (s == "echec"    || s == "échec")    return "Echec";
    return {};
}
static bool canAddConduiteSuccess(const QString& cin, const QDate& dateConduite)
{
    // Dernière réussite "Code" pour ce CIN
    QSqlQuery q;
    q.prepare(
        "SELECT MAX(DATE_EXAM) "
        "FROM EXAMENS "
        "WHERE CINC=:c "
        "  AND TYPE='Code' "
        "  AND UPPER(RESULTAT) IN ('REUSSITE','RÉUSSITE')");
    q.bindValue(":c", cin);

    if (!q.exec() || !q.next())
        return false; // prudence: si on n'arrive pas à lire, on refuse

    const QDate codeOk = q.value(0).toDate();
    if (!codeOk.isValid())
        return false; // pas de réussite "Code" trouvée

    return codeOk.addMonths(1) <= dateConduite;
}



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QCalendarWidget cal;
    auto cal = ui->calendar;
    Q_ASSERT(call);

    // Safe lambda with null check
    const auto go = [this](QWidget* page){
        if (page && ui->stack) {
            ui->stack->setCurrentWidget(page);
        }
    };

    // Only connect if buttons AND pages exist
    if (ui->btnExamens && ui->pageExamens)
        connect(ui->btnExamens, &QPushButton::clicked, this, [=]{ go(ui->pageExamens); });

    if (ui->btnCandidat && ui->pageCandidats)
        connect(ui->btnCandidat, &QPushButton::clicked, this, [=]{ go(ui->pageCandidats); });

    if (ui->btnMoniteur && ui->pageMoniteur)
        connect(ui->btnMoniteur, &QPushButton::clicked, this, [=]{ go(ui->pageMoniteur); });

    if (ui->btnVehicule && ui->pageVehicules)
        connect(ui->btnVehicule, &QPushButton::clicked, this, [=]{
            if (ui->stack && ui->pageVehicules) {
                ui->stack->setCurrentWidget(ui->pageVehicules);
                loadVehicleData(); // Refresh vehicle data when opening
            }
        });

    if (ui->btnFinance && ui->pageFinance)
        connect(ui->btnFinance, &QPushButton::clicked, this, [=]{ go(ui->pageFinance); });

    // Calendar setup with safety checks
    if (ui->calendar) {
        connect(ui->calendar, &QCalendarWidget::currentPageChanged,
                this, [this](int, int){ refreshCalendarMarks(); });
        ui->calendar->setGridVisible(true);
    }

    // ComboBox setup with safety checks
    if (ui->comboTypePlan) {
        ui->comboTypePlan->clear();
        ui->comboTypePlan->addItems({ "Code", "Conduite" });
    }

    if (ui->datePlan)
        ui->datePlan->setDate(QDate::currentDate());

    if (ui->timePlan)
        ui->timePlan->setTime(QTime::currentTime());

    if (ui->listDay) {
        ui->listDay->setAlternatingRowColors(true);
        ui->listDay->setSelectionMode(QAbstractItemView::SingleSelection);
        ui->listDay->setWordWrap(true);
    }

    if (ui->calendar) {
        refreshCalendarMarks();
        loadDayList(ui->calendar->selectedDate());
    }

    // Set default page
    if (ui->stack && ui->pageExamens)
        ui->stack->setCurrentWidget(ui->pageExamens);

    // Field mask setup
    auto setMaskForField = [this](const QString &field){
        if (!ui->lineEdit_7) return;

        ui->lineEdit_7->setInputMask("");
        ui->lineEdit_7->setMaxLength(64);
        ui->lineEdit_7->clear();
        if (ui->tableWidget) {
            ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
            ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
            ui->tableWidget->setAlternatingRowColors(true);
        }
        loadVehicleData();

        if (field == "CIN") {
            ui->lineEdit_7->setInputMask("00000000;_");
            ui->lineEdit_7->setMaxLength(8);
            ui->lineEdit_7->setPlaceholderText("8 chiffres");
        } else if (field == "Date") {
            ui->lineEdit_7->setInputMask("99/99/9999;_");
            ui->lineEdit_7->setPlaceholderText("jj/mm/aaaa");
        } else if (field == "Type") {
            ui->lineEdit_7->setPlaceholderText("Code ou Conduite");
        } else if (field == "Résultat") {
            ui->lineEdit_7->setPlaceholderText("Réussite ou Echec");
        } else if (field == "Lieu") {
            ui->lineEdit_7->setPlaceholderText("Lettres/espaces (' -)");
        }
    };

    if (ui->comboBox_4) {
        setMaskForField(ui->comboBox_4->currentText());
        connect(ui->comboBox_4, &QComboBox::currentTextChanged, this, setMaskForField);
    }

    // Table view setup
    if (ui->tableViewExams) {
        ui->tableViewExams->setSelectionBehavior(QAbstractItemView::SelectRows);
        ui->tableViewExams->setSelectionMode(QAbstractItemView::SingleSelection);
        ui->tableViewExams->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        ui->tableViewExams->setAlternatingRowColors(false);
        ui->tableViewExams->setMouseTracking(true);
        ui->tableViewExams->viewport()->setMouseTracking(true);
        ui->tableViewExams->viewport()->setAttribute(Qt::WA_Hover, true);

        ui->tableViewExams->setStyleSheet(
            "QTableView::item:hover { background: rgba(0,0,0,0.06); }"
            "QTableView::item:selected { background: #CDE5FF; color: black; }"
            );
    }

    loadTableData();

    // Stats chart setup with safety check
    if (ui->chartContainer) {
        statsChartView = new StatsChartWidget(this);
        if (auto lay = qobject_cast<QVBoxLayout*>(ui->chartContainer->layout())) {
            lay->addWidget(statsChartView);
        } else {
            auto *v = new QVBoxLayout(ui->chartContainer);
            v->setContentsMargins(0,0,0,0);
            v->addWidget(statsChartView);
        }
    }

    // Combo stats connection
    if (ui->comboTypeStats) {
        connect(ui->comboTypeStats, SIGNAL(currentIndexChanged(int)),
                this, SLOT(on_comboTypeStats_currentIndexChanged(int)));
    }

    if (ui->tableViewExams && ui->tableViewExams->horizontalHeader())
        ui->tableViewExams->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}


// --- vérifie l'accès une fois (par ex. dans le ctor ou à l'ouverture de l'onglet)
void MainWindow::checkPlanningAccess()
{
    QSqlQuery q;
    q.prepare("SELECT COUNT(*) FROM EXAM_PLANNING");
    if (!q.exec() || !q.next()) {
        QMessageBox::critical(this, "DB",
                              "Pas d’accès à EXAM_PLANNING:\n" + q.lastError().text());
    }
}


MainWindow::~MainWindow()
{
    detachModel();
    delete ui;
}


void MainWindow::detachModel()
{
    if (modelExams) {
        ui->tableViewExams->setModel(nullptr);
        delete modelExams;
        modelExams = nullptr;
    }
}
// MainWindow::loadTableData()
void MainWindow::loadTableData() {
    detachModel();
    modelExams = Exam::afficherTous();
    ui->tableViewExams->setModel(modelExams);
    ui->tableViewExams->setSortingEnabled(false); // on pilote le tri via le bouton
}





void MainWindow::clearForm()
{
    ui->lineEdit_3->clear(); // Lieu
    ui->lineEdit_5->clear(); // CIN
    ui->dateEdit->setDate(QDate::currentDate());
}

void MainWindow::on_addButton_clicked()
{
    if (role_ != Role::Admin && role_ != Role::MobiliteExamens) {
        QMessageBox::warning(this, "Droits", "Action réservée à l'admin.");
        return;
    }

    const QString typeUi     = ui->comboBox_3->currentText().trimmed();
    const QString type       = normType(typeUi);
    const QString lieu       = ui->lineEdit_3->text().trimmed();
    const QString dateStr    = ui->dateEdit->date().toString("dd/MM/yyyy");
    const QString resultatUi = ui->comboBox_6->currentText().trimmed();
    const QString resultat   = normRes(resultatUi);
    const QString cin        = ui->lineEdit_5->text().trimmed();

    if (type.isEmpty())      { QMessageBox::warning(this,"Erreur","Type = Code ou Conduite."); return; }
    if (lieu.isEmpty() || !isLettersSpaces(lieu)) {
        QMessageBox::warning(this,"Erreur","Lieu: lettres/espaces (- ') uniquement.");
        return;
    }
    if (resultat.isEmpty())  { QMessageBox::warning(this,"Erreur","Résultat = Réussite ou Echec."); return; }
    if (!isExactly8Digits(cin)) {
        QMessageBox::warning(this,"Erreur","CIN: exactement 8 chiffres.");
        return;
    }

    Exam e(type, dateStr, lieu, resultat, cin);
    detachModel();
    if (e.ajouter()) {
        loadTableData();
        clearForm();
        QMessageBox::information(this, "Succès", "Examen ajouté.");
        return;
    }

    // Message d’échec + “pistes” (les vraies règles sont dans Exam::ajouter()).
    QMessageBox::warning(this, "Échec d'ajout",
                         "L'ajout a été refusé par les règles métier :\n"
                         "• Pas de 2e « Réussite » pour le même (CIN, TYPE)\n"
                         "• Conduite « Réussite » seulement si Code déjà « Réussi » avant\n"
                         "• Délai d’au moins 1 mois entre tentatives.\n"
                         "Vérifie aussi que la date est valide et le CIN a 8 chiffres.");
    loadTableData();
}






void MainWindow::on_deleteButton_clicked()
{
    if (role_ != Role::Admin && role_ != Role::MobiliteExamens) {
        QMessageBox::warning(this, "Droits", "Action réservée à l'admin.");
        return;
    }
    const int row = ui->tableViewExams->currentIndex().row();
    if (row < 0) { QMessageBox::warning(this,"Erreur","Sélectionnez une ligne."); return; }

    QAbstractItemModel *m = ui->tableViewExams->model();
    const QString cin  = m->index(row, 4).data().toString();
    const QString date = m->index(row, 1).data().toString(); // "DD/MM/YYYY"

    detachModel();
    if (Exam::supprimer(cin, date)) {
        loadTableData();
        QMessageBox::information(this, "Succès", "Examen supprimé.");
    } else {
        loadTableData();
        QMessageBox::critical(this, "Erreur", "Échec de suppression.");
    }
}




void MainWindow::on_btnSearch_clicked()
{
    const QString s = ui->lineEdit_6->text().trimmed();
    detachModel();
    modelExams = s.isEmpty() ? Exam::afficherTous() : Exam::afficherParCin(s);
    ui->tableViewExams->setModel(modelExams);
    ui->tableViewExams->setSortingEnabled(false);
}





void MainWindow::on_tabWidget_currentChanged(int index)
{
    // Quand on entre sur l’onglet "Statistiques", reload
    if (ui->tabWidget->tabText(index).contains("Stat", Qt::CaseInsensitive)) {
        loadStatsTable();
    }
}

void MainWindow::on_comboTypeStats_currentIndexChanged(int)
{
    loadStatsTable();
}

void MainWindow::loadStatsTable()
{
    if (!statsChartView) return;

    const QString type = ui->comboTypeStats->currentText().trimmed(); // "Code" ou "Conduite"

    // vecteur 12 mois, initialisé à 0
    QVector<int>   total(12, 0);
    QVector<int>   succes(12, 0);
    QVector<double> taux(12, 0.0);

    // Requête par mois de l'année courante
    QSqlQuery q;
    q.prepare(R"(
     WITH B AS (
          SELECT EXTRACT(MONTH FROM DATE_EXAM) AS MOIS,
                 COUNT(*) AS TOTAL,
                 SUM(CASE WHEN UPPER(RESULTAT) IN ('REUSSITE','RÉUSSITE') THEN 1 ELSE 0 END) AS SUCCES
          FROM EXAMENS
          WHERE UPPER(TYPE) = UPPER(:type)
            AND EXTRACT(YEAR FROM DATE_EXAM) = EXTRACT(YEAR FROM SYSDATE)
          GROUP BY EXTRACT(MONTH FROM DATE_EXAM)
        ), M AS (SELECT LEVEL AS MOIS FROM dual CONNECT BY LEVEL<=12)
        SELECT M.MOIS,
               NVL(B.TOTAL,0) AS TOTAL,
               NVL(B.SUCCES,0) AS SUCCES,
               CASE WHEN NVL(B.TOTAL,0)=0 THEN 0
                    ELSE ROUND(100*B.SUCCES/B.TOTAL,2) END AS TAUX
        FROM M LEFT JOIN B ON B.MOIS = M.MOIS
        ORDER BY M.MOIS
    )");
    q.bindValue(":type", ui->comboTypeStats->currentText().trimmed());

    if (!q.exec()) {
        // si erreur SQL, on laisse la courbe vide
        statsChartView->setData(QVector<double>(12, 0.0),
                                tr("Erreur SQL: %1").arg(q.lastError().text()));
        return;
    }

    while (q.next()) {
        int m  = q.value(0).toInt();   // 1..12
        int t  = q.value(1).toInt();
        int s  = q.value(2).toInt();
        if (m >= 1 && m <= 12) {
            total[m-1]  = t;
            succes[m-1] = s;
        }
    }

    for (int i=0; i<12; ++i) {
        if (total[i] > 0)
            taux[i] = 100.0 * double(succes[i]) / double(total[i]);
        else
            taux[i] = 0.0;
    }

    const int year = QDate::currentDate().year();
    const QString title = tr("Taux de réussite %1 — %2").arg(type).arg(year);

    statsChartView->setData(taux, title);
}



void MainWindow::on_modifyButton_clicked()
{
    if (role_ != Role::Admin && role_ != Role::MobiliteExamens) {
        QMessageBox::warning(this, "Droits", "Action réservée à l'admin.");
        return;
    }

    auto *m   = qobject_cast<QSqlQueryModel*>(ui->tableViewExams->model());
    auto *sel = ui->tableViewExams->selectionModel();
    if (!m || !sel || !sel->hasSelection()) {
        QMessageBox::warning(this, "Erreur", "Sélectionnez une ligne.");
        return;
    }

    const int row      = sel->currentIndex().row();
    const QString cin0 = m->data(m->index(row, 4)).toString();
    const QDate   d0   = QDate::fromString(m->data(m->index(row, 1)).toString(), "dd/MM/yyyy");

    const QString champ = ui->comboBox_4->currentText();  // Type/Date/Lieu/Résultat/CIN
    const QString nv    = ui->lineEdit_7->text().trimmed();
    if (nv.isEmpty()) { QMessageBox::warning(this,"Erreur","Entrez la nouvelle valeur."); return; }

    bool ok = false;
    detachModel();

    if (champ == "Date") {
        const QDate nd = QDate::fromString(nv, "dd/MM/yyyy");
        if (!nd.isValid()) { loadTableData(); QMessageBox::warning(this,"Erreur","Date invalide (jj/mm/aaaa)."); return; }
        ok = Exam::modifierDate(cin0, d0, nd);

    } else if (champ == "Type") {
        const QString v = normType(nv);
        if (v.isEmpty()) { loadTableData(); QMessageBox::warning(this,"Erreur","Type = Code ou Conduite."); return; }
        ok = Exam::modifierTexte(cin0, d0, "TYPE", v);

    } else if (champ == "Lieu") {
        if (!isLettersSpaces(nv)) { loadTableData(); QMessageBox::warning(this,"Erreur","Lieu: lettres/espaces (- ') uniquement."); return; }
        ok = Exam::modifierTexte(cin0, d0, "LIEU", nv);

    } else if (champ == "Résultat") {
        const QString v = normRes(nv);
        if (v.isEmpty()) { loadTableData(); QMessageBox::warning(this,"Erreur","Résultat = Réussite ou Echec."); return; }
        ok = Exam::modifierTexte(cin0, d0, "RESULTAT", v);

    } else if (champ == "CIN") {
        if (!isExactly8Digits(nv)) { loadTableData(); QMessageBox::warning(this,"Erreur","CIN: exactement 8 chiffres."); return; }
        ok = Exam::modifierTexte(cin0, d0, "CINC", nv);
    }

    loadTableData();
    if (ok) QMessageBox::information(this, "Succès", "Modification enregistrée.");
    else    QMessageBox::critical(this, "Erreur", "Échec de modification.");
}




void MainWindow::on_triButton_clicked()
{
    const bool asc = (ui->comboTri->currentText() == "Croissant");
    detachModel();
    modelExams = Exam::afficherParDate(asc);  // ORDER BY EXAM_DATE ASC/DESC (déjà prêt)
    ui->tableViewExams->setModel(modelExams);
    ui->tableViewExams->setSortingEnabled(false);
}



void MainWindow::on_exportButton_clicked()
{
    if (role_ != Role::Admin && role_ != Role::MobiliteExamens) {
        QMessageBox::warning(this, "Droits", "Action réservée à l'admin.");
        return;
    }

    const QString filePath = QFileDialog::getSaveFileName(
        this, tr("Exporter en PDF"),
        QDir::homePath() + "/Examens_" +
            QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss") + ".pdf",
        "PDF (*.pdf)");
    if (filePath.isEmpty()) return;

    const QString filterCin = ui->lineEdit_6->text().trimmed();

    QPdfWriter pdf(filePath);
    pdf.setPageSize(QPageSize(QPageSize::A4));
    pdf.setResolution(300);

    QPainter p(&pdf);
    if (!p.isActive()) {
        QMessageBox::critical(this, "Erreur", "Impossible de créer le PDF.");
        return;
    }

    // mise en page
    const int margin = 80;
    const int pageW  = pdf.width()  - 2*margin;
    const int pageH  = pdf.height() - 2*margin;
    int y = margin;

    // colonnes: largeur fixes (en px imprimante)
    const int wType = 250;
    const int wDate = 350;
    const int wLieu = 300;
    const int wRes  = 250;
    const int wCin  = 250;
    const int rowH  = 70;

    const int xType = margin;
    const int xDate = xType + wType + 25;
    const int xLieu = xDate + wDate + 25;
    const int xRes  = xLieu + wLieu + 25;
    const int xCin  = xRes  + wRes  + 25;

    auto elide = [&](const QString& s, int width, const QFont& f) {
        QFontMetrics fm(f);
        return fm.elidedText(s, Qt::ElideRight, width - 6); // petite marge interne
    };

    // Titre
    p.setFont(QFont("Arial", 14, QFont::Bold));
    QString title = "Liste des examens";
    if (!filterCin.isEmpty()) title += " — CIN :" + filterCin;
    p.drawText(margin, y, pageW, 40, Qt::AlignLeft | Qt::AlignVCenter, title);
    y += 45;

    auto drawHeader = [&](){
        QRect r(margin, y, pageW, rowH);
        p.fillRect(r, QColor(41,127,110));
        p.setPen(Qt::white);
        p.setFont(QFont("Arial", 10, QFont::Bold));
        p.drawText(xType, y, wType, rowH, Qt::AlignVCenter, "Type");
        p.drawText(xDate, y, wDate, rowH, Qt::AlignVCenter, "Date");
        p.drawText(xLieu, y, wLieu, rowH, Qt::AlignVCenter, "Lieu");
        p.drawText(xRes,  y, wRes,  rowH, Qt::AlignVCenter, "Résultat");
        p.drawText(xCin,  y, wCin,  rowH, Qt::AlignVCenter, "CIN");
        y += rowH;
        p.setPen(Qt::black);
        p.setFont(QFont("Arial", 10));
    };

    auto newPage = [&](){
        pdf.newPage();
        y = margin;
        p.setFont(QFont("Arial", 16, QFont::Bold));
        QString t = "Liste des examens";
        if (!filterCin.isEmpty()) t += " — CIN " + filterCin;
        p.drawText(margin, y, pageW, 30, Qt::AlignLeft | Qt::AlignVCenter, t);
        y += 38;
        drawHeader();
    };

    drawHeader();

    // Requête (avec ou sans filtre CIN) – tri par vraie date
    QSqlQuery q;
    if (filterCin.isEmpty()) {
        q.prepare(
            "SELECT TYPE, TO_CHAR(DATE_EXAM,'DD/MM/YYYY'), LIEU, RESULTAT, CINC "
            "FROM EXAMENS ORDER BY DATE_EXAM ASC");
    } else {
        q.prepare(
            "SELECT TYPE, TO_CHAR(DATE_EXAM,'DD/MM/YYYY'), LIEU, RESULTAT, CINC "
            "FROM EXAMENS WHERE CINC = :cin ORDER BY DATE_EXAM ASC");
        q.bindValue(":cin", filterCin);
    }
    if (!q.exec()) {
        QMessageBox::critical(this, "Erreur SQL",
                              "Lecture impossible:\n" + q.lastError().text());
        return;
    }

    int row = 0;
    const QFont rowFont("Arial", 10);
    while (q.next()) {
        if (y > margin + pageH - rowH) newPage();

        // fond alterné
        if (row % 2) p.fillRect(QRect(margin, y, pageW, rowH), QColor(245,245,245));

        const QString type = q.value(0).toString();
        const QString date = q.value(1).toString();
        const QString lieu = q.value(2).toString();
        const QString res  = q.value(3).toString();
        const QString cin  = q.value(4).toString();

        // rognage propre
        p.setFont(rowFont);
        p.drawText(xType, y, wType, rowH, Qt::AlignVCenter,
                   elide(type, wType, rowFont));
        p.drawText(xDate, y, wDate, rowH, Qt::AlignVCenter,
                   elide(date, wDate, rowFont));
        p.drawText(xLieu, y, wLieu, rowH, Qt::AlignVCenter,
                   elide(lieu, wLieu, rowFont));
        p.drawText(xRes,  y, wRes,  rowH, Qt::AlignVCenter,
                   elide(res,  wRes,  rowFont));
        p.drawText(xCin,  y, wCin,  rowH, Qt::AlignVCenter,
                   elide(cin,  wCin,  rowFont));

        // petite ligne de séparation
        p.setPen(QColor(220,220,220));
        p.drawLine(margin, y+rowH-1, margin+pageW, y+rowH-1);
        p.setPen(Qt::black);

        y += rowH;
        ++row;
    }

    p.end();
    QMessageBox::information(this, "Succès", "PDF exporté avec succès.");
}


void MainWindow::setRole(Role r) {
    role_ = r;
    applyRole();
}


void MainWindow::applyRole() {
    const bool isAdmin    = (role_ == Role::Admin);
    const bool isRH       = (role_ == Role::RH);
    const bool isMobExam  = (role_ == Role::MobiliteExamens);
    const bool isFinance  = (role_ == Role::Finance);

    // Accueil always visible
    if (ui->btnAccueil) ui->btnAccueil->setVisible(true);

    // RH buttons
    if (ui->btnCandidat) ui->btnCandidat->setVisible(isAdmin || isRH);
    if (ui->btnMoniteur) ui->btnMoniteur->setVisible(isAdmin || isRH);

    // Vehicle and Exams - both for MobiliteExamens
    if (ui->btnVehicule) ui->btnVehicule->setVisible(isAdmin || isMobExam);
    if (ui->btnExamens)  ui->btnExamens->setVisible(isAdmin || isMobExam);

    // Finance button
    if (ui->btnFinance) ui->btnFinance->setVisible(isAdmin || isFinance);
}

//calendrier

// --- efface les formats
void MainWindow::clearCalendarMarks()
{
    auto map = ui->calendar->dateTextFormat();
    map.clear();
    ui->calendar->setDateTextFormat(QDate(), QTextCharFormat());
}

void MainWindow::refreshCalendarMarks()
{
    clearCalendarMarks();

    const int y = ui->calendar->yearShown();
    const int m = ui->calendar->monthShown();
    const QDate d1(y, m, 1);
    const QDate d2 = d1.addMonths(1);               // borne supérieure ouverte

    QSqlQuery q;
    q.prepare(R"(
        SELECT TRUNC(EXAM_DATE), COUNT(*)
        FROM EXAM_PLANNING
        WHERE EXAM_DATE >= TO_DATE(:d1,'YYYY-MM-DD')
          AND EXAM_DATE <  TO_DATE(:d2,'YYYY-MM-DD')
        GROUP BY TRUNC(EXAM_DATE)
    )");
    q.bindValue(":d1", d1.toString("yyyy-MM-dd"));
    q.bindValue(":d2", d2.toString("yyyy-MM-dd"));

    if (!q.exec()) {
        QMessageBox::critical(this, "Calendrier", q.lastError().text());
        return;
    }

    QTextCharFormat fmt;
    fmt.setBackground(QColor(41,127,110));  // vert de ton thème
    fmt.setForeground(Qt::white);
    fmt.setFontWeight(QFont::Bold);

    while (q.next()) {
        const QDate day = q.value(0).toDate();      // TRUNC(EXAM_DATE)
        if (day.isValid())
            ui->calendar->setDateTextFormat(day, fmt);
    }

    // Remplir la liste du jour sélectionné
    loadDayList(ui->calendar->selectedDate());
}



void MainWindow::on_calendar_selectionChanged() {
    loadDayList(ui->calendar->selectedDate());
}

// --- remplit la liste (QListWidget) des RDV du jour sélectionné
void MainWindow::loadDayList(const QDate& d)
{
    ui->listDay->clear();   // adapte au nom exact de ta QListWidget

    QSqlQuery q;
    q.prepare(R"(
        SELECT ID, TYPE, CINC, NVL(NOTE,'')
        FROM EXAM_PLANNING
        WHERE TRUNC(EXAM_DATE) = TRUNC(TO_DATE(:d,'YYYY-MM-DD'))
        ORDER BY EXAM_DATE
    )");
    q.bindValue(":d", d.toString("yyyy-MM-dd"));

    if (!q.exec()) {
        QMessageBox::critical(this, "Calendrier", q.lastError().text());
        return;
    }

    while (q.next()) {
        const int id        = q.value(0).toInt();
        const QString type  = q.value(1).toString();
        const QString cin   = q.value(2).toString();
        const QString note  = q.value(3).toString();

        // Badge + couleur selon le type
        const bool isConduite = (type.compare("Conduite", Qt::CaseInsensitive) == 0);
        const QString badge   = isConduite ? QString::fromUtf8("🟠") : QString::fromUtf8("🟢");

        // Texte multi-ligne
        const QString line1 = QString("%1  %2").arg(badge, type);           // 🟢  Code   |  🟠  Conduite
        const QString line2 = QString("CIN : %1").arg(cin);
        const QString line3 = note.trimmed().isEmpty() ? QString() : QString("Note : %1").arg(note.trimmed());
        const QString text  = line3.isEmpty() ? (line1 + "\n" + line2)
                                             : (line1 + "\n" + line2 + "\n" + line3);

        auto *it = new QListWidgetItem(text, ui->listDay);
        it->setData(Qt::UserRole, id);                                // pour retrouver l’ID
        it->setToolTip(QString("%1 — CIN %2%3")
                           .arg(type, cin, note.trimmed().isEmpty() ? QString() : QString(" — %1").arg(note)));

        // Un peu d’air entre les items
        it->setSizeHint(QSize(it->sizeHint().width(), 52));

        // Option : teinter le texte selon le type (si tu veux garder le vert global, commente ces deux lignes)
        if (isConduite) it->setForeground(QColor(220, 120, 0));       // orange doux
        else            it->setForeground(QColor(41, 127, 110));      // ton vert

        ui->listDay->addItem(it);
    }

}


// --- bouton Ajouter
void MainWindow::on_btnAddPlan_clicked()
{
    const QDate date = ui->calendar->selectedDate();
    const QString type = ui->comboTypePlan->currentText().trimmed();   // "Code" / "Conduite"
    const QString cin  = ui->editPlanCin->text().trimmed();
    const QString note = ui->editPlanNote->text().trimmed();

    if (!date.isValid() || type.isEmpty() || cin.isEmpty()) {
        QMessageBox::warning(this, "Calendrier", "Date, Type et CIN sont obligatoires.");
        return;
    }

    // 1) Anti-doublon
    {
        QSqlQuery chk;
        chk.prepare(R"(
            SELECT 1
            FROM EXAM_PLANNING
            WHERE TRUNC(EXAM_DATE) = TRUNC(TO_DATE(:d,'YYYY-MM-DD'))
              AND TYPE = :t
              AND CINC = :c
              AND NVL(NOTE,'') = NVL(:n,'')
        )");
        chk.bindValue(":d", date.toString("yyyy-MM-dd"));
        chk.bindValue(":t", type);
        chk.bindValue(":c", cin);
        chk.bindValue(":n", note);

        if (!chk.exec()) {
            QMessageBox::critical(this, "Calendrier (check)",
                                  "Erreur SQL:\n" + chk.lastError().text());
            return;
        }
        if (chk.next()) {
            QMessageBox::warning(this, "Calendrier",
                                 "Un rendez-vous identique existe déjà.");
            return;
        }
    }

    // 2) INSERT
    {
        QSqlQuery ins;
        ins.prepare(R"(
            INSERT INTO EXAM_PLANNING (EXAM_DATE, TYPE, CINC, NOTE)
            VALUES (TO_DATE(:d,'YYYY-MM-DD'), :t, :c, :n)
        )");
        ins.bindValue(":d", date.toString("yyyy-MM-dd"));
        ins.bindValue(":t", type);
        ins.bindValue(":c", cin);
        ins.bindValue(":n", note);

        if (!ins.exec()) {
            QMessageBox::critical(this, "Calendrier (insert)",
                                  "Erreur SQL:\n" + ins.lastError().text());
            return;
        }
    }

    // 3) Nettoyage et rafraîchissement UI
    ui->editPlanNote->clear();
    // recharge les marqueurs et la liste du jour (adapte si tes noms diffèrent)
    refreshCalendarMarks();
    loadDayList(date);

    QMessageBox::information(this, "Calendrier", "Rendez-vous ajouté.");
}


// --- bouton Supprimer (supprime l'item sélectionné)
void MainWindow::on_btnDelPlan_clicked()
{
    auto *item = ui->listDay->currentItem();
    if (!item) { QMessageBox::warning(this,"Calendrier","Sélectionne un rendez-vous."); return; }

    const int id = item->data(Qt::UserRole).toInt();
    QSqlQuery q;
    q.prepare("DELETE FROM EXAM_PLANNING WHERE ID=:id");
    q.bindValue(":id", id);

    if (!q.exec()) {
        QMessageBox::critical(this, "Calendrier", q.lastError().text());
        return;
    }

    refreshCalendarMarks();
}


void MainWindow::on_btnRefreshPlan_clicked() {
    refreshCalendarMarks();
}

// ============================================================================
// VEHICLE MANAGEMENT FUNCTIONS - COMPLETE AND VERIFIED
// Add these to the END of mainwindow.cpp (after calendar functions)
// ============================================================================

// Load vehicle data into table
void MainWindow::loadVehicleData()
{
    if (!ui->tableWidget) return;

    ui->tableWidget->setRowCount(0);
    ui->tableWidget->setColumnCount(5);

    QStringList headers;
    headers << "ID" << "Matricule" << "Marque" << "Kilométrage" << "État";
    ui->tableWidget->setHorizontalHeaderLabels(headers);

    QSqlQuery q;
    q.prepare("SELECT ID_U, MATRICULE, MARQUE, KILOMETRAGE, ETAT FROM VEHICULE ORDER BY MATRICULE");

    if (!q.exec()) {
        qDebug() << "Erreur chargement véhicules:" << q.lastError().text();
        return;
    }

    int row = 0;
    while (q.next()) {
        ui->tableWidget->insertRow(row);

        ui->tableWidget->setItem(row, 0, new QTableWidgetItem(q.value(0).toString()));
        ui->tableWidget->setItem(row, 1, new QTableWidgetItem(q.value(1).toString()));
        ui->tableWidget->setItem(row, 2, new QTableWidgetItem(q.value(2).toString()));
        ui->tableWidget->setItem(row, 3, new QTableWidgetItem(q.value(3).toString() + " km"));
        ui->tableWidget->setItem(row, 4, new QTableWidgetItem(q.value(4).toString()));

        row++;
    }
}

void MainWindow::clearVehicleForm()
{
    if (ui->lineEdit) ui->lineEdit->clear();
    if (ui->lineEdit_2) ui->lineEdit_2->clear();
    if (ui->lineEdit_4) ui->lineEdit_4->clear();
    if (ui->comboBox) ui->comboBox->setCurrentIndex(0);
}

// Add vehicle
void MainWindow::on_pushButton_clicked()
{
    if (role_ != Role::Admin && role_ != Role::MobiliteExamens) {
        QMessageBox::warning(this, "Droits", "Action réservée à l'admin ou mobilité.");
        return;
    }

    QString marque = ui->lineEdit->text().trimmed();
    QString matricule = ui->lineEdit_2->text().trimmed().toUpper();
    QString kmStr = ui->lineEdit_4->text().trimmed();
    QString etat = ui->comboBox->currentText().trimmed();

    if (marque.isEmpty() || matricule.isEmpty() || kmStr.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Tous les champs sont obligatoires.");
        return;
    }

    // Validate matricule format
    QRegularExpression matriculeRegex("^\\d{1,3}[A-Z]{2}\\d{1,4}$");
    if (!matriculeRegex.match(matricule).hasMatch()) {
        QMessageBox::warning(this, "Erreur",
                             "Format matricule invalide.\nFormat attendu: 123TU1234");
        return;
    }

    bool ok;
    int km = kmStr.toInt(&ok);
    if (!ok || km < 0) {
        QMessageBox::warning(this, "Erreur", "Kilométrage invalide.");
        return;
    }

    // Generate ID
    QSqlQuery idQuery;
    int id = 1;
    if (idQuery.exec("SELECT NVL(MAX(ID_U), 0) + 1 FROM VEHICULE")) {
        if (idQuery.next()) {
            id = idQuery.value(0).toInt();
        }
    }

    // Insert into database
    QSqlQuery q;
    q.prepare("INSERT INTO VEHICULE (ID_U, MATRICULE, MARQUE, KILOMETRAGE, ETAT) "
              "VALUES (:id, :mat, :marque, :km, :etat)");
    q.bindValue(":id", id);
    q.bindValue(":mat", matricule);
    q.bindValue(":marque", marque);
    q.bindValue(":km", km);
    q.bindValue(":etat", etat);

    if (q.exec()) {
        loadVehicleData();
        clearVehicleForm();
        QMessageBox::information(this, "Succès", "Véhicule ajouté.");
    } else {
        QMessageBox::critical(this, "Erreur", "Échec d'ajout:\n" + q.lastError().text());
    }
}

// Search vehicle
void MainWindow::on_pushButton_2_clicked()
{
    QString search = ui->lineEdit_3->text().trimmed();

    if (!ui->tableWidget) return;

    if (search.isEmpty()) {
        loadVehicleData();
        return;
    }

    ui->tableWidget->setRowCount(0);

    QSqlQuery q;
    q.prepare("SELECT ID_U, MATRICULE, MARQUE, KILOMETRAGE, ETAT FROM VEHICULE "
              "WHERE UPPER(MATRICULE) LIKE UPPER(:s) OR UPPER(MARQUE) LIKE UPPER(:s) "
              "ORDER BY MATRICULE");
    q.bindValue(":s", "%" + search + "%");

    if (!q.exec()) {
        QMessageBox::critical(this, "Erreur", "Recherche échouée:\n" + q.lastError().text());
        return;
    }

    int row = 0;
    while (q.next()) {
        ui->tableWidget->insertRow(row);
        ui->tableWidget->setItem(row, 0, new QTableWidgetItem(q.value(0).toString()));
        ui->tableWidget->setItem(row, 1, new QTableWidgetItem(q.value(1).toString()));
        ui->tableWidget->setItem(row, 2, new QTableWidgetItem(q.value(2).toString()));
        ui->tableWidget->setItem(row, 3, new QTableWidgetItem(q.value(3).toString() + " km"));
        ui->tableWidget->setItem(row, 4, new QTableWidgetItem(q.value(4).toString()));
        row++;
    }

    if (row == 0) {
        QMessageBox::information(this, "Recherche", "Aucun véhicule trouvé.");
    }
}

// Show all vehicles
void MainWindow::on_pushButton_4_clicked()
{
    loadVehicleData();
    if (ui->lineEdit_3) ui->lineEdit_3->clear();
}

// Delete all vehicles
void MainWindow::on_pushButton_5_clicked()
{
    if (role_ != Role::Admin) {
        QMessageBox::warning(this, "Droits", "Action réservée à l'admin uniquement.");
        return;
    }

    auto reply = QMessageBox::question(this, "Confirmation",
                                       "Supprimer TOUS les véhicules ?",
                                       QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        QSqlQuery q("DELETE FROM VEHICULE");

        if (q.exec()) {
            loadVehicleData();
            QMessageBox::information(this, "Succès", "Tous les véhicules supprimés.");
        } else {
            QMessageBox::critical(this, "Erreur", "Suppression échouée:\n" + q.lastError().text());
        }
    }
}

// Delete selected vehicle
void MainWindow::on_pushButton_6_clicked()
{
    if (role_ != Role::Admin && role_ != Role::MobiliteExamens) {
        QMessageBox::warning(this, "Droits", "Action réservée à l'admin.");
        return;
    }

    int row = ui->tableWidget->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "Erreur", "Sélectionnez un véhicule.");
        return;
    }

    int id = ui->tableWidget->item(row, 0)->text().toInt();

    auto reply = QMessageBox::question(this, "Confirmation",
                                       "Supprimer ce véhicule ?",
                                       QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        QSqlQuery q;
        q.prepare("DELETE FROM VEHICULE WHERE ID_U = :id");
        q.bindValue(":id", id);

        if (q.exec()) {
            loadVehicleData();
            QMessageBox::information(this, "Succès", "Véhicule supprimé.");
        } else {
            QMessageBox::critical(this, "Erreur", "Suppression échouée:\n" + q.lastError().text());
        }
    }
}

// Sort vehicles
void MainWindow::on_pushButton_9_clicked()
{
    QString criteria = ui->comboBox_3->currentText().trimmed();

    if (!ui->tableWidget) return;

    ui->tableWidget->setRowCount(0);

    QSqlQuery q;
    if (criteria == "en kilometrage") {
        q.prepare("SELECT ID_U, MATRICULE, MARQUE, KILOMETRAGE, ETAT FROM VEHICULE ORDER BY KILOMETRAGE DESC");
    } else if (criteria == "en etat") {
        q.prepare("SELECT ID_U, MATRICULE, MARQUE, KILOMETRAGE, ETAT FROM VEHICULE ORDER BY ETAT, MATRICULE");
    } else {
        q.prepare("SELECT ID_U, MATRICULE, MARQUE, KILOMETRAGE, ETAT FROM VEHICULE ORDER BY MATRICULE");
    }

    if (!q.exec()) {
        QMessageBox::critical(this, "Erreur", "Tri échoué:\n" + q.lastError().text());
        return;
    }

    int row = 0;
    while (q.next()) {
        ui->tableWidget->insertRow(row);
        ui->tableWidget->setItem(row, 0, new QTableWidgetItem(q.value(0).toString()));
        ui->tableWidget->setItem(row, 1, new QTableWidgetItem(q.value(1).toString()));
        ui->tableWidget->setItem(row, 2, new QTableWidgetItem(q.value(2).toString()));
        ui->tableWidget->setItem(row, 3, new QTableWidgetItem(q.value(3).toString() + " km"));
        ui->tableWidget->setItem(row, 4, new QTableWidgetItem(q.value(4).toString()));
        row++;
    }
}

// Modify vehicle
void MainWindow::on_pushButton_10_clicked()
{
    if (role_ != Role::Admin && role_ != Role::MobiliteExamens) {
        QMessageBox::warning(this, "Droits", "Action réservée à l'admin.");
        return;
    }

    int row = ui->tableWidget->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "Erreur", "Sélectionnez un véhicule dans le tableau.");
        return;
    }

    int id = ui->tableWidget->item(row, 0)->text().toInt();
    QString champ = ui->comboBox_2->currentText().trimmed();
    QString newValue = ui->lineEdit_5->text().trimmed();

    if (newValue.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Entrez une nouvelle valeur.");
        return;
    }

    QString column;
    if (champ == "marque") {
        column = "MARQUE";
    } else if (champ == "matricule") {
        column = "MATRICULE";
    } else if (champ == "kilometrage") {
        column = "KILOMETRAGE";
        bool ok;
        newValue.toInt(&ok);
        if (!ok) {
            QMessageBox::warning(this, "Erreur", "Le kilométrage doit être un nombre!");
            return;
        }
    } else if (champ == "etat") {
        column = "ETAT";
    } else {
        QMessageBox::warning(this, "Erreur", "Champ invalide.");
        return;
    }

    QSqlQuery q;
    q.prepare(QString("UPDATE VEHICULE SET %1 = :val WHERE ID_U = :id").arg(column));
    q.bindValue(":val", newValue);
    q.bindValue(":id", id);

    if (q.exec()) {
        loadVehicleData();
        ui->lineEdit_5->clear();
        QMessageBox::information(this, "Succès", "Véhicule modifié.");
    } else {
        QMessageBox::critical(this, "Erreur", "Modification échouée:\n" + q.lastError().text());
    }
}
