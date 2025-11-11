#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QSqlQueryModel>

#include <QMainWindow>

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
    void on_addButton_clicked();
    void on_deleteButton_clicked();
    void on_btnSearch_clicked();
    void on_modifyButton_clicked();
    void on_triButton_clicked();
    void on_exportButton_2_clicked();
    void on_tabWidget_currentChanged(int index);

private:
    Ui::MainWindow *ui;
    QSqlQueryModel* modelExams = nullptr;
    void loadTableData();
    void clearForm();
    void detachModel();
};

#endif // MAINWINDOW_H
