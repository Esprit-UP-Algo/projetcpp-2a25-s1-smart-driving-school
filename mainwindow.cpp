#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "connection.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , selectedId(-1)
{
    ui->setupUi(this);


    ui->tableWidget->setModel(Vtmp.afficher());
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{

    QString nom = ui->lineEdit->text();
    QString immat = ui->lineEdit_2->text();
    QString kmText = ui->lineEdit_4->text();
    QString etat = ui->comboBox->currentText();


    if (nom.isEmpty() || immat.isEmpty() || kmText.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez remplir tous les champs!");
        return;
    }


    int km = kmText.toInt();

    if (km < 0) {
        QMessageBox::warning(this, "Erreur", "Kilométrage invalide!");
        return;
    }


    QSqlQuery query;
    query.exec("SELECT NVL(MAX(ID), 0) + 1 FROM VEHICULES");
    int id = 1;
    if (query.next()) {
        id = query.value(0).toInt();
    }


    Vehicule v(id, nom, immat, km, etat);


    bool test = v.ajouter();

    if (test) {

        ui->tableWidget->setModel(Vtmp.afficher());


        ui->lineEdit->clear();
        ui->lineEdit_2->clear();
        ui->lineEdit_4->clear();
        ui->comboBox->setCurrentIndex(0);

        QMessageBox::information(this, "Succès", "Véhicule ajouté avec succès! ✅");
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de l'ajout!");
    }
}


void MainWindow::on_pushButton_2_clicked()
{
    QString terme = ui->lineEdit_3->text();

    if (terme.isEmpty()) {

        ui->tableWidget->setModel(Vtmp.afficher());
    } else {

        ui->tableWidget->setModel(Vtmp.rechercher(terme));
    }
}


void MainWindow::on_pushButton_4_clicked()
{

    ui->tableWidget->setModel(Vtmp.afficher());
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
    if (champ == "nom voiture") {
        champSQL = "NOM";
    } else if (champ == "immatricule") {
        champSQL = "IMMATRICULE";
    } else if (champ == "kilometrage") {
        champSQL = "KILOMETRAGE";
        // Vérifier que c'est un nombre
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

        ui->tableWidget->setModel(Vtmp.afficher());
        ui->lineEdit_5->clear();
        QMessageBox::information(this, "Succès", "Véhicule modifier️");
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

            ui->tableWidget->setModel(Vtmp.afficher());
            selectedId = -1;
            QMessageBox::information(this, "Succès", "Véhicule supprimé avec succès! 🗑️");
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
        if (query.exec("DELETE FROM VEHICULES")) {

            ui->tableWidget->setModel(Vtmp.afficher());
            QMessageBox::information(this, "Succès", "Tous les véhicules supprimés!");
        } else {
            QMessageBox::critical(this, "Erreur", "Échec de la suppression!");
        }
    }
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


    ui->tableWidget->setModel(Vtmp.trier(critereSQL));
}


void MainWindow::on_tableWidget_clicked(const QModelIndex &index)
{
    selectedId = ui->tableWidget->model()->data(ui->tableWidget->model()->index(index.row(), 0)).toInt();
    qDebug() << "ID sélectionné:" << selectedId;
}
