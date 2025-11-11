#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>


#include <QSqlQueryModel>
#include <QtCharts/QChartView>

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
    void on_addButton_clicked();
    void on_deleteButton_clicked();
    void on_btnSearch_clicked();
    void on_exportButton_2_clicked();
    void on_tabWidget_currentChanged(int index);
    void on_UpdateButton_clicked();
    void onTableCurrentChanged(const QModelIndex &current, const QModelIndex &previous);

    void on_TRIE_activated(int index);
private:
    Ui::MainWindow *ui;
    QSqlQueryModel *modelTransactions = nullptr;

    QSqlQueryModel *modelStats = nullptr;
    QChartView *chartView = nullptr;

    void loadStatistics();
    void loadTableData();
    void detachModel();
    void clearForm();
};

#endif // MAINWINDOW_H
