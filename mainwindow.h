#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidgetItem>
#include "vehicule.h"
#include "emailsender.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_tabWidget_currentChanged(int index);
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_4_clicked();
    void on_pushButton_5_clicked();
    void on_pushButton_6_clicked();
    void on_pushButton_8_clicked();
    void on_pushButton_9_clicked();
    void on_pushButton_10_clicked();
    void on_pushButton_email_clicked();  // NOUVEAU: Bouton email
    void on_tableWidget_clicked(const QModelIndex &index);

private:
    Ui::MainWindow *ui;
    Vehicule Vtmp;
    int selectedId;
    EmailSender *emailSender;  // NOUVEAU: Gestionnaire d'emails

    void updateStatistics();
    void refreshTable(QSqlQueryModel* model);

    // NOUVEAU: Méthodes email
    QString getAvailableVehiclesText();
    QStringList getInstructorsList();
    QString selectInstructor();
};

#endif // MAINWINDOW_H
