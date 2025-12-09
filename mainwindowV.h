#ifndef MAINWINDOWV_H
#define MAINWINDOWV_H

#include <QMainWindow>
#include <QTableWidgetItem>
#include <QTimer>
#include "vehicule.h"
#include "emailsender.h"
#include "voicerecognition.h"
#include "arduinovh.h"

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
    void on_pushButton_email_clicked();
    void on_pushButton_microphone_clicked();
    void on_tableWidget_clicked(const QModelIndex &index);

    // Voice recognition slots
    void onVoiceTextRecognized(const QString &text);
    void onVoiceError(const QString &error);
    void onRecordingStarted();
    void onRecordingStopped();

    // Arduino slots - NO BUTTON, just automatic detection
    void onArduinoCardDetected(const QString &cardCode);
    void onArduinoCardScanned(const QString &cardCode, bool isNew);
    void onArduinoError(const QString &error);

private:
    Ui::MainWindow *ui;
    Vehicule Vtmp;
    int selectedId;
    EmailSender *emailSender;
    VoiceRecognition *voiceRecognition;
    ArduinoVH *arduinoManager;

    void updateStatistics();
    void refreshTable(QSqlQueryModel* model);

    // Email methods
    QString getAvailableVehiclesText();
    QStringList getInstructorsList();
    QString selectInstructor();
};

#endif // MAINWINDOWV_H
