#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QCalendarWidget>
#include <QListWidget>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QTextCharFormat>
#include <QRegularExpression>
#include <QTableWidgetItem>
#include "role.h"
#include <QMainWindow>
#include "statschartwidget.h"
#include "voicerecognition.h"  // ✅ ADD THIS

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void setRole(Role r);

private slots:
    // Exam slots
    void on_addButton_clicked();
    void on_deleteButton_clicked();
    void on_btnSearch_clicked();
    void on_modifyButton_clicked();
    void on_triButton_clicked();
    void on_exportButton_clicked();
    void on_tabWidget_currentChanged(int index);
    void on_comboTypeStats_currentIndexChanged(int);

    // Vehicle management slots
    void on_pushButton_clicked();      // Add vehicle
    void on_pushButton_2_clicked();    // Search vehicle
    void on_pushButton_4_clicked();    // Show all vehicles
    void on_pushButton_5_clicked();    // Delete all vehicles
    void on_pushButton_6_clicked();    // Delete selected vehicle
    void on_pushButton_9_clicked();    // Sort vehicles
    void on_pushButton_10_clicked();   // Modify vehicle
    void on_pushButton_microphone_clicked();  // Voice input
    void on_pushButton_8_clicked();           // Export vehicles to PDF
    void on_pushButton_email_clicked();       // Email notification

    // Calendar slots
    void refreshCalendarMarks();
    void on_calendar_selectionChanged();
    void on_btnAddPlan_clicked();
    void on_btnDelPlan_clicked();
    void on_btnRefreshPlan_clicked();

    // ✅ ADD THESE - Voice recognition slots
    void onVoiceTextRecognized(const QString &text);
    void onVoiceError(const QString &error);
    void onRecordingStarted();
    void onRecordingStopped();

private:
    void applyRole();

    Ui::MainWindow *ui = nullptr;

    // Exam model
    QSqlQueryModel* modelExams = nullptr;
    void loadTableData();
    void clearForm();
    void detachModel();

    // Vehicle management
    void loadVehicleData();
    void clearVehicleForm();

    // ✅ ADD THIS - Voice recognition
    VoiceRecognition *voiceRecognition = nullptr;

    Role role_ = Role::Admin;
    void loadStatsTable();
    StatsChartWidget* statsChartView = nullptr;

    void checkPlanningAccess();
    void loadDayList(const QDate& d);
    void clearCalendarMarks();

    // PDF export helper functions
    void exportBrokenVehiclesPDF(const QString& filePath);
    void exportAllVehiclesPDF(const QString& filePath);
};

#endif // MAINWINDOW_H
