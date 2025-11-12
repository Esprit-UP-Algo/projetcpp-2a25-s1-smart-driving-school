#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include "moniteur.h"

// QtCharts includes - NO namespace
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
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();   // ➕ Add
    void on_pushButton_2_clicked(); // 🔍 Search
    void on_pushButton_4_clicked(); // 📄 Export PDF
    void on_pushButton_5_clicked(); // 🗑️ Delete all
    void on_pushButton_9_clicked(); // 🔠 Sort
    void on_pushButton_3_clicked(); // 📋 Show all
    void on_pushButton_6_clicked(); // ❌ Delete one
    void on_pushButton_10_clicked();// ✏️ Modify

    void afficherStatistiques();    // 📊 Show statistics
    void on_tabWidget_currentChanged(int index);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
