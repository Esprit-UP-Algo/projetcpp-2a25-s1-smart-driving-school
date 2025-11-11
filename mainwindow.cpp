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

#include <QtCharts/QChartView>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarSeries>
#include <QtCharts/QChart>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
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
    // columns in your SELECT: 0 ID, 1 Type, 2 Date(dd/MM/yyyy), 3 Amount, 4 Category, 5 Methode
    ui->TYPE->setCurrentText(m->index(row, 1).data().toString());
    ui->dateEdit->setDate(QDate::fromString(m->index(row, 2).data().toString(), "dd/MM/yyyy"));
    ui->MONTANT->setText(m->index(row, 3).data().toString());
    ui->CATEGORY->setCurrentText(m->index(row, 4).data().toString());
    ui->METHOD->setCurrentText(m->index(row, 5).data().toString());
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
        loadStatistics();
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
        loadStatistics();
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
