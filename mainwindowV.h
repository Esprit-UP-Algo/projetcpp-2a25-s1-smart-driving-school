#ifndef MAINWINDOWV_H
#define MAINWINDOWV_H

#include <QMainWindow>
#include <QTableWidgetItem>
#include "vehicule.h"
#include "emailsender.h"
#include "voicerecognition.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;  // Keep as MainWindow (matches the UI file)
}
QT_END_NAMESPACE

class MainWindowV : public QMainWindow  // Changed class name
{
    Q_OBJECT

public:
    MainWindowV(QWidget *parent = nullptr);
    ~MainWindowV();

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
    void on_pushButton_email_clicked();
    void on_pushButton_microphone_clicked();
    void on_tableWidget_clicked(const QModelIndex &index);

    // Slots pour la reconnaissance vocale
    void onVoiceTextRecognized(const QString &text);
    void onVoiceError(const QString &error);
    void onRecordingStarted();
    void onRecordingStopped();

private:
    Ui::MainWindow *ui;  // Keep as MainWindow (matches the UI file)
    Vehicule Vtmp;
    int selectedId;
    EmailSender *emailSender;
    VoiceRecognition *voiceRecognition;

    void updateStatistics();
    void refreshTable(QSqlQueryModel* model);

    // Méthodes email
    QString getAvailableVehiclesText();
    QStringList getInstructorsList();
    QString selectInstructor();
};

#endif // MAINWINDOWV_H
