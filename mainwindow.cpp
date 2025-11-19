#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMessageBox>
#include <QDate>
#include <QHeaderView>
#include <QAbstractItemModel>
#include "transaction.h"
#include <QSqlQueryModel>
#include <QSqlRecord>
#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QChart>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>
#include <QVBoxLayout>
#include <QSqlQueryModel>
#include <QSqlRecord>
#include <QVBoxLayout>
#include <QPageLayout>
#include <QPageSize>
#include <QPainter>
#include <QMessageBox>
#include <QPrinter>
#include <QFileDialog>
#include <QPainter>
#include <QtCharts/QChartView>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarSeries>
#include <QtCharts/QChart>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>
#include <QDebug>
#include <QtCharts/QPieSeries>
#include "qr/qrcodegen.hpp"
using namespace qrcodegen;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    chatWindow = new chat(this);
    chatWindow->setMainWindow(this);

    // === Create chart view for the Dashboard ===
    chartViewBalance = new QChartView(ui->groupBox_search_7);
    chartViewBalance->setRenderHint(QPainter::Antialiasing);
    chartViewBalance->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    chartViewBalance->setGeometry(ui->groupBox_search_7->rect());

    // First load
    updateDashboard();

    loadTableData();
    loadStatistics();
    if (ui->tableViewTRANS->horizontalHeader())
        ui->tableViewTRANS->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

MainWindow::~MainWindow()
{
    detachModel();
    delete ui;
}

void MainWindow::detachModel()
{
    if (modelTransactions) {
        ui->tableViewTRANS->setModel(nullptr);
        delete modelTransactions;
        modelTransactions = nullptr;
    }
}


void MainWindow::updateDashboard()
{
    double totalIncome  = 0.0;
    double totalExpense = 0.0;

    QSqlQuery query;

    // ---------- Total revenues ----------
    if (!query.exec("SELECT COALESCE(SUM(AMOUNT), 0) FROM TRANSACTION WHERE TYPE = 'incom'")) {
        qDebug() << "Error income:" << query.lastError().text();
        return;
    }
    if (query.next()) {
        totalIncome = query.value(0).toDouble();
    }

    // ---------- Total expenses ----------
    if (!query.exec("SELECT COALESCE(SUM(AMOUNT), 0) FROM TRANSACTION WHERE TYPE = 'expense'")) {
        qDebug() << "Error expense:" << query.lastError().text();
        return;
    }
    if (query.next()) {
        totalExpense = query.value(0).toDouble();
    }

    double balance = totalIncome - totalExpense;

    // ---------- Update LCDs ----------
    ui->lcdIncome->display(totalIncome);
    ui->lcdExpense->display(totalExpense);
    ui->lcdBalance->display(balance);

    // Optional: change color of balance depending on sign
    if (balance >= 0)
        ui->lcdBalance->setStyleSheet("color: green;");
    else
        ui->lcdBalance->setStyleSheet("color: red;");

    // ---------- Mini pie chart ----------
    if (chartViewBalance) {
        // Remove old chart to avoid leaks
        if (chartViewBalance->chart())
            delete chartViewBalance->chart();

        QPieSeries *series = new QPieSeries();
        series->append("Revenus",  totalIncome);
        series->append("Dépenses", totalExpense);

        QChart *chart = new QChart();
        chart->addSeries(series);
        chart->setTitle("Répartition revenus / dépenses");
        chart->legend()->setAlignment(Qt::AlignBottom);

        chartViewBalance->setChart(chart);
    }
}


void MainWindow::loadTableData()
{
    detachModel();
    modelTransactions = Transaction::afficherTous();
    ui->tableViewTRANS->setModel(modelTransactions);
    ui->tableViewTRANS->verticalHeader()->setVisible(false);
    // (re)connect row change signal
    if (ui->tableViewTRANS->selectionModel()) {
        connect(ui->tableViewTRANS->selectionModel(), &QItemSelectionModel::currentRowChanged,
                this, &MainWindow::onTableCurrentChanged);
    }
}

void MainWindow::onTableCurrentChanged(const QModelIndex &current, const QModelIndex &)
{
    if (!current.isValid()) return;
    int row = current.row();
    auto *m = ui->tableViewTRANS->model();
    ui->TYPE->setCurrentText(m->index(row, 1).data().toString());
    ui->dateEdit->setDate(QDate::fromString(m->index(row, 2).data().toString(), "dd/MM/yyyy"));
    ui->MONTANT->setText(m->index(row, 3).data().toString());
    ui->CATEGORY->setCurrentText(m->index(row, 4).data().toString());
    ui->METHOD->setCurrentText(m->index(row, 5).data().toString());
}

void MainWindow::on_exportButton_clicked()
{
    QTableView* view = ui->tableViewTRANS;
    if (!view) return;

    QString filePath = QFileDialog::getSaveFileName(
        this, "Save PDF", "", "PDF Files (*.pdf)");
    if (filePath.isEmpty()) return;
    if (!filePath.endsWith(".pdf", Qt::CaseInsensitive)) filePath += ".pdf";

    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(filePath);
    printer.setPageSize(QPageSize(QPageSize::A4));
    printer.setPageMargins(QMarginsF(12, 12, 12, 12), QPageLayout::Millimeter);

    QPainter painter(&printer);

    const QRectF pageRect = printer.pageLayout().paintRectPixels(printer.resolution());
    const QSize  contentSize = ui->tableViewTRANS->viewport()->size();   // or your view
    const qreal  sx = pageRect.width()  / qreal(contentSize.width());
    const qreal  sy = pageRect.height() / qreal(contentSize.height());

    painter.save();
    painter.translate(pageRect.topLeft());
    painter.scale(std::min(sx, sy), std::min(sx, sy));
    ui->tableViewTRANS->render(&painter);   // or your view
    painter.restore();

    painter.end();
    QMessageBox::information(this, "Export", "Table exported to PDF.");
}

void MainWindow::on_UpdateButton_clicked()
{
    // must have a selection
    QModelIndex idx = ui->tableViewTRANS->currentIndex();
    if (!idx.isValid()) {
        QMessageBox::warning(this, "Erreur", "Veuillez sélectionner une ligne à modifier");
        return;
    }
    int row = idx.row();
    auto *m = ui->tableViewTRANS->model();
    int id = m->index(row, 0).data().toInt(); // ID_T in col 0

    // read fields
    QString type     = ui->TYPE->currentText();
    QString category = ui->CATEGORY->currentText();
    QString date     = ui->dateEdit->date().toString("dd/MM/yyyy");
    QString montantS = ui->MONTANT->text().trimmed();
    QString methode  = ui->METHOD->currentText();

    bool ok=false;
    double amount = montantS.toDouble(&ok);
    if (!ok) { QMessageBox::warning(this,"Erreur","Montant invalide"); return; }

    Transaction t;
    t.setType(type);
    t.setDate(date);
    t.setAmount(amount);
    t.setCategory(category);
    t.setMethode(methode);

    detachModel();
    if (t.mettreAJour(id)) {
        loadTableData();
        loadStatistics();
        updateDashboard();
        QMessageBox::information(this, "Succès", "Transaction mise à jour");
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de mise à jour");
        loadTableData();
    }
}


void MainWindow::loadStatistics()
{
    if (modelStats) {
        delete modelStats;
        modelStats = nullptr;
    }

    modelStats = Transaction::afficherStat();

    // Create bar chart
    QBarSet *set = new QBarSet("Total des transactions");
    QStringList categories;

    for (int i = 0; i < modelStats->rowCount(); ++i) {
        QString year = modelStats->record(i).value("ANNEE").toString();
        double total = modelStats->record(i).value("TOTAL").toDouble();

        categories << year;
        *set << total;
    }

    QBarSeries *series = new QBarSeries();
    series->append(set);

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Statistiques des transactions par année");
    chart->setAnimationOptions(QChart::SeriesAnimations);

    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(categories);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis();
    axisY->setTitleText("Montant (DT)");
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    // Remove old chart if exists
    if (chartView) {
        ui->groupBox_search_2->layout()->removeWidget(chartView);
        delete chartView;
    }

    chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    // Add to the statistics groupbox
    if (!ui->groupBox_search_2->layout()) {
        QVBoxLayout *layout = new QVBoxLayout(ui->groupBox_search_2);
        ui->groupBox_search_2->setLayout(layout);
    }
    ui->groupBox_search_2->layout()->addWidget(chartView);
}

void MainWindow::clearForm()
{
    ui->CATEGORY->setCurrentIndex(0);
    ui->MONTANT->clear();
    ui->METHOD->setCurrentIndex(0);
    ui->dateEdit->setDate(QDate::currentDate());
}

void MainWindow::on_addButton_clicked()
{
    const QString type     = ui->TYPE->currentText();
    const QString category = ui->CATEGORY->currentText();
    const QString date     = ui->dateEdit->date().toString("dd/MM/yyyy");
    const QString montant  = ui->MONTANT->text().trimmed();
    const QString methode  = ui->METHOD->currentText();

    // Validate amount
    bool ok;
    double amount = montant.toDouble(&ok);

    if (!ok || montant.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez entrer un montant valide");
        return;
    }


    Transaction t;
    static int nextId = 1;
    t.setIdT(nextId++);
    t.setType(type);
    t.setDate(date);
    t.setAmount(amount);
    t.setCategory(category);
    t.setMethode(methode);

    detachModel();
    if (t.ajouter()) {
        loadTableData();
        updateDashboard();
        clearForm();
        QMessageBox::information(this, "Succès", "Transaction ajoutée");
    } else {
        QMessageBox::critical(this, "Erreur", "Échec d'ajout en base");
        loadTableData();
    }
}

void MainWindow::on_deleteButton_clicked()
{
    const int row = ui->tableViewTRANS->currentIndex().row();
    if (row < 0) {
        QMessageBox::warning(this, "Erreur", "Veuillez sélectionner une ligne");
        return;
    }

    QAbstractItemModel *m = ui->tableViewTRANS->model();
    const int id_t = m->index(row, 0).data().toInt(); // ID_T is in column 0

    detachModel();
    if (Transaction::supprimer(id_t)) {
        loadTableData();
        updateDashboard();
        QMessageBox::information(this, "Succès", "Transaction supprimée");
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de suppression");
        loadTableData();
    }
}

void MainWindow::on_btnSearch_clicked()
{    QString searchText = ui->lineEdit_recherche_transaction->text();

    if (searchText.isEmpty()) {
        modelTransactions = Transaction::afficherTous();
        return;
    }

    QSqlQueryModel *model = new QSqlQueryModel();

    QString queryString = "SELECT * FROM TRANSACTION WHERE "
                          "CAST(ID_T AS VARCHAR(50)) LIKE '%" + searchText + "%' OR "
                                         "TYPE LIKE '%" + searchText + "%' OR "
                                         "AMOUNT LIKE '%" + searchText + "%' OR "
                                         "METHODE LIKE '%" + searchText + "%' OR "
                                         "CATEGORY LIKE '%" + searchText + "%'";

    model->setQuery(queryString);

    if (model->rowCount() > 0) {
        ui->tableViewTRANS->setModel(model);
    } else {
        QMessageBox::information(this, "Recherche", "Aucune transaction trouvée.");
    }
}

void MainWindow::on_exportButton_2_clicked()
{
    loadTableData();
    QMessageBox::information(this, "Info", "Données rechargées");
}

void MainWindow::on_tabWidget_currentChanged(int index)
{
    Q_UNUSED(index);
}

void MainWindow::on_TRIE_activated(int index)
{
    QString selectedSort = ui->TRIE->currentText();
    int sortType = 1;

    if (selectedSort == "Date") {
        sortType = 2;
    } else if (selectedSort == "Type") {
        sortType = 3;
    } else if (selectedSort == "ID") {
        sortType = 4;
    } else if (selectedSort == "Montant") {
        sortType = 5;
    }

    detachModel();
    modelTransactions = Transaction::tri(sortType);
    ui->tableViewTRANS->setModel(modelTransactions);
}

QString MainWindow::buildDashboardText() const
{
    // 🔁 CHANGE THESE NAMES to match your QLCDNumber widgets
    double revenus  = ui->lcdIncome->value();      // e.g. ui->lcdTotalRevenus
    double depenses = ui->lcdExpense->value();     // e.g. ui->lcdTotalDepenses
    double balance  = ui->lcdBalance->value();      // e.g. ui->lcdBalance

    QString revenusStr  = QString::number(revenus,  'f', 2);
    QString depensesStr = QString::number(depenses, 'f', 2);
    QString balanceStr  = QString::number(balance,  'f', 2);

    QString date = QDate::currentDate().toString("dd.MM.yyyy");

    QString text;
    text += "Gestion Transaction : BILAN \n\n";
    text += "Revenus totaux : " + revenusStr +"  " + "DT"+"\n";
    text += "Depenses totales : " + depensesStr +"  " + "DT"+"\n";
    text += "Balance : "       + balanceStr +"  " + "DT"+"\n\n";
    text += "Date : " + date + "\n";

    return text;
}

QImage MainWindow::generateQrImage(const QString &text, int pixelsPerModule)
{
    const std::string utf8 = text.toUtf8().constData();
    QrCode qr = QrCode::encodeText(utf8.c_str(), QrCode::Ecc::QUARTILE);

    int size = qr.getSize();
    int imgSize = size * pixelsPerModule;

    QImage image(imgSize, imgSize, QImage::Format_RGB32);
    image.fill(Qt::white);

    QPainter painter(&image);
    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::black);

    for (int y = 0; y < size; ++y) {
        for (int x = 0; x < size; ++x) {
            if (qr.getModule(x, y)) {
                QRect r(x * pixelsPerModule,
                        y * pixelsPerModule,
                        pixelsPerModule,
                        pixelsPerModule);
                painter.drawRect(r);
            }
        }
    }

    painter.end();
    return image;
}

void MainWindow::on_btnGenerateQr_clicked()
{
    QString data = buildDashboardText();
    QImage qrImg = generateQrImage(data, 8);   // 8 px per module = good quality

    QPixmap pix = QPixmap::fromImage(qrImg)
                      .scaled(ui->labelQrDashboard->size(),
                              Qt::KeepAspectRatio,
                              Qt::SmoothTransformation);

    ui->labelQrDashboard->setPixmap(pix);
}

void MainWindow::on_chatButton_clicked()
{
    chatWindow->show();
    chatWindow->raise();
    chatWindow->activateWindow();
}
double MainWindow::getTotalIncome() const
{
    return ui->lcdIncome->value();
}

double MainWindow::getTotalExpenses() const
{
    return ui->lcdExpense->value();
}

double MainWindow::getTotalBalance() const
{
    return ui->lcdBalance->value();
}
