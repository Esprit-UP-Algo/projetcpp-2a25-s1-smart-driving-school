#ifndef MAINWINDOWK_H
#define MAINWINDOWK_H

<<<<<<< HEAD
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
=======
#include <QMainWindow>

#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QSqlQueryModel>

#include "chat.h"
#include "qr/qrcodegen.hpp"

#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QChart>

#include <QProcess>
#include <QMessageBox>
#include <QCoreApplication>
>>>>>>> origin/integrate-condidat

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindowk; }
QT_END_NAMESPACE

class MainWindowk : public QMainWindow
{
    Q_OBJECT

public:
<<<<<<< HEAD
    explicit MainWindowk(QWidget *parent = nullptr);
    ~MainWindowk();
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
    void on_pushButton_10_clicked();
    void on_pushButton_microphone_clicked();  // Voice input
    void on_pushButton_8_clicked();           // Export vehicles to PDF
    void on_pushButton_email_clicked();    // Modify vehicle

    // Calendar slots
    void refreshCalendarMarks();
    void on_calendar_selectionChanged();
    void on_btnAddPlan_clicked();
    void on_btnDelPlan_clicked();
    void on_btnRefreshPlan_clicked();

private:
    void applyRole();

    Ui::MainWindowk *ui = nullptr;

    // Exam model
    QSqlQueryModel* modelExams = nullptr;
    void loadTableData();
    void clearForm();
    void detachModel();

    // Vehicle management
    void loadVehicleData();
    void clearVehicleForm();

    Role role_ = Role::Admin;
    void loadStatsTable();
    StatsChartWidget* statsChartView = nullptr;

    void checkPlanningAccess();
    void loadDayList(const QDate& d);
    void clearCalendarMarks();
=======
    MainWindowk(QWidget *parent = nullptr);
    ~MainWindowk();

    QString getClientInfoById(int id) const;
    QString getTransactionInfoById(int id) const;

    double getTotalIncome() const;
    double getTotalExpenses() const;
    double getTotalBalance() const;

private slots:
    void on_addButton_clicked();
    void on_deleteButton_clicked();
    void on_btnSearch_clicked();
    void on_exportButton_2_clicked();
    void on_tabWidget_currentChanged(int index);
    void on_UpdateButton_clicked();
    void onTableCurrentChanged(const QModelIndex &current, const QModelIndex &previous);
    void on_exportButton_clicked();
    void on_TRIE_activated(int index);
    void on_btnGenerateQr_clicked();
    void on_chatButton_clicked();
    void on_btnCandidat_clicked();
    void on_tableViewTRANS_clicked(const QModelIndex &index);
    void on_btnLinkCin_clicked();
    void on_btnUnlinkCin_clicked();
    void on_btnRefreshCondidatTRANS_clicked();
    void on_btnSearchCondidatTRANS_clicked();
    void on_comboSortCondidatTRANS_activated(int index);
    void on_tableViewCondidatTRANS_clicked(const QModelIndex &index);
    void on_btnExportCondidatPDF_clicked();
    void on_btnGenerateCondidatQR_clicked();
    void on_comboCondidatCin_currentIndexChanged(int index);
    void on_btnExamens_clicked();


private:
    Ui::MainWindowk *ui;
    QSqlQueryModel *modelTransactions = nullptr;

    chat *chatWindow;
    QSqlQueryModel *modelStats = nullptr;
    QChartView *chartView = nullptr;          // statistics chart
    QImage generateQrImage(const QString &text, int pixelsPerModule = 8);
    QString buildDashboardText() const;
    QString currentCondidatCin;   // CIN selected in the Condidat/transaction tab

    QSqlQueryModel *modelStatsCondidat = nullptr;
    QChartView     *chartViewCondidat  = nullptr;


    void loadCondidatStatistics(const QString &cin);

    void loadStatistics();
    void loadTableData();
    void detachModel();
    void clearForm();
    void initYearFilter();
    void updateDashboard(int year = 0);   // 0 = all years
    int currentTransactionId = -1;
    void loadCondidatCombo();
    void loadCondidatTransactions();


    QChartView *chartViewBalance = nullptr;   // NEW: dashboard chart
>>>>>>> origin/integrate-condidat
};

#endif // MainWindowkK_H
