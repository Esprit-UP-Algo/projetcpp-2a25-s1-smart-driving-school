#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QSqlQueryModel>

#include "chat.h"
#include "qr/qrcodegen.hpp"

#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QChart>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

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

private:
    Ui::MainWindow *ui;
    QSqlQueryModel *modelTransactions = nullptr;

    chat *chatWindow;
    QSqlQueryModel *modelStats = nullptr;
    QChartView *chartView = nullptr;          // statistics chart
    QImage generateQrImage(const QString &text, int pixelsPerModule = 8);
    QString buildDashboardText() const;

    void loadStatistics();
    void loadTableData();
    void detachModel();
    void clearForm();
    void updateDashboard();

    QChartView *chartViewBalance = nullptr;   // NEW: dashboard chart
};

#endif // MAINWINDOW_H
