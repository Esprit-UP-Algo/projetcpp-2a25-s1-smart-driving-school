#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QMessageBox>
#include <QDate>
#include <QHeaderView>
#include <QAbstractItemModel>

#include "exam.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

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

void MainWindow::on_exportButton_2_clicked()
{
    loadTableData();
    QMessageBox::information(this, "Info", "Données rechargées");
}

void MainWindow::on_tabWidget_currentChanged(int index)
{
    Q_UNUSED(index);

}
