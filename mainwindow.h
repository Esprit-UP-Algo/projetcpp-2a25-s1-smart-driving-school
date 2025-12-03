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

#include <QProcess>
#include <QMessageBox>
#include <QCoreApplication>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

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
    Ui::MainWindow *ui;
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
};

#endif // MAINWINDOW_H
