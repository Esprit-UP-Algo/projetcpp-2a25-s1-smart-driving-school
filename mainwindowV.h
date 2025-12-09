#ifndef MAINWINDOWV_H
#define MAINWINDOWV_H

#include <QMainWindow>
#include <QTableWidgetItem>
#include <QTimer>
#include <QSqlQueryModel>
#include "vehicule.h"
#include "emailsender.h"
#include "voicerecognition.h"
#include "arduinoVH.h"
#include "role.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindowV;
}
QT_END_NAMESPACE

class MainWindowV : public QMainWindow
{
    Q_OBJECT

public:
    MainWindowV(QWidget *parent = nullptr);
    ~MainWindowV();

    void setRole(Role r) { role_ = r; }

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

    // Voice recognition slots
    void onVoiceTextRecognized(const QString &text);
    void onVoiceError(const QString &error);
    void onRecordingStarted();
    void onRecordingStopped();

    // Arduino slots
    void onArduinoCardDetected(const QString &cardCode);
    void onArduinoCardScanned(const QString &cardCode, bool exists);
    void onArduinoError(const QString &error);

private:
    Ui::MainWindowV *ui;
    Vehicule Vtmp;
    int selectedId;
    Role role_;
    EmailSender *emailSender;
    VoiceRecognition *voiceRecognition;
    ArduinoVH *arduinoManager;

    void updateStatistics();
    void refreshTable(QSqlQueryModel* model);

    // Email helper methods
    QString getAvailableVehiclesText();
    QStringList getInstructorsList();
    QString selectInstructor();
};

#endif // MAINWINDOWV_H
