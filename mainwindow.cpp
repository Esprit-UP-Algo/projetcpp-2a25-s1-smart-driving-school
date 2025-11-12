#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "moniteur.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QInputDialog>
#include <QTableWidgetItem>
#include <QMessageBox>
#include <QPdfWriter>
#include <QPainter>
#include <QFileDialog>
#include <QDateTime>
#include <QDebug>
#include <QVBoxLayout>
#include <QLayout>
#include <QRegularExpression>

// QtCharts includes - NO namespace
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QChart>

// REMOVE THIS LINE COMPLETELY - NO using namespace QtCharts;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// Utility: refresh table from DB
void remplirTable(QTableWidget *table)
{
    QSqlQuery query("SELECT * FROM MONITEUR ORDER BY TO_NUMBER(ID) ASC");
    table->setRowCount(0);
    int row = 0;

    while (query.next()) {
        table->insertRow(row);
        table->setItem(row, 0, new QTableWidgetItem(query.value("NOM").toString()));
        table->setItem(row, 1, new QTableWidgetItem(query.value("PRENOM").toString()));
        table->setItem(row, 2, new QTableWidgetItem(query.value("CIN").toString()));
        table->setItem(row, 3, new QTableWidgetItem(query.value("TEL").toString()));
        table->setItem(row, 4, new QTableWidgetItem(query.value("MAIL").toString()));
        table->setItem(row, 5, new QTableWidgetItem(query.value("VOITURE").toString()));
        table->setItem(row, 6, new QTableWidgetItem(query.value("ID").toString()));
        row++;
    }
}

// ➕ Ajouter moniteur
void MainWindow::on_pushButton_clicked()
{
    QString nom = ui->lineEdit_2->text();
    QString prenom = ui->lineEdit_4->text();
    QString cin = ui->lineEdit_9->text();
    QString tel = ui->lineEdit_10->text();
    QString mail = ui->lineEdit_11->text();
    QString voiture = ui->lineEdit_12->text();

    Moniteur m(nom, prenom, cin, tel, mail, voiture);

    if (!m.ajouter()) {
        QMessageBox::warning(this, "Erreur",
                             "⚠ Vérifiez vos informations :\n"
                             "- Nom/Prénom : lettres (min 2)\n"
                             "- CIN : 8 chiffres\n"
                             "- Tel : 8 chiffres\n"
                             "- Mail valide");
        return;
    }

    QMessageBox::information(this, "Succès", "Moniteur ajouté ✅");
    remplirTable(ui->tableWidget);
}

// 🔍 Rechercher par CIN avec contrôle de saisie
void MainWindow::on_pushButton_2_clicked()
{
    QString cin = ui->lineEdit_3->text().trimmed();

    // 🔒 Vérification : champ vide interdit
    if (cin.isEmpty()) {
        QMessageBox::warning(this, "Erreur de saisie", "Veuillez entrer un numéro CIN pour effectuer la recherche !");
        return;
    }

    // ⚠️ Vérifie que CIN contient uniquement des chiffres et fait 8 caractères
    QRegularExpression regex("^\\d{8}$");
    if (!regex.match(cin).hasMatch()) {
        QMessageBox::warning(this, "Erreur de saisie", "Le CIN doit contenir exactement 8 chiffres !");
        return;
    }

    // 🔍 Exécuter la recherche si valide
    QSqlQuery query;
    query.prepare("SELECT * FROM MONITEUR WHERE CIN LIKE :cin");
    query.bindValue(":cin", "%" + cin + "%");

    if (!query.exec()) {
        QMessageBox::critical(this, "Erreur", "Erreur lors de la recherche : " + query.lastError().text());
        return;
    }

    ui->tableWidget->setRowCount(0);
    int row = 0;
    while (query.next()) {
        ui->tableWidget->insertRow(row);
        ui->tableWidget->setItem(row, 0, new QTableWidgetItem(query.value("NOM").toString()));
        ui->tableWidget->setItem(row, 1, new QTableWidgetItem(query.value("PRENOM").toString()));
        ui->tableWidget->setItem(row, 2, new QTableWidgetItem(query.value("CIN").toString()));
        ui->tableWidget->setItem(row, 3, new QTableWidgetItem(query.value("TEL").toString()));
        ui->tableWidget->setItem(row, 4, new QTableWidgetItem(query.value("MAIL").toString()));
        ui->tableWidget->setItem(row, 5, new QTableWidgetItem(query.value("VOITURE").toString()));
        ui->tableWidget->setItem(row, 6, new QTableWidgetItem(query.value("ID").toString()));
        row++;
    }

    if (row == 0) {
        QMessageBox::information(this, "Résultat", "Aucun moniteur trouvé avec ce CIN.");
    }
}

// 📋 Afficher tous
void MainWindow::on_pushButton_3_clicked()
{
    remplirTable(ui->tableWidget);
}

// 🗑️ Tout Supprimer
void MainWindow::on_pushButton_5_clicked()
{
    QMessageBox::StandardButton reply =
        QMessageBox::warning(this, "Confirmation",
                             "⚠ Ceci va supprimer TOUS les moniteurs\nContinuer ?",
                             QMessageBox::Yes | QMessageBox::Cancel);

    if (reply == QMessageBox::Cancel) {
        QMessageBox::information(this, "Annulé", "Aucune suppression effectuée.");
        return;
    }

    QSqlQuery query;
    if (!query.exec("DELETE FROM MONITEUR")) {
        QMessageBox::warning(this, "Erreur", "Échec de suppression :\n" + query.lastError().text());
        return;
    }

    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);

    QMessageBox::information(this, "Succès", "Tous les moniteurs ont été supprimés ✅");
}

// 🗑️ Supprimer moniteur avec vérification d'ID
void MainWindow::on_pushButton_6_clicked()
{
    int id = QInputDialog::getInt(this, "Supprimer", "Entrer ID à supprimer :");
    if (id <= 0) return;

    // 🔍 Vérifie si l'ID existe avant suppression
    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT COUNT(*) FROM MONITEUR WHERE ID = :id");
    checkQuery.bindValue(":id", id);
    checkQuery.exec();
    checkQuery.next();
    int count = checkQuery.value(0).toInt();

    if (count == 0) {
        QMessageBox::warning(this, "Erreur", "Aucun moniteur trouvé avec cet ID !");
        return;
    }

    // 🔒 Confirmation
    QMessageBox::StandardButton reply =
        QMessageBox::question(this, "Confirmation",
                              "Supprimer le moniteur avec l'ID " + QString::number(id) + " ?",
                              QMessageBox::Yes | QMessageBox::Cancel);

    if (reply == QMessageBox::Cancel) {
        QMessageBox::information(this, "Annulé", "Suppression annulée.");
        return;
    }

    // ✅ Supprimer si confirmé
    Moniteur m;
    if (m.supprimer(id))
        QMessageBox::information(this, "Succès", "Moniteur supprimé ✅");
    else
        QMessageBox::critical(this, "Erreur", "Suppression échouée ❌");

    remplirTable(ui->tableWidget);
}

// ✏️ Modifier moniteur
void MainWindow::on_pushButton_10_clicked()
{
    int id = QInputDialog::getInt(this, "Modifier", "Entrer ID à modifier :");
    if (id <= 0) return;

    // Vérifie si l'ID existe
    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT COUNT(*) FROM MONITEUR WHERE ID = :id");
    checkQuery.bindValue(":id", id);
    if (!checkQuery.exec() || !checkQuery.next()) {
        QMessageBox::warning(this, "Erreur", "Impossible de vérifier l'ID (erreur base de données).");
        return;
    }

    int count = checkQuery.value(0).toInt();
    if (count == 0) {
        QMessageBox::warning(this, "Erreur", "Aucun moniteur trouvé avec cet ID !");
        return;
    }

    // Confirmation
    QMessageBox::StandardButton reply =
        QMessageBox::question(this, "Confirmation",
                              "Modifier le moniteur avec l'ID " + QString::number(id) + " ?",
                              QMessageBox::Yes | QMessageBox::Cancel);

    if (reply == QMessageBox::Cancel) {
        QMessageBox::information(this, "Annulé", "Modification annulée.");
        return;
    }

    QString champ = ui->comboBox_2->currentText();
    QString valeur = ui->lineEdit_5->text().trimmed();

    // ⚠️ Nouvelle vérification : champ vide interdit
    if (valeur.isEmpty()) {
        QMessageBox::warning(this, "Erreur de saisie", "Le champ de modification ne peut pas être vide !");
        return;
    }

    if (champ == "Nom") champ = "NOM";
    else if (champ == "Prénom") champ = "PRENOM";
    else if (champ == "CIN") champ = "CIN";
    else if (champ == "téléphone") champ = "TEL";
    else if (champ == "Mail") champ = "MAIL";
    else if (champ.contains("voiture", Qt::CaseInsensitive)) champ = "VOITURE";

    Moniteur m;
    bool ok = m.modifier(id, champ, valeur);

    if (ok) {
        QMessageBox::information(this, "Succès", "Moniteur modifié ✅");
        remplirTable(ui->tableWidget);
    }
}

// 🔄 Trier
void MainWindow::on_pushButton_9_clicked()
{
    QString typeTri = ui->comboBox_3->currentText();
    QString queryText;

    if (typeTri == "Alphabet")
        queryText = "SELECT * FROM MONITEUR ORDER BY NOM ASC";
    else
        queryText = "SELECT * FROM MONITEUR ORDER BY TO_NUMBER(ID) ASC";

    QSqlQuery query;
    if (!query.exec(queryText)) {
        QMessageBox::warning(this, "Erreur", "Échec de tri :\n" + query.lastError().text());
        return;
    }

    ui->tableWidget->setRowCount(0);
    int row = 0;

    while (query.next()) {
        ui->tableWidget->insertRow(row);
        ui->tableWidget->setItem(row, 0, new QTableWidgetItem(query.value("NOM").toString()));
        ui->tableWidget->setItem(row, 1, new QTableWidgetItem(query.value("PRENOM").toString()));
        ui->tableWidget->setItem(row, 2, new QTableWidgetItem(query.value("CIN").toString()));
        ui->tableWidget->setItem(row, 3, new QTableWidgetItem(query.value("TEL").toString()));
        ui->tableWidget->setItem(row, 4, new QTableWidgetItem(query.value("MAIL").toString()));
        ui->tableWidget->setItem(row, 5, new QTableWidgetItem(query.value("VOITURE").toString()));
        ui->tableWidget->setItem(row, 6, new QTableWidgetItem(query.value("ID").toString()));
        row++;
    }

    QMessageBox::information(this, "Tri effectué", "Tri réussi ✅");
}

void MainWindow::on_pushButton_4_clicked()
{
    QString filePath = QFileDialog::getSaveFileName(
        this, "Exporter en PDF",
        QDir::homePath() + "/Moniteurs_" + QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss") + ".pdf",
        "PDF Files (*.pdf)"
        );

    if (filePath.isEmpty())
        return;

    QPdfWriter pdf(filePath);
    pdf.setPageSize(QPageSize(QPageSize::A4));
    pdf.setResolution(300);
    QPainter painter(&pdf);

    int y = 200;
    painter.setFont(QFont("Arial", 14, QFont::Bold));
    painter.drawText(100, 100, "Liste des Moniteurs");
    painter.setFont(QFont("Arial", 10));

    QSqlQuery query;
    if (!query.exec("SELECT * FROM MONITEUR ORDER BY ID ASC")) {
        QMessageBox::critical(this, "Erreur SQL", "Échec de lecture des données :\n" + query.lastError().text());
        return;
    }

    // Header
    painter.drawText(100, y, "ID");
    painter.drawText(200, y, "Nom");
    painter.drawText(400, y, "Prénom");
    painter.drawText(600, y, "CIN");
    painter.drawText(800, y, "TEL");
    painter.drawText(1000, y, "MAIL");
    painter.drawText(1400, y, "VOITURE");
    y += 40;

    // Rows
    while (query.next()) {
        painter.drawText(100, y, query.value("ID").toString());
        painter.drawText(200, y, query.value("NOM").toString());
        painter.drawText(400, y, query.value("PRENOM").toString());
        painter.drawText(600, y, query.value("CIN").toString());
        painter.drawText(800, y, query.value("TEL").toString());
        painter.drawText(1000, y, query.value("MAIL").toString());
        painter.drawText(1400, y, query.value("VOITURE").toString());
        y += 30;
    }

    painter.end();
    QMessageBox::information(this, "Succès", "PDF exporté avec succès !");
}

void MainWindow::afficherStatistiques()
{
    // Clear any pending operations on the default connection
    QSqlDatabase::database().commit();

    QSqlQuery query;
    query.finish(); // Finish any previous query

    if (!query.exec("SELECT VOITURE, COUNT(*) AS NOMBRE FROM MONITEUR GROUP BY VOITURE")) {
        ui->label_stats_info->setText("❌ Error loading data: " + query.lastError().text());
        return;
    }

    QPieSeries *series = new QPieSeries();
    while (query.next()) {
        QString voiture = query.value("VOITURE").toString();
        int nombre = query.value("NOMBRE").toInt();
        series->append(voiture + " (" + QString::number(nombre) + ")", nombre);
    }

    if (series->isEmpty()) {
        ui->label_stats_info->setText("No data available to display.");
        return;
    }

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("🚗 Number of Moniteurs per Car Brand");
    chart->setAnimationOptions(QChart::AllAnimations);

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    // Clear existing layout and set new one
    QLayout *oldLayout = ui->label_stats_info->layout();
    if (oldLayout) {
        QLayoutItem *item;
        while ((item = oldLayout->takeAt(0)) != nullptr) {
            if (item->widget()) {
                item->widget()->deleteLater();
            }
            delete item;
        }
        delete oldLayout;
    }

    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(chartView);
    ui->label_stats_info->setLayout(layout);
}

void MainWindow::on_tabWidget_currentChanged(int index)
{
    if (index == 1) // assuming "Statistiques" is tab 1
        afficherStatistiques();
}
