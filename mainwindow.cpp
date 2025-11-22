#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "connection.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QDebug>
#include <QFileDialog>
#include <QDateTime>
#include <QDir>
#include <QtPrintSupport/QPrinter>
#include <QPainter>
#include <QTextDocument>
#include <QPageLayout>
#include <QPageSize>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , selectedId(-1)
{
    ui->setupUi(this);

    refreshTable(Vtmp.afficher());
    connect(ui->tabWidget, &QTabWidget::currentChanged, this, &MainWindow::on_tabWidget_currentChanged);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::refreshTable(QSqlQueryModel* model)
{
    ui->tableWidget->setRowCount(0);
    ui->tableWidget->setColumnCount(5);

    QStringList headers;
    headers << "ID" << "Nom Véhicule" << "Immatricule" << "Kilométrage" << "État";
    ui->tableWidget->setHorizontalHeaderLabels(headers);

    for(int row = 0; row < model->rowCount(); row++)
    {
        ui->tableWidget->insertRow(row);
        for(int col = 0; col < 5; col++)
        {
            QTableWidgetItem* item = new QTableWidgetItem(model->data(model->index(row, col)).toString());
            ui->tableWidget->setItem(row, col, item);
        }
    }

    delete model;
}

void MainWindow::on_pushButton_clicked()
{
    QString marque = ui->lineEdit->text();
    QString matricule = ui->lineEdit_2->text();
    QString kmText = ui->lineEdit_4->text();
    QString etat = ui->comboBox->currentText();

    if (marque.isEmpty() || matricule.isEmpty() || kmText.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez remplir tous les champs!");
        return;
    }

    bool ok;
    int km = kmText.toInt(&ok);

    if (!ok || km < 0) {
        QMessageBox::warning(this, "Erreur", "Kilométrage invalide!");
        return;
    }

    QSqlQuery query;
    int id = 1;

    qDebug() << "=== GENERATION ID ===";
    if (query.exec("SELECT NVL(MAX(ID_U), 0) + 1 FROM VEHICULE")) {
        if (query.next()) {
            id = query.value(0).toInt();
            qDebug() << "ID généré:" << id;
        }
    } else {
        qDebug() << "Erreur génération ID:" << query.lastError().text();
        if (query.exec("SELECT COUNT(*) FROM VEHICULE")) {
            if (query.next()) {
                id = query.value(0).toInt() + 1;
                qDebug() << "ID alternatif:" << id;
            }
        }
    }

    qDebug() << "Création véhicule avec ID:" << id << "Matricule:" << matricule << "Marque:" << marque;

    Vehicule v(id, matricule, marque, km, etat);

    bool test = v.ajouter();

    if (test) {
        refreshTable(Vtmp.afficher());

        ui->lineEdit->clear();
        ui->lineEdit_2->clear();
        ui->lineEdit_4->clear();
        ui->comboBox->setCurrentIndex(0);

        QMessageBox::information(this, "Succès", "Véhicule ajouté avec succès!");
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de l'ajout! Vérifiez la console pour plus de détails.");
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    QString terme = ui->lineEdit_3->text();

    if (terme.isEmpty()) {
        refreshTable(Vtmp.afficher());
    } else {
        refreshTable(Vtmp.rechercher(terme));
    }
}

void MainWindow::on_pushButton_4_clicked()
{
    refreshTable(Vtmp.afficher());
    ui->lineEdit_3->clear();
}

void MainWindow::on_pushButton_10_clicked()
{
    if (selectedId == -1) {
        QMessageBox::warning(this, "Erreur", "Sélectionnez un véhicule dans le tableau!");
        return;
    }

    QString champ = ui->comboBox_2->currentText();
    QString nouvelleValeur = ui->lineEdit_5->text();

    if (nouvelleValeur.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Entrez une nouvelle valeur!");
        return;
    }

    QString champSQL;
    if (champ == "marque") {
        champSQL = "MARQUE";
    } else if (champ == "matricule") {
        champSQL = "MATRICULE";
    } else if (champ == "kilometrage") {
        champSQL = "KILOMETRAGE";

        bool ok;
        nouvelleValeur.toInt(&ok);
        if (!ok) {
            QMessageBox::warning(this, "Erreur", "Le kilométrage doit être un nombre!");
            return;
        }
    } else if (champ == "etat") {
        champSQL = "ETAT";
    }

    bool test = Vtmp.modifier(selectedId, champSQL, nouvelleValeur);

    if (test) {
        refreshTable(Vtmp.afficher());
        ui->lineEdit_5->clear();
        QMessageBox::information(this, "Succès", "Véhicule modifié!");
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de la modification");
    }
}

void MainWindow::on_pushButton_6_clicked()
{
    if (selectedId == -1) {
        QMessageBox::warning(this, "Erreur", "Sélectionnez un véhicule dans le tableau!");
        return;
    }

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirmation",
                                  "Voulez-vous vraiment supprimer ce véhicule?",
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        bool test = Vtmp.supprimer(selectedId);

        if (test) {
            refreshTable(Vtmp.afficher());
            selectedId = -1;
            QMessageBox::information(this, "Succès", "Véhicule supprimé avec succès!");
        } else {
            QMessageBox::critical(this, "Erreur", "Échec de la suppression!");
        }
    }
}

void MainWindow::on_pushButton_5_clicked()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirmation",
                                  "Voulez-vous vraiment supprimer TOUS les véhicules?",
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        QSqlQuery query;
        if (query.exec("DELETE FROM VEHICULE")) {
            refreshTable(Vtmp.afficher());
            QMessageBox::information(this, "Succès", "Tous les véhicules supprimés!");
        } else {
            QMessageBox::critical(this, "Erreur", "Échec de la suppression!");
        }
    }
}

void MainWindow::on_pushButton_8_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("Exporter en PDF"),
                                                    QDir::homePath() + "/vehicules_" +
                                                        QDateTime::currentDateTime().toString("yyyy-MM-dd_hh-mm") + ".pdf",
                                                    tr("PDF Files (*.pdf)"));

    if (fileName.isEmpty()) {
        return;
    }

    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(fileName);
    printer.setPageSize(QPageSize::A4);
    printer.setPageOrientation(QPageLayout::Portrait);

    QString htmlContent = "<html><head><style>"
                          "body { font-family: Arial, sans-serif; margin: 20px; }"
                          "h1 { color: #2c3e50; text-align: center; margin-bottom: 10px; }"
                          "table { width: 100%; border-collapse: collapse; margin-top: 20px; }"
                          "th { background-color: #3498db; color: white; padding: 10px; border: 1px solid #2980b9; }"
                          "td { padding: 8px; border: 1px solid #ddd; text-align: left; }"
                          "tr:nth-child(even) { background-color: #f2f2f2; }"
                          ".footer { margin-top: 30px; text-align: center; font-size: 10pt; color: #7f8c8d; }"
                          ".subtitle { text-align: center; color: #7f8c8d; margin-bottom: 20px; }"
                          "</style></head><body>";

    htmlContent += "<h1>LISTE DES VÉHICULES</h1>";
    htmlContent += "<p class='subtitle'>Généré le : " +
                   QDateTime::currentDateTime().toString("dd/MM/yyyy à hh:mm") + "</p>";

    htmlContent += "<table>";
    htmlContent += "<tr><th>ID</th><th>Nom Véhicule</th><th>Immatricule</th><th>Kilométrage</th><th>État</th></tr>";

    QSqlQueryModel* model = Vtmp.afficher();

    for (int row = 0; row < model->rowCount(); row++) {
        htmlContent += "<tr>";
        for (int col = 0; col < model->columnCount(); col++) {
            QString value = model->data(model->index(row, col)).toString();
            htmlContent += "<td>" + value + "</td>";
        }
        htmlContent += "</tr>";
    }

    htmlContent += "</table>";

    htmlContent += "<div class='footer'>";
    htmlContent += "<p><strong>Total de véhicules : " + QString::number(model->rowCount()) + "</strong></p>";
    htmlContent += "<p>Document généré automatiquement par le système de gestion des véhicules</p>";
    htmlContent += "</div>";

    htmlContent += "</body></html>";

    QTextDocument document;
    document.setHtml(htmlContent);
    document.print(&printer);

    delete model;

    QMessageBox::information(this, "Succès", "Le fichier PDF a été exporté avec succès!\n\nEmplacement : " + fileName);
}

void MainWindow::on_pushButton_9_clicked()
{
    QString critere = ui->comboBox_3->currentText();
    QString critereSQL;

    if (critere == "en kilometrage") {
        critereSQL = "KILOMETRAGE DESC";
    } else if (critere == "en etat") {
        critereSQL = "ETAT";
    }

    refreshTable(Vtmp.trier(critereSQL));
}

void MainWindow::on_tableWidget_clicked(const QModelIndex &index)
{
    selectedId = ui->tableWidget->item(index.row(), 0)->text().toInt();
    qDebug() << "ID sélectionné:" << selectedId;
}

void MainWindow::on_tabWidget_currentChanged(int index)
{
    if (index == 1) {
        updateStatistics();
    }
}

void MainWindow::updateStatistics()
{
    QSqlQuery query;

    int totalVehicles = 0;
    int goodCondition = 0;
    int brokenDown = 0;
    double avgKm = 0;

    query.exec("SELECT COUNT(*) FROM VEHICULE");
    if (query.next()) totalVehicles = query.value(0).toInt();

    query.exec("SELECT COUNT(*) FROM VEHICULE WHERE ETAT = 'bonne etat'");
    if (query.next()) goodCondition = query.value(0).toInt();

    query.exec("SELECT COUNT(*) FROM VEHICULE WHERE ETAT = 'en panne'");
    if (query.next()) brokenDown = query.value(0).toInt();

    query.exec("SELECT AVG(KILOMETRAGE) FROM VEHICULE");
    if (query.next()) avgKm = query.value(0).toDouble();


    QWidget* statsWidget = ui->tab_2->findChild<QWidget*>("statsContainer");
    if (statsWidget) {
        delete statsWidget;
    }


    QWidget* container = new QWidget(ui->tab_2);
    container->setObjectName("statsContainer");
    container->setGeometry(50, 80, 1050, 500);

    QVBoxLayout* mainLayout = new QVBoxLayout(container);
    mainLayout->setSpacing(20);


    QPieSeries *pieSeries = new QPieSeries();
    pieSeries->append("Bonne état", goodCondition);
    pieSeries->append("En panne", brokenDown);

    QPieSlice *goodSlice = pieSeries->slices().at(0);
    goodSlice->setBrush(QColor("#4CAF50"));
    goodSlice->setLabelVisible(true);
    goodSlice->setLabelColor(Qt::black);
    goodSlice->setLabelPosition(QPieSlice::LabelOutside);

    QPieSlice *brokenSlice = pieSeries->slices().at(1);
    brokenSlice->setBrush(QColor("#F44336"));
    brokenSlice->setLabelVisible(true);
    brokenSlice->setLabelColor(Qt::black);
    brokenSlice->setLabelPosition(QPieSlice::LabelOutside);

    QChart *pieChart = new QChart();
    pieChart->addSeries(pieSeries);
    pieChart->setTitle("📊 État des Véhicules");
    pieChart->setAnimationOptions(QChart::SeriesAnimations);
    pieChart->setTheme(QChart::ChartThemeLight);

    QFont titleFont;
    titleFont.setPointSize(14);
    titleFont.setBold(true);
    pieChart->setTitleFont(titleFont);

    QChartView *pieChartView = new QChartView(pieChart);
    pieChartView->setRenderHint(QPainter::Antialiasing);
    pieChartView->setMinimumHeight(250);


    QBarSet *set = new QBarSet("Statistiques");
    *set << totalVehicles << goodCondition << brokenDown << (avgKm / 1000); // Convert km to thousands for better visualization
    set->setColor(QColor("#009688"));

    QBarSeries *barSeries = new QBarSeries();
    barSeries->append(set);

    QChart *barChart = new QChart();
    barChart->addSeries(barSeries);
    barChart->setTitle("📈 Statistiques Générales");
    barChart->setAnimationOptions(QChart::SeriesAnimations);
    barChart->setTheme(QChart::ChartThemeLight);
    barChart->setTitleFont(titleFont);

    QStringList categories;
    categories << "Total" << "Bon état" << "En panne" << "Moy. KM (x1000)";

    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(categories);
    barChart->addAxis(axisX, Qt::AlignBottom);
    barSeries->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis();
    axisY->setRange(0, qMax(totalVehicles, (int)(avgKm / 1000)) + 5);
    barChart->addAxis(axisY, Qt::AlignLeft);
    barSeries->attachAxis(axisY);

    barChart->legend()->setVisible(false);

    QChartView *barChartView = new QChartView(barChart);
    barChartView->setRenderHint(QPainter::Antialiasing);
    barChartView->setMinimumHeight(250);


    mainLayout->addWidget(pieChartView);
    mainLayout->addWidget(barChartView);

    container->setLayout(mainLayout);
    container->show();


    QString html = R"(
        <style>
            .title { color:#1E88E5; font-size:18px; font-weight:700; margin-bottom:10px; }
            .info { font-size:14px; color:#455A64; line-height: 1.8; }
        </style>
        <div>
            <div class='title'>📊 Résumé des Statistiques</div>
            <div class='info'>
                <b>Total:</b> )" + QString::number(totalVehicles) + R"( véhicules<br>
                <b>En bon état:</b> )" + QString::number(goodCondition) + R"(<br>
                <b>En panne:</b> )" + QString::number(brokenDown) + R"(<br>
                <b>Kilométrage moyen:</b> )" + QString::number(avgKm, 'f', 0) + R"( km
            </div>
        </div>
    )";

    ui->label_stats_info->setText(html);
    ui->label_stats_info->setTextFormat(Qt::RichText);
    ui->label_stats_info->setGeometry(50, 20, 400, 150);
}
