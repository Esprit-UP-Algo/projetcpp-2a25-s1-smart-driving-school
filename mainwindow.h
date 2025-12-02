#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QCalendarWidget>
#include <QListWidget>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QTextCharFormat>
#include "role.h"
#include <QMainWindow>
#include "statschartwidget.h"

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
    void on_addButton_clicked();
    void on_deleteButton_clicked();
    void on_btnSearch_clicked();
    void on_modifyButton_clicked();
    void on_triButton_clicked();
    void on_exportButton_clicked();
    void on_tabWidget_currentChanged(int index);
    void on_comboTypeStats_currentIndexChanged(int);

    // --- Calendrier ---
    void refreshCalendarMarks();
    void on_calendar_selectionChanged();
    void on_btnAddPlan_clicked();
    void on_btnDelPlan_clicked();
    void on_btnRefreshPlan_clicked();
private:
    void applyRole();

    Ui::MainWindow *ui= nullptr;
    QSqlQueryModel* modelExams = nullptr;
    void loadTableData();
    void clearForm();
    void detachModel();
    Role role_ = Role::Admin;
    void loadStatsTable();
    StatsChartWidget* statsChartView = nullptr;

    void checkPlanningAccess();
    void loadDayList(const QDate& d);
    void clearCalendarMarks();
};


#endif // MAINWINDOW_H
