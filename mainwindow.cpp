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


#include "exam.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    const auto go = [&](QWidget* page){ ui->stack->setCurrentWidget(page); };
    connect(ui->btnExamens,  &QPushButton::clicked, this, [=]{ go(ui->pageExamens);  });
    connect(ui->btnCandidat, &QPushButton::clicked, this, [=]{ go(ui->pageCandidats); });
    connect(ui->btnMoniteur, &QPushButton::clicked, this, [=]{ go(ui->pageMoniteur);  });
    connect(ui->btnVehicule, &QPushButton::clicked, this, [=]{ go(ui->pageVehicules); });
    connect(ui->btnFinance,  &QPushButton::clicked, this, [=]{ go(ui->pageFinance);  });

    // page par défaut :
    ui->stack->setCurrentWidget(ui->pageExamens);
    auto setMaskForField = [this](const QString &field){
        ui->lineEdit_7->setInputMask("");
        ui->lineEdit_7->setMaxLength(64);
        ui->lineEdit_7->clear();

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
    // initialiser + suivre les changements du combo
    setMaskForField(ui->comboBox_4->currentText());
    connect(ui->comboBox_4, &QComboBox::currentTextChanged, this, setMaskForField);


    // sélection par lignes
    ui->tableViewExams->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableViewExams->setSelectionMode(QAbstractItemView::SingleSelection);

    // un peu de confort visuel, sans tout repeindre
    ui->tableViewExams->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableViewExams->setAlternatingRowColors(false);
    // activer le survol (léger)
    ui->tableViewExams->setMouseTracking(true);
    ui->tableViewExams->viewport()->setMouseTracking(true);
    ui->tableViewExams->viewport()->setAttribute(Qt::WA_Hover, true);

    // Style MINIMAL : cellule survolée très légère + sélection lisible
    ui->tableViewExams->setStyleSheet(
        "QTableView::item:hover { background: rgba(0,0,0,0.06); }"
        "QTableView::item:selected { background: #CDE5FF; color: black; }"
        );

    loadTableData();

    if (ui->tableViewExams->horizontalHeader())
        ui->tableViewExams->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
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

void MainWindow::loadTableData()
{
    detachModel();
    modelExams = Exam::afficherTous();
    ui->tableViewExams->setModel(modelExams);
}

void MainWindow::clearForm()
{
    ui->lineEdit_3->clear(); // Lieu
    ui->lineEdit_5->clear(); // CIN
    ui->dateEdit->setDate(QDate::currentDate());
}


void MainWindow::on_addButton_clicked()
{
    const QString type     = ui->comboBox_3->currentText();
    const QString lieu     = ui->lineEdit_3->text();
    const QString date     = ui->dateEdit->date().toString("dd/MM/yyyy");
    const QString resultat = ui->comboBox_6->currentText();
    const QString cin      = ui->lineEdit_5->text();
    if (role_ != Role::Admin) {
        QMessageBox::warning(this, "Droits", "Action réservée à l'admin.");
        return;
    }
    if (lieu.isEmpty() || cin.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez remplir tous les champs");
        return;
    }

    Exam e(type, date, lieu, resultat, cin);

    detachModel();

    if (e.ajouter()) {
        loadTableData();
        clearForm();
        QMessageBox::information(this, "Succès", "Examen ajouté");
    } else {
        QMessageBox::critical(this, "Erreur", "Échec d'ajout en base");
        loadTableData();
    }
}

void MainWindow::on_deleteButton_clicked()
{
    const int row = ui->tableViewExams->currentIndex().row();
    if (row < 0) {
        QMessageBox::warning(this, "Erreur", "Veuillez sélectionner une ligne");
        return;
    }

    QAbstractItemModel *m = ui->tableViewExams->model();
    const QString cin  = m->index(row, 4).data().toString();
    const QString date = m->index(row, 1).data().toString();

    detachModel();
    if (role_ != Role::Admin) {
        QMessageBox::warning(this, "Droits", "Action réservée à l'admin.");
        return;
    }
    if (Exam::supprimer(cin, date)) {
        loadTableData();
        QMessageBox::information(this, "Succès", "Examen supprimé");
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de suppression");
        loadTableData();
    }
}

void MainWindow::on_btnSearch_clicked()
{
    const QString searchCin = ui->lineEdit_6->text().trimmed();

    detachModel();

    if (searchCin.isEmpty()) {
        modelExams = Exam::afficherTous();
    } else {
        modelExams = Exam::afficherParCin(searchCin);
    }
    ui->tableViewExams->setModel(modelExams);
}



void MainWindow::on_tabWidget_currentChanged(int index)
{
    Q_UNUSED(index);

}
void MainWindow::on_modifyButton_clicked()
{
    auto *m   = qobject_cast<QSqlQueryModel*>(ui->tableViewExams->model());
    auto *sel = ui->tableViewExams->selectionModel();
    if (role_ != Role::Admin) {
        QMessageBox::warning(this, "Droits", "Action réservée à l'admin.");
        return;
    }
    if (!m || !sel || !sel->hasSelection()) {
        QMessageBox::warning(this, "Erreur", "Sélectionnez une ligne du tableau.");
        return;
    }

    const int row = sel->currentIndex().row();
    const QString cinOld = m->data(m->index(row, 4)).toString();
    const QString dS     = m->data(m->index(row, 1)).toString();   // "DD/MM/YYYY"
    const QDate   oldDate= QDate::fromString(dS, "dd/MM/yyyy");

    const QString field = ui->comboBox_4->currentText();           // Type/Date/Lieu/Résultat/CIN
    const QString nv    = ui->lineEdit_7->text().trimmed();
    if (nv.isEmpty()) { QMessageBox::warning(this, "Erreur", "Entrez une nouvelle valeur."); return; }

    bool ok = false;
    detachModel();

    if (field == "Date") {
        const QDate nd = QDate::fromString(nv, "dd/MM/yyyy");
        if (!nd.isValid()) { QMessageBox::warning(this, "Erreur", "Date invalide (jj/mm/aaaa)."); loadTableData(); return; }
        ok = Exam::modifierDate(cinOld, oldDate, nd);

    } else if (field == "Type") {
        const QString v = normType(nv);                   // "code" -> "Code"
        if (v.isEmpty()) { QMessageBox::warning(this, "Erreur", "Type = Code ou Conduite."); loadTableData(); return; }
        ok = Exam::modifierTexte(cinOld, oldDate, "TYPE", v);

    } else if (field == "Lieu") {
        if (!isLettersSpaces(nv)) { QMessageBox::warning(this, "Erreur", "Lieu: lettres/espaces seulement."); loadTableData(); return; }
        ok = Exam::modifierTexte(cinOld, oldDate, "LIEU", nv);

    } else if (field == "Résultat") {
        const QString v = normRes(nv);                    // "reussite" -> "Réussite"
        if (v.isEmpty()) { QMessageBox::warning(this, "Erreur", "Résultat = Réussite ou Echec."); loadTableData(); return; }
        ok = Exam::modifierTexte(cinOld, oldDate, "RESULTAT", v);

    } else if (field == "CIN") {
        if (!isExactly8Digits(nv)) { QMessageBox::warning(this, "Erreur", "CIN: exactement 8 chiffres."); loadTableData(); return; }
        ok = Exam::modifierTexte(cinOld, oldDate, "CINC", nv);
    } else {
        loadTableData();
        return;
    }

    loadTableData();

    if (ok) QMessageBox::information(this, "Succès", "Modification enregistrée.");
    else    QMessageBox::critical(this, "Erreur", "Échec de modification (voir console).");
}

void MainWindow::on_triButton_clicked() {
    const bool asc = (ui->comboTri->currentText()=="Croissant");
    detachModel();
    modelExams = Exam::afficherParDate(asc);
    ui->tableViewExams->setModel(modelExams);
}


void MainWindow::on_exportButton_clicked()
{
    const QString filePath = QFileDialog::getSaveFileName(
        this, tr("Exporter en PDF"),
        QDir::homePath() + "/Examens_" +
            QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss") + ".pdf",
        "PDF (*.pdf)"
        );
    if (role_ != Role::Admin) {
        QMessageBox::warning(this, "Droits", "Action réservée à l'admin.");
        return;
    }
    if (filePath.isEmpty()) return;

    QPdfWriter pdf(filePath);
    pdf.setPageSize(QPageSize(QPageSize::A4));
    pdf.setResolution(300);

    QPainter p(&pdf);
    if (!p.isActive()) {
        QMessageBox::critical(this, "Erreur", "Impossible de créer le PDF.");
        return;
    }

    const int margin = 80;
    const int lineH  = 28;
    int y = margin + 40;

    auto drawHeader = [&](){
        p.setFont(QFont("Arial", 16, QFont::Bold));
        p.drawText(margin, margin, tr("Liste des examens"));
        p.setFont(QFont("Arial", 10, QFont::Bold));
        p.drawText(margin +   0, y, "Type");
        p.drawText(margin + 180, y, "Date");
        p.drawText(margin + 320, y, "Lieu");
        p.drawText(margin + 560, y, "Résultat");
        p.drawText(margin + 760, y, "CIN");
        y += lineH;
        p.setFont(QFont("Arial", 10));
    };
    auto newPage = [&](){
        pdf.newPage();
        y = margin + 40;
        drawHeader();
    };

    drawHeader();

    QSqlQuery q;
    if (!q.exec(
            "SELECT TYPE, "
            "       TO_CHAR(DATE_EXAM,'DD/MM/YYYY') AS DATE_EXAM, "
            "       LIEU, RESULTAT, CINC "
            "FROM EXAMENS "
            "ORDER BY DATE_EXAM ASC"))
    {
        QMessageBox::critical(this, "Erreur SQL",
                              "Échec de lecture des données :\n" + q.lastError().text());
        return;
    }

    while (q.next()) {
        if (y > pdf.height() - margin) newPage();

        const QString type = q.value(0).toString();
        const QString date = q.value(1).toString();
        const QString lieu = q.value(2).toString();
        const QString res  = q.value(3).toString();
        const QString cin  = q.value(4).toString();

        p.drawText(margin +   0, y, type);
        p.drawText(margin + 180, y, date);
        p.drawText(margin + 320, y, lieu);
        p.drawText(margin + 560, y, res);
        p.drawText(margin + 760, y, cin);
        y += lineH;
    }

    p.end();
    QMessageBox::information(this, "Succès", "PDF exporté avec succès !");
}

void MainWindow::setRole(Role r) {
    role_ = r;
    applyRole();
}


void MainWindow::applyRole() {
    const bool isAdmin    = (role_ == Role::Admin);
    const bool isMoniteur = (role_ == Role::Moniteur);

    ui->btnCandidat->setVisible(isAdmin);
    ui->btnFinance->setVisible(isAdmin);
    ui->btnMoniteur->setVisible(isAdmin || isMoniteur);
    ui->btnVehicule->setVisible(isAdmin || isMoniteur);


}

