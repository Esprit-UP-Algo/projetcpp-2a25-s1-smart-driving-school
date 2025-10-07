#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "arduino.h"
#include <QTime>
#include<QtSql/QSqlDatabase>
#include<string>
#include <QFile>
#include<QDate>
#include<QDebug>
#include<QtSql/QSqlError>
#include<QMessageBox>
#include<QSqlQueryModel>
#include<QtSql/QSqlQuery>
#include<list>
#include<transaction.h>
#include <QtPrintSupport/QPrinter>
#include <QListWidget>
#include <QVBoxLayout>
#include <QDialog>
#include <QPainter>
#include <QFileDialog>
#include <QTextStream>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QLineSeries>
#include <QtCharts/QLegend>
#include <QtCharts/QBarCategoryAxis>
#include <QMenu>
#include <QtCharts/QValueAxis>
#include<QWidgetAction>
#include <QCalendarWidget>
#include<xlsxdocument.h>
#include<xlsxformat.h>
#include<QtCharts/QPieSeries >
#include<QVector>
#include<statistics.h>
#include <QtCharts/QChart>
#include<QWindow>
#include <QVBoxLayout>

#include <QSystemTrayIcon>
#include <QApplication>
#include <QIcon>
#include<QAudioSource>
#include<QNetworkAccessManager>
#include<QNetworkReply>
#include<QAudioInput>
#include<QNetworkRequest>
#include<QBuffer>
#include<QAudioDevice>
#include<QMediaDevices>
#include<QThread>
#include<QAudioFormat>
#include<QJsonArray>
#include<QJsonDocument>
#include<QMediaFormat>
#include<QJsonObject>
#include <QMainWindow>
#include <QTimer>

//#include <QSound>



#include<iostream>
Arduino* arduino = nullptr;
using namespace QXlsx;

using namespace std;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
     // I
    arduino = new Arduino();  // Initialize Arduino object
        int ret = arduino->connect_arduino();  // Connect to Arduino
        switch (ret) {
            case 0:
                qDebug() << "Arduino is available and connected to: " << arduino->getarduino_port_name();
                break;
            case 1:
                qDebug() << "Arduino is available but not connected to: " << arduino->getarduino_port_name();
                break;
            case -1:
                qDebug() << "Arduino is not available";
        }

        // Connect the serial read signal to the slot that updates the label
        QObject::connect(arduino->getserial(), SIGNAL(readyRead()), this, SLOT(update_label()));
    setFixedSize(1520, 1080);
    ui->date_Search->setPlaceholderText("                 Search Date");

    ui->todoliste->setVisible(false);

       // Connect the button click to the function
       connect(ui->pushButton_3, &QPushButton::clicked, this, &MainWindow::on_pushButton_3_clicked);



    QVector<Transaction> transactions = getTransactions();
    QList<int> *listOfYears = getTransactionYears(transactions);


    for (int year : *listOfYears) {
        ui->cmbTransactionYears->addItem(QString::number(year));
    }


    connect(ui->cmbTransactionYears, &QComboBox::currentTextChanged, this, &MainWindow::updateChart);


    updateChart();
    if (!QSystemTrayIcon::isSystemTrayAvailable()) {
            QMessageBox::critical(this, "Error", "System tray is not available on this system.");
            return;
        }



    networkManager=new QNetworkAccessManager(this);
    connect(ui->start_listening,&QPushButton::clicked,this,&MainWindow::on_start_listening_clicked);
    connect(networkManager,&QNetworkAccessManager::finished,this,&MainWindow::onSpeechReconnitionFinished);

}





void MainWindow::on_manageLighting_clicked()
{

       QByteArray dataToSend = "TurnOnLED";
       A->write_to_arduino(dataToSend);


       ui->lightingStatusLabel->setText("Sending command: TurnOnLED");
}

void MainWindow::on_manageLighting_2_clicked()
{

    isDetecting = false;
    ui->statusLabel->setText("Stopped detecting light status.");
}

void MainWindow::update_label()
{
    QByteArray dataReceived = A->read_from_arduino();
        QString dataString = QString(dataReceived);


        ui->lightingStatusLabel->setText("Arduino says: " + dataString);


        if (dataString == "LIGHT_ON") {
            ui->lightingStatusLabel->setText("Lights are turned ON.");
        } else if (dataString == "LIGHT_OFF") {
            ui->lightingStatusLabel->setText("Lights are turned OFF.");
        }
}
void MainWindow::updateChart()
{

    if (QLayout *oldLayout = ui->chartZone->layout()) {
        QLayoutItem *item;
        while ((item = oldLayout->takeAt(0)) != nullptr) {
            delete item->widget();
            delete item;
        }
        delete oldLayout;
    }


    int selectedYear = ui->cmbTransactionYears->currentText().toInt();


    Statistics stat = fillStatestics(getTransactions(), selectedYear);


    QBarSet *incomeSet = new QBarSet("Income");
    QBarSet *expenseSet = new QBarSet("Expense");
    QBarSet *profitLossSet = new QBarSet("Profit/Loss");


    QStringList monthsWithData;


    QStringList allMonths = {"Jan", "Feb", "Mar", "Apr", "May", "Jun",
                             "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
    for (int i = 0; i < 12; ++i) {
        if (stat.incomes[i] != 0 || stat.expenses[i] != 0) {
            // Add the data to the chart only for months with data
            *incomeSet << stat.incomes[i];
            *expenseSet << stat.expenses[i];
            *profitLossSet << stat.profitLoss[i];
            monthsWithData << allMonths[i];
        }
    }


    QBarSeries *series = new QBarSeries();
    series->append(incomeSet);
    series->append(expenseSet);
    series->append(profitLossSet);
    incomeSet->setColor(QColor("#021024"));
        expenseSet->setColor(QColor("#86ABCF"));
        profitLossSet->setColor(QColor("#C38EB4"));



    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle(QString("Monthly Income, Expense, and Profit/Loss for %1").arg(selectedYear));
    chart->setTitleFont(QFont("Arial", 14, QFont::Bold));
        chart->setTitleBrush(QBrush(QColor("#021024")));
        chart->setBackgroundBrush(QColor("#FFFFFF"));
            chart->setBackgroundPen(Qt::NoPen);


    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);


    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(monthsWithData);


    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);


    QValueAxis *axisY = new QValueAxis();
    axisY->setTitleText("Amount");
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);


    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);


    QVBoxLayout *chartLayout = new QVBoxLayout(ui->chartZone);
    chartLayout->addWidget(chartView);
    ui->chartZone->setLayout(chartLayout);
}


MainWindow::~MainWindow()
{
 if(db.isOpen())
 db.close();
 delete ui;
 delete arduino;
}

void MainWindow::on_typeRecord_currentIndexChanged(int index)
{
 ui->categoryRecord->clear();
 if(index==0)//income case
 {
 ui->categoryRecord->addItem("Room Revenue");
 ui->categoryRecord->addItem("Food and Beverage Sales");
 ui->categoryRecord->addItem("Event and Conference Services");
 ui->categoryRecord->addItem("Spa and Wellness Services");
 ui->categoryRecord->addItem("Leisure and Entertainment Services");
 ui->categoryRecord->addItem("Early Check-in and Late Check-out Fees");


 }
 else{
 ui->categoryRecord->addItem("Staff Salaries and Benefits");
 ui->categoryRecord->addItem("Technology and Software Maintenance");
 ui->categoryRecord->addItem("Utility Expense");
 ui->categoryRecord->addItem("Food and Beverage Costs");
 ui->categoryRecord->addItem("Property Maintenance and Repairs");
 ui->categoryRecord->addItem("Furniture, Fixtures, and Equipment");

 }

}
void MainWindow::connectToDatabase(){

//database connection
db=QSqlDatabase::addDatabase("QODBC");
db.setDatabaseName("souce_projet_smatHotel");
db.setUserName("mira");
db.setPassword("123456");

}



void MainWindow::on_pushButton_17_clicked()
{
    bool isInt;
    int id = ui->id_transaction->text().toInt(&isInt);

    if (!isInt) {
        QMessageBox::warning(this, "Input Error", "Transaction ID must be a numeric value.");
        return;
    }
    QSqlQuery checkQuery(db);
       checkQuery.prepare("SELECT COUNT(*) FROM transactions WHERE id = :id");
       checkQuery.bindValue(":id", id);

       if (!checkQuery.exec()) {
           qDebug() << "Error checking transaction ID: " << checkQuery.lastError().text();
           return;
       }

       checkQuery.next();
       if (checkQuery.value(0).toInt() > 0) {
           QMessageBox::warning(this, "Input Error", "Transaction ID already exists. Please enter a different ID.");
           return;
       }
 if(!db.isOpen())
 connectToDatabase();
 if(!db.open())
 {
 qDebug()<<"error,unable to connect ro database";
 qDebug()<<db.lastError().text();
 return;
 }

 if (ui->id_transaction->text().isEmpty() ||
 ui->date_transaction->text().isEmpty() ||
 ui->typeRecord->currentText().isEmpty() ||
 ui->categoryRecord->currentText().isEmpty() ||
 ui->amount_transaction->text().isEmpty() ||
 ui->payment_transaction->currentText().isEmpty()) {

 QMessageBox::warning(this, "Input Error", "Please fill in all fields before adding a transaction.");
 return;
 }
 QSqlQuery query(db);
 QString formatedDate=QString("to_date(:date,'DD MM YYYY')");

 query.prepare("insert into transactions values (:id,"+formatedDate+",:type,:category,:amount,:method)");
 query.bindValue(":id",ui->id_transaction->text().toInt());
 query.bindValue(":date",ui->date_transaction->text());
 query.bindValue(":type",ui->typeRecord->currentText());
 query.bindValue(":category",ui->categoryRecord->currentText());
 query.bindValue(":amount",ui->amount_transaction->text().toDouble());
 query.bindValue(":method",ui->payment_transaction->currentText());
 QMessageBox msb;
 msb.setWindowTitle("add transaction state");

 if(!query.exec())
 {
 msb.setText("error adding transaction "+query.value(1).toString());

 }
 else
 {
 msb.setText(" transaction has been added successfuly "+query.value(1).toString());
 ui->typeRecord_2->setCurrentIndex(0);
 // ui->date_transaction_2->setDate(QDate());
 // ui->date_transaction_2->setDisplayFormat("yyyy-MM-dd");
 //ui->date_transaction_2->setSpecialValueText("");
 ui->date_Search->setText("");
 ui->date_Search->setPlaceholderText(" Search Date");
 getTransactions();

 }
 msb.exec();

}
QVector<Transaction> MainWindow::getTransactions(){

 QVector<Transaction> transactions;
 if(!db.isOpen())
 connectToDatabase();
 if(!db.open())
 {
 qDebug()<<"error,unable to connect ro database";
 qDebug()<<db.lastError().text();
 return transactions;
 }


 QSqlQuery query(db);
 query.exec("select * from transactions;");

 QMessageBox msb;
 // msb.setWindowTitle("connection state");
 // msb.setText("connection established "+query.value(1).toString());
 // msb.exec();
 int row=0;
 ui->listTransaction->clear();
 ui->listTransaction->clearContents();
 ui->listTransaction->setRowCount(0);
 QStringList titles;
 titles << "ID" << "Date" << "type" << "category" << "Amount " << "Payment Method" << "Actions";

 ui->listTransaction->setHorizontalHeaderLabels(titles);

 while(query.next()){
 ui->listTransaction->insertRow(row);
 ui->listTransaction->setItem(row,0,new QTableWidgetItem(query.value(0).toString()));//id
 ui->listTransaction->setItem(row,1,new QTableWidgetItem(query.value(1).toDate().toString("yyyy-MM-dd")));//date
 ui->listTransaction->setItem(row,2,new QTableWidgetItem(query.value(2).toString()));//type
 ui->listTransaction->setItem(row,3,new QTableWidgetItem(query.value(3).toString()));//category
 ui->listTransaction->setItem(row,4,new QTableWidgetItem(query.value(4).toString()));//amount
 ui->listTransaction->setItem(row,5,new QTableWidgetItem(query.value(5).toString()));//paymentmethod
 Transaction t1;
 t1.setId(query.value(0).toInt());
 t1.setDate(query.value(1).toDate());
 qDebug()<<query.value(1).toDate().toString();
 t1.setType(query.value(2).toString());
 t1.setCategory(query.value(3).toString());
 t1.setAmount(query.value(4).toDouble());
 t1.setPaymentMethod(query.value(5).toString());
 transactions.append(t1);




 QPushButton* updateButton=new QPushButton();
 updateButton->setIcon(QIcon(":/icons/icons/update.png"));

 QPushButton* deleteButton=new QPushButton();
 deleteButton->setIcon(QIcon(":/icons/icons/delete.png"));
 deleteButton->setStyleSheet("background-color:#ff3333");


 QWidget* cellAction=new QWidget();
 QHBoxLayout* layout=new QHBoxLayout(cellAction);
 layout->addWidget(updateButton);
 layout->addWidget(deleteButton);
 layout->setContentsMargins(0,0,0,0);
 layout->setSpacing(5);
 ui->listTransaction->setCellWidget(row,6,cellAction);//edit (2 icons)

 QObject::connect(deleteButton, &QPushButton::clicked, [=]() {
 // Ask for confirmation before deleting
 QMessageBox::StandardButton reply;
 reply = QMessageBox::question(this, "Confirm Deletion",
 "Are you sure you want to delete this transaction?",
 QMessageBox::Yes | QMessageBox::No);
 if (reply != QMessageBox::Yes) {
 return; // User chose not to delete, exit the function
 }

if (!db.isOpen()) {
    connectToDatabase();
}
 if (!db.open()) {
 qDebug() << "Error, unable to connect to database";
 qDebug() << db.lastError().text();
 return;
 }

 QSqlQuery query(db);
 query.prepare("DELETE FROM transactions WHERE id = :id");
 query.bindValue(":id", (ui->listTransaction->item(row, 0)->text()).toInt());

 QMessageBox msb;
 msb.setWindowTitle("Delete Transaction State");

 if (!query.exec()) {
 msb.setText("Error deleting transaction: " + query.lastError().text());
 } else {
 msb.setText("Transaction has been deleted successfully.");
 getTransactions();
 }
 db.close();
 msb.exec();
 });

 QObject::connect(updateButton,&QPushButton::clicked,[=]()
 {
 if(!db.isOpen())
 connectToDatabase();


 if(!db.open())
 {
 qDebug()<<"error,unable to connect ro database";
 qDebug()<<db.lastError().text();
 return;
 }


 QSqlQuery query(db);
 QDate tdate=QDate::fromString(ui->listTransaction->item(row,1)->text(),"yyyy-MM-dd");
 if(!tdate.isValid()){
 qDebug()<< "invalid date format";
 return;
 }
 QString formatedDate=tdate.toString("yyyy-MM-dd");
 query.prepare("UPDATE transactions SET TANSACTION_DATE=to_date(:date,'YYYY-MM-DD'),transaction_type=:type, category=:category, amount=:amount,payment_method=:method WHERE id=:id");
 query.bindValue(":id",(ui->listTransaction->item(row,0)->text()).toInt());


 query.bindValue(":date",formatedDate);
 query.bindValue(":type",ui->listTransaction->item(row,2)->text());
 query.bindValue(":category",ui->listTransaction->item(row,3)->text());
 query.bindValue(":amount",(ui->listTransaction->item(row,4)->text()));
 query.bindValue(":method",ui->listTransaction->item(row,5)->text());
 QMessageBox msb;
 msb.setWindowTitle("update transaction state");

 if(!query.exec())
 {
 msb.setText("error updating transaction "+query.value(1).toString());

 }
 else
 {
 msb.setText(" transaction has been updated successfuly "+query.value(1).toString());
 getTransactions();

 }
 db.close();
 msb.exec();


 });

 row++;


 }//fin while

 // ui->listTransaction->resize(850,300);
 //ui->listTransaction->show();
 return transactions;

 }









void MainWindow::on_listTransaction_cellDoubleClicked(int row, int column)
{
 ui->listTransaction->editItem(ui->listTransaction->item(row,column));
}


void MainWindow::on_typeRecord_2_currentIndexChanged(int index)
{

}

QList<Transaction> *MainWindow::executeQuery(QString request){
 QList<Transaction>* lstTransaction= new QList<Transaction>();
 if(!db.isOpen())
 connectToDatabase();
 if(!db.open())
 {
 qDebug()<<"error,unable to connect ro database";
 qDebug()<<db.lastError().text();
 return lstTransaction;
 }


 QSqlQuery query(db);
 qDebug() <<request;
 query.exec(request);


 // QMessageBox msb;
 //msb.setWindowTitle("connection state");
 //msb.setText("connection established "+query.value(1).toString());
 //msb.exec();

 while(query.next()){

 int id=query.value(0).toInt();//id
 QDate date=query.value(1).toDate();
 QString type=query.value(2).toString();
 QString category=query.value(3).toString();
 double amount=query.value(4).toDouble();
 QString method=query.value(5).toString();
 lstTransaction->append(Transaction(id,type,category,date,method,amount));
 }

return lstTransaction;

}


void MainWindow::on_typeRecord_2_currentTextChanged(const QString &arg1)
{
 // QString request = QString("SELECT * FROM transactions WHERE transaction_type = '%1';").arg(arg1);
 QString request = QString("SELECT * FROM transactions;");

 QList<Transaction> *lstTrans = executeQuery(request);

 ui->listTransaction->clearContents();
 ui->listTransaction->setRowCount(0);
 QStringList titles = { "ID", "Date", "Type", "Category", "Amount", "Payment Method", "Actions" };
 ui->listTransaction->setHorizontalHeaderLabels(titles);
 //if (arg1.isEmpty() || arg1 == "All") { // Assuming "All" is an option in your dropdown to show all types
 // QString request = QString("SELECT * FROM transactions WHERE transaction_type = '%1';").arg(arg1); } // Show all transactions

 if(arg1!="All"){
 int row1 = 0;
 if(arg1=="income"){
 qDebug() <<"start "<<arg1<<row1;
 // row-=4;
 }
 for (const auto &transaction : *lstTrans) {
 row1=0;
 if(transaction.getType()==arg1){

 qDebug() <<arg1<<row1;
 ui->listTransaction->insertRow(row1);

 ui->listTransaction->setItem(row1, 0, new QTableWidgetItem(QString::number(transaction.getId())));
 ui->listTransaction->setItem(row1, 1, new QTableWidgetItem(transaction.getDate().toString("yyyy-MM-dd")));
 ui->listTransaction->setItem(row1, 2, new QTableWidgetItem(transaction.getType()));
 ui->listTransaction->setItem(row1, 3, new QTableWidgetItem(transaction.getCategory()));
 ui->listTransaction->setItem(row1, 4, new QTableWidgetItem(QString::number(transaction.getAmount())));
 ui->listTransaction->setItem(row1, 5, new QTableWidgetItem(transaction.getPaymentMethod()));
 }

 QPushButton* updateButton=new QPushButton();
 updateButton->setIcon(QIcon(":/icons/icons/update.png"));

 QPushButton* deleteButton=new QPushButton();
 deleteButton->setIcon(QIcon(":/icons/icons/delete.png"));
 deleteButton->setStyleSheet("background-color:#ff3333");


 QWidget* cellAction=new QWidget();
 QHBoxLayout* layout=new QHBoxLayout(cellAction);
 layout->addWidget(updateButton);
 layout->addWidget(deleteButton);
 layout->setContentsMargins(0,0,0,0);
 layout->setSpacing(5);
 ui->listTransaction->setCellWidget(row1,6,cellAction);//edit (2 icons)


 QObject::connect(deleteButton, &QPushButton::clicked, [=]() {
 // Ask for confirmation before deleting
 QMessageBox::StandardButton reply;
 reply = QMessageBox::question(this, "Confirm Deletion",
 "Are you sure you want to delete this transaction?",
 QMessageBox::Yes | QMessageBox::No);
 if (reply != QMessageBox::Yes) {
 return; // User chose not to delete, exit the function
 }



if (!db.isOpen()) {
    connectToDatabase();
}
if (!db.open()) {
 qDebug() << "Error, unable to connect to database";
 qDebug() << db.lastError().text();
 return;
 }

 QSqlQuery query(db);
 query.prepare("DELETE FROM transactions WHERE id = :id");
 query.bindValue(":id", (ui->listTransaction->item(row1, 0)->text()).toInt());

 QMessageBox msb;
 msb.setWindowTitle("Delete Transaction State");

 if (!query.exec()) {
 msb.setText("Error deleting transaction: " + query.lastError().text());
 } else {
 msb.setText("Transaction has been deleted successfully.");
 getTransactions();
 }
 db.close();
 msb.exec();
 });
 QObject::connect(updateButton,&QPushButton::clicked,[=]()
 {
 if(!db.isOpen())
    connectToDatabase();


 if(!db.open())
 {
 qDebug()<<"error,unable to connect ro database";
 qDebug()<<db.lastError().text();
 return;
 }


 QSqlQuery query(db);
 QDate tdate=QDate::fromString(ui->listTransaction->item(row1,1)->text(),"yyyy-MM-dd");
 if(!tdate.isValid()){
 qDebug()<< "invalid date format";
 return;
 }
 QString formatedDate=tdate.toString("yyyy-MM-dd");
 query.prepare("UPDATE transactions SET TANSACTION_DATE=to_date(:date,'YYYY-MM-DD'),transaction_type=:type, category=:category, amount=:amount,payment_method=:method WHERE id=:id");
 query.bindValue(":id",(ui->listTransaction->item(row1,0)->text()).toInt());


 query.bindValue(":date",formatedDate);
 query.bindValue(":type",ui->listTransaction->item(row1,2)->text());
 query.bindValue(":category",ui->listTransaction->item(row1,3)->text());
 query.bindValue(":amount",(ui->listTransaction->item(row1,4)->text()));
 query.bindValue(":method",ui->listTransaction->item(row1,5)->text());
 QMessageBox msb;
 msb.setWindowTitle("update transaction state");

 if(!query.exec())
 {
 msb.setText("error updating transaction "+query.value(1).toString());

 }
 else
 {
 msb.setText(" transaction has been updated successfuly "+query.value(1).toString());
 getTransactions();

 }
 db.close();
 msb.exec();


 });

 row1++;

 }
 }
 else{
 int row=0;
 for (const auto &transaction : *lstTrans) {
 qDebug() <<arg1<<row;

 ui->listTransaction->insertRow(row);

 ui->listTransaction->setItem(row, 0, new QTableWidgetItem(QString::number(transaction.getId())));
 ui->listTransaction->setItem(row, 1, new QTableWidgetItem(transaction.getDate().toString("yyyy-MM-dd")));
 ui->listTransaction->setItem(row, 2, new QTableWidgetItem(transaction.getType()));
 ui->listTransaction->setItem(row, 3, new QTableWidgetItem(transaction.getCategory()));
 ui->listTransaction->setItem(row, 4, new QTableWidgetItem(QString::number(transaction.getAmount())));
 ui->listTransaction->setItem(row, 5, new QTableWidgetItem(transaction.getPaymentMethod()));





 QPushButton* updateButton=new QPushButton();
 updateButton->setIcon(QIcon(":/icons/icons/update.png"));

 QPushButton* deleteButton=new QPushButton();
 deleteButton->setIcon(QIcon(":/icons/icons/delete.png"));
 deleteButton->setStyleSheet("background-color:#ff3333");


 QWidget* cellAction=new QWidget();
 QHBoxLayout* layout=new QHBoxLayout(cellAction);
 layout->addWidget(updateButton);
 layout->addWidget(deleteButton);
 layout->setContentsMargins(0,0,0,0);
 layout->setSpacing(5);
 ui->listTransaction->setCellWidget(row,6,cellAction);//edit (2 icons)

 QObject::connect(deleteButton, &QPushButton::clicked, [=]() {
 // Ask for confirmation before deleting
 QMessageBox::StandardButton reply;
 reply = QMessageBox::question(this, "Confirm Deletion",
 "Are you sure you want to delete this transaction?",
 QMessageBox::Yes | QMessageBox::No);
 if (reply != QMessageBox::Yes) {
 return; // User chose not to delete, exit the function
 }

    if (!db.isOpen()) {
        connectToDatabase();
    }
    if (!db.open()) {
 qDebug() << "Error, unable to connect to database";
 qDebug() << db.lastError().text();
 return;
 }

 QSqlQuery query(db);
 query.prepare("DELETE FROM transactions WHERE id = :id");
 query.bindValue(":id", (ui->listTransaction->item(row, 0)->text()).toInt());

 QMessageBox msb;
 msb.setWindowTitle("Delete Transaction State");

 if (!query.exec()) {
 msb.setText("Error deleting transaction: " + query.lastError().text());
 } else {
 msb.setText("Transaction has been deleted successfully.");
 getTransactions();
 }
 db.close();
 msb.exec();
 });
 QObject::connect(updateButton,&QPushButton::clicked,[=]()
 {
 if(!db.isOpen())
 connectToDatabase();


 if(!db.open())
 {
 qDebug()<<"error,unable to connect ro database";
 qDebug()<<db.lastError().text();
 return;
 }


 QSqlQuery query(db);
 QDate tdate=QDate::fromString(ui->listTransaction->item(row,1)->text(),"yyyy-MM-dd");
 if(!tdate.isValid()){
 qDebug()<< "invalid date format";
 return;
 }
 QString formatedDate=tdate.toString("yyyy-MM-dd");
 query.prepare("UPDATE transactions SET TANSACTION_DATE=to_date(:date,'YYYY-MM-DD'),transaction_type=:type, category=:category, amount=:amount,payment_method=:method WHERE id=:id");
 query.bindValue(":id",(ui->listTransaction->item(row,0)->text()).toInt());


 query.bindValue(":date",formatedDate);
 query.bindValue(":type",ui->listTransaction->item(row,2)->text());
 query.bindValue(":category",ui->listTransaction->item(row,3)->text());
 query.bindValue(":amount",(ui->listTransaction->item(row,4)->text()));
 query.bindValue(":method",ui->listTransaction->item(row,5)->text());
 QMessageBox msb;
 msb.setWindowTitle("update transaction state");

 if(!query.exec())
 {
 msb.setText("error updating transaction "+query.value(1).toString());

 }
 else
 {
 msb.setText(" transaction has been updated successfuly "+query.value(1).toString());
 getTransactions();

 }
 db.close();
 msb.exec();


 });

 row++;


 }//enf for
 }//end else


}

QString MainWindow::exportTableToexcel(QTableWidget* table)
{
 // Set up the Excel document
 /*QXlsx::Document xls;
 QXlsx::Format headerFormat;
 QXlsx::Format oddRowFormat;
 QXlsx::Format evenRowFormat;
 QXlsx::Format borderFormat;

 // Header formatting
 headerFormat.setFontBold(true);
 headerFormat.setFontColor(QColor(Qt::blue));
 headerFormat.setFontSize(12);
 headerFormat.setHorizontalAlignment(QXlsx::Format::AlignHCenter);
 headerFormat.setPatternBackgroundColor(QColor(200, 221, 242)); // Light blue background for header

 // Row formatting
 oddRowFormat.setPatternBackgroundColor(QColor(240, 248, 255)); // Light alternating color for odd rows
 evenRowFormat.setPatternBackgroundColor(QColor(255, 255, 255)); // White background for even rows

 // Border formatting
 borderFormat.setBorderStyle(QXlsx::Format::BorderThin);

 // Set a larger column width for all columns
 int defaultColumnWidth = 20; // Set width for all columns
 for (int column = 1; column <= table->columnCount(); column++)
 {
 xls.setColumnWidth(column, defaultColumnWidth); // Apply uniform width to all columns
 }

 // Write the header
 for (int column = 0; column < table->columnCount() - 1; column++)
 {
 QString header = table->horizontalHeaderItem(column)->text();
 xls.write(1, column + 1, header, headerFormat); // Apply header format
 }

 // Write table data with alternating row colors and borders
 for (int row = 0; row < table->rowCount(); row++)
 {
 QXlsx::Format rowFormat = (row % 2 == 0) ? evenRowFormat : oddRowFormat; // Alternate row colors
 rowFormat.mergeFormat(borderFormat); // Add borders to each cell

 for (int col = 0; col < table->columnCount(); col++)
 {
 QTableWidgetItem* item = table->item(row, col);
 if (item)
 {
 xls.write(row + 2, col + 1, item->text(), rowFormat); // Apply row formatting
 }
 }
 }

 // Save the file
 QString filePath = QFileDialog::getSaveFileName(nullptr, "Save Excel file", "", "Excel files (*.xlsx)");
 if (!filePath.isEmpty())
 {
 if (!filePath.endsWith(".xlsx"))
 filePath += ".xlsx";
 xls.saveAs(filePath);
 }

 return filePath;*/

    QXlsx::Document xls;
        QXlsx::Format headerFormat;
        QXlsx::Format oddRowFormat;
        QXlsx::Format evenRowFormat;
        QXlsx::Format borderFormat;

        // Header formatting
        headerFormat.setFontBold(true);
        headerFormat.setFontColor(QColor(Qt::blue));
        headerFormat.setFontSize(12);
        headerFormat.setHorizontalAlignment(QXlsx::Format::AlignHCenter);
        headerFormat.setPatternBackgroundColor(QColor(200, 221, 242)); // Light blue background for header

        // Row formatting
        oddRowFormat.setPatternBackgroundColor(QColor(240, 248, 255)); // Light alternating color for odd rows
        evenRowFormat.setPatternBackgroundColor(QColor(255, 255, 255)); // White background for even rows

        // Border formatting
        borderFormat.setBorderStyle(QXlsx::Format::BorderThin);

        // Set a larger column width for all columns
        int defaultColumnWidth = 20;
        for (int column = 1; column <= table->columnCount() - 1; column++) // Skip the last column
        {
            xls.setColumnWidth(column, defaultColumnWidth);
        }

        // Write the header
        for (int column = 0; column < table->columnCount() - 1; column++) // Skip the last column
        {
            QString header = table->horizontalHeaderItem(column)->text();
            xls.write(1, column + 1, header, headerFormat); // Apply header format
        }

        // Initialize totals
        double totalIncome = 0.0;
        double totalExpense = 0.0;

        // Write the table data with alternating row colors
        int currentRow = 2; // Start writing data from the second row
        for (int row = 0; row < table->rowCount(); row++)
        {
            QXlsx::Format rowFormat = (currentRow % 2 == 0) ? evenRowFormat : oddRowFormat; // Alternate row colors
            rowFormat.mergeFormat(borderFormat);

            for (int col = 0; col < table->columnCount() - 1; col++) // Skip the last column
            {
                QTableWidgetItem* item = table->item(row, col);
                if (item)
                {
                    xls.write(currentRow, col + 1, item->text(), rowFormat);
                }
            }

            // Calculate totals based on the "type" column
            QString type = table->item(row, 2)->text(); // Assuming "type" is in the 3rd column
            double amount = table->item(row, 4)->text().toDouble(); // Assuming "amount" is in the 5th column
            if (type == "income")
            {
                totalIncome += amount;
            }
            else if (type == "expense")
            {
                totalExpense += amount;
            }

            currentRow++;
        }

        // Write totals
        QXlsx::Format totalFormat;
        totalFormat.setFontBold(true);
        totalFormat.setFontColor(QColor(Qt::red));
        totalFormat.setPatternBackgroundColor(QColor(230, 230, 250)); // Light purple background
        totalFormat.setBorderStyle(QXlsx::Format::BorderThin);

        xls.write(currentRow, 1, "Total Income:", totalFormat);
        xls.write(currentRow, 5, totalIncome, totalFormat); // Write total income in the "Amount" column
        currentRow++;

        xls.write(currentRow, 1, "Total Expense:", totalFormat);
        xls.write(currentRow, 5, totalExpense, totalFormat); // Write total expense in the "Amount" column
        currentRow++;

        xls.write(currentRow, 1, "Net Balance:", totalFormat);
        xls.write(currentRow, 5, totalIncome - totalExpense, totalFormat); // Write net balance
        currentRow++;

        // Save the file
        QString filePath = QFileDialog::getSaveFileName(nullptr, "Save Excel file", "", "Excel files (*.xlsx)");
        if (!filePath.isEmpty())
        {
            if (!filePath.endsWith(".xlsx"))
                filePath += ".xlsx";
            xls.saveAs(filePath);
        }

        return filePath;

}

void MainWindow::on_pushButton_15_clicked()
{

 QString filePath=exportTableToexcel(ui->listTransaction);
 QMessageBox::information(this, "Export Successful", "CSV file exported successfully to " + filePath);
}



void MainWindow::on_pushButton_2_clicked()
{


    int amountColumnIndex = 4;
    qDebug() << "Current amounts in column:" << amountColumnIndex;
    for (int row = 0; row < ui->listTransaction->rowCount(); row++) {
        QTableWidgetItem* item = ui->listTransaction->item(row, amountColumnIndex);
        if (item) {
            qDebug() << "Row:" << row << "Amount:" << item->text();
        }
    }


    QList<QPair<double, int>> itemsToSort;
    for (int row = 0; row < ui->listTransaction->rowCount(); row++) {
        QTableWidgetItem* item = ui->listTransaction->item(row, amountColumnIndex);
        if (item) {

            bool ok;
            double amount = item->text().toDouble(&ok);
            if (ok) {
                itemsToSort.append(qMakePair(amount, row));
            }
        }
    }


    std::sort(itemsToSort.begin(), itemsToSort.end(), [](const QPair<double, int>& a, const QPair<double, int>& b) {
        return a.first > b.first;
    });


    QList<QTableWidgetItem*> sortedRowItems;
    for (int i = 0; i < itemsToSort.size(); ++i) {
        int sortedRow = itemsToSort[i].second;
        for (int col = 0; col < ui->listTransaction->columnCount(); ++col) {
            QTableWidgetItem* item = ui->listTransaction->takeItem(sortedRow, col);
            sortedRowItems.append(item);
        }
    }


    int rowIndex = 0;
    for (int i = 0; i < itemsToSort.size(); ++i) {
        int sortedRow = itemsToSort[i].second;
        for (int col = 0; col < ui->listTransaction->columnCount(); ++col) {

            ui->listTransaction->setItem(rowIndex, col, sortedRowItems.takeFirst());
        }
        rowIndex++;
    }


    qDebug() << "Amounts after sorting:";
    for (int row = 0; row < ui->listTransaction->rowCount(); row++) {
        QTableWidgetItem* item = ui->listTransaction->item(row, amountColumnIndex);
        if (item) {
            qDebug() << "Row:" << row << "Amount:" << item->text();
        }
    }

}






void MainWindow::on_date_Search_textChanged(const QString &arg1)
{
 QString dateValue=ui->date_Search->text();
 QDate selected_date=QDate::fromString(dateValue,"yyyy-MM-dd");
 ui->listTransaction->clearContents();
 ui->listTransaction->setRowCount(0);
 QStringList titles = { "ID", "Date", "Type", "Category", "Amount", "Payment Method", "Actions" };
 ui->listTransaction->setHorizontalHeaderLabels(titles);
 QString request = QString("SELECT * FROM transactions;");

 QList<Transaction> *lstTrans = executeQuery(request);

 if(dateValue.isEmpty()){
 ui->date_Search->setPlaceholderText(" Search Date");

 int row=0;
 for (const auto &transaction : *lstTrans) {


 ui->listTransaction->insertRow(row);

 ui->listTransaction->setItem(row, 0, new QTableWidgetItem(QString::number(transaction.getId())));
 ui->listTransaction->setItem(row, 1, new QTableWidgetItem(transaction.getDate().toString("yyyy-MM-dd")));
 ui->listTransaction->setItem(row, 2, new QTableWidgetItem(transaction.getType()));
 ui->listTransaction->setItem(row, 3, new QTableWidgetItem(transaction.getCategory()));
 ui->listTransaction->setItem(row, 4, new QTableWidgetItem(QString::number(transaction.getAmount())));
 ui->listTransaction->setItem(row, 5, new QTableWidgetItem(transaction.getPaymentMethod()));





 QPushButton* updateButton=new QPushButton();
 updateButton->setIcon(QIcon(":/icons/icons/update.png"));

 QPushButton* deleteButton=new QPushButton();
 deleteButton->setIcon(QIcon(":/icons/icons/delete.png"));
 deleteButton->setStyleSheet("background-color:#ff3333");


 QWidget* cellAction=new QWidget();
 QHBoxLayout* layout=new QHBoxLayout(cellAction);
 layout->addWidget(updateButton);
 layout->addWidget(deleteButton);
 layout->setContentsMargins(0,0,0,0);
 layout->setSpacing(5);
 ui->listTransaction->setCellWidget(row,6,cellAction);//edit (2 icons)

 QObject::connect(deleteButton, &QPushButton::clicked, [=]() {
 // Ask for confirmation before deleting
 QMessageBox::StandardButton reply;
 reply = QMessageBox::question(this, "Confirm Deletion",
 "Are you sure you want to delete this transaction?",
 QMessageBox::Yes | QMessageBox::No);
 if (reply != QMessageBox::Yes) {
 return; // User chose not to delete, exit the function
 }

    if (!db.isOpen()) {
        connectToDatabase();
    }
    if (!db.open()) {
    qDebug() << "Error, unable to connect to database";
    qDebug() << db.lastError().text();
    return;
    }

 QSqlQuery query(db);
 query.prepare("DELETE FROM transactions WHERE id = :id");
 query.bindValue(":id", (ui->listTransaction->item(row, 0)->text()).toInt());

 QMessageBox msb;
 msb.setWindowTitle("Delete Transaction State");

 if (!query.exec()) {
 msb.setText("Error deleting transaction: " + query.lastError().text());
 } else {
 msb.setText("Transaction has been deleted successfully.");
 getTransactions();
 }
 db.close();
 msb.exec();
 });
 QObject::connect(updateButton,&QPushButton::clicked,[=]()
 {
 if(!db.isOpen())
    connectToDatabase();


 if(!db.open())
 {
 qDebug()<<"error,unable to connect ro database";
 qDebug()<<db.lastError().text();
 return;
 }


 QSqlQuery query(db);
 QDate tdate=QDate::fromString(ui->listTransaction->item(row,1)->text(),"yyyy-MM-dd");
 if(!tdate.isValid()){
 qDebug()<< "invalid date format";
 return;
 }
 QString formatedDate=tdate.toString("yyyy-MM-dd");
 query.prepare("UPDATE transactions SET TANSACTION_DATE=to_date(:date,'YYYY-MM-DD'),transaction_type=:type, category=:category, amount=:amount,payment_method=:method WHERE id=:id");
 query.bindValue(":id",(ui->listTransaction->item(row,0)->text()).toInt());


 query.bindValue(":date",formatedDate);
 query.bindValue(":type",ui->listTransaction->item(row,2)->text());
 query.bindValue(":category",ui->listTransaction->item(row,3)->text());
 query.bindValue(":amount",(ui->listTransaction->item(row,4)->text()));
 query.bindValue(":method",ui->listTransaction->item(row,5)->text());
 QMessageBox msb;
 msb.setWindowTitle("update transaction state");

 if(!query.exec())
 {
 msb.setText("error updating transaction "+query.value(1).toString());

 }
 else
 {
 msb.setText(" transaction has been updated successfuly "+query.value(1).toString());
 getTransactions();

 }
 db.close();
 msb.exec();


 });

 row++;


 }//enf for


 }//end if
 else
 {
 int row1=0;
 for (const auto &transaction : *lstTrans) {
 row1=0;
 if(transaction.getDate()==selected_date){

 //qDebug() <<arg1<<row1;
 ui->listTransaction->insertRow(row1);

 ui->listTransaction->setItem(row1, 0, new QTableWidgetItem(QString::number(transaction.getId())));
 ui->listTransaction->setItem(row1, 1, new QTableWidgetItem(transaction.getDate().toString("yyyy-MM-dd")));
 ui->listTransaction->setItem(row1, 2, new QTableWidgetItem(transaction.getType()));
 ui->listTransaction->setItem(row1, 3, new QTableWidgetItem(transaction.getCategory()));
 ui->listTransaction->setItem(row1, 4, new QTableWidgetItem(QString::number(transaction.getAmount())));
 ui->listTransaction->setItem(row1, 5, new QTableWidgetItem(transaction.getPaymentMethod()));
 }

 QPushButton* updateButton=new QPushButton();
 updateButton->setIcon(QIcon(":/icons/icons/update.png"));

 QPushButton* deleteButton=new QPushButton();
 deleteButton->setIcon(QIcon(":/icons/icons/delete.png"));
 deleteButton->setStyleSheet("background-color:#ff3333");


 QWidget* cellAction=new QWidget();
 QHBoxLayout* layout=new QHBoxLayout(cellAction);
 layout->addWidget(updateButton);
 layout->addWidget(deleteButton);
 layout->setContentsMargins(0,0,0,0);
 layout->setSpacing(5);
 ui->listTransaction->setCellWidget(row1,6,cellAction);//edit (2 icons)

 QObject::connect(deleteButton,&QPushButton::clicked,[=]()
 {
    if (!db.isOpen()) {
        connectToDatabase();
    }
    if(!db.open())
    {
    qDebug()<<"error,unable to connect ro database";
    qDebug()<<db.lastError().text();
    return ;
    }

 QSqlQuery query(db);
 query.prepare("delete from transactions where id=:id");
 query.bindValue(":id",(ui->listTransaction->item(row1,0)->text()).toInt());

 QMessageBox msb;
 msb.setWindowTitle("delete transaction state");

 if(!query.exec())
 {
 msb.setText("error deleting transaction "+query.value(1).toString());

 }
 else
 {
 msb.setText(" transaction has been deleted successfuly "+query.value(1).toString());
 getTransactions();

 }
 db.close();
 msb.exec();


 });
 QObject::connect(updateButton,&QPushButton::clicked,[=]()
 {
 if(!db.isOpen())
    connectToDatabase();


 if(!db.open())
 {
 qDebug()<<"error,unable to connect ro database";
 qDebug()<<db.lastError().text();
 return;
 }


 QSqlQuery query(db);
 QDate tdate=QDate::fromString(ui->listTransaction->item(row1,1)->text(),"yyyy-MM-dd");
 if(!tdate.isValid()){
 qDebug()<< "invalid date format";
 return;
 }
 QString formatedDate=tdate.toString("yyyy-MM-dd");
 query.prepare("UPDATE transactions SET TANSACTION_DATE=to_date(:date,'YYYY-MM-DD'),transaction_type=:type, category=:category, amount=:amount,payment_method=:method WHERE id=:id");
 query.bindValue(":id",(ui->listTransaction->item(row1,0)->text()).toInt());


 query.bindValue(":date",formatedDate);
 query.bindValue(":type",ui->listTransaction->item(row1,2)->text());
 query.bindValue(":category",ui->listTransaction->item(row1,3)->text());
 query.bindValue(":amount",(ui->listTransaction->item(row1,4)->text()));
 query.bindValue(":method",ui->listTransaction->item(row1,5)->text());
 QMessageBox msb;
 msb.setWindowTitle("update transaction state");

 if(!query.exec())
 {
 msb.setText("error updating transaction "+query.value(1).toString());

 }
 else
 {
 msb.setText(" transaction has been updated successfuly "+query.value(1).toString());
 getTransactions();

 }
 db.close();
 msb.exec();


 });

 row1++;

 }


 }
}


void MainWindow::on_search_id_textChanged(const QString &arg1)
{
 QString idValue = ui->search_id->text(); // Get ID from the search field
 ui->listTransaction->clearContents();
 ui->listTransaction->setRowCount(0);
 QStringList titles = { "ID", "Date", "Type", "Category", "Amount", "Payment Method", "Actions" };
 ui->listTransaction->setHorizontalHeaderLabels(titles);

 // Query to get transactions by matching ID if input is not empty, or all transactions if it is
 QString request;
 if (idValue.isEmpty()) {
 request = "SELECT * FROM transactions;";
 } else {
 request = QString("SELECT * FROM transactions WHERE id = %1;").arg(idValue.toInt());
 }

 // Execute query and get the results
 QList<Transaction> *lstTrans = executeQuery(request);

 // Populate the table with results
 int row = 0;
 for (const auto &transaction : *lstTrans) {
 ui->listTransaction->insertRow(row);

 // Populate transaction details in each row
 ui->listTransaction->setItem(row, 0, new QTableWidgetItem(QString::number(transaction.getId())));
 ui->listTransaction->setItem(row, 1, new QTableWidgetItem(transaction.getDate().toString("yyyy-MM-dd")));
 ui->listTransaction->setItem(row, 2, new QTableWidgetItem(transaction.getType()));
 ui->listTransaction->setItem(row, 3, new QTableWidgetItem(transaction.getCategory()));
 ui->listTransaction->setItem(row, 4, new QTableWidgetItem(QString::number(transaction.getAmount())));
 ui->listTransaction->setItem(row, 5, new QTableWidgetItem(transaction.getPaymentMethod()));

 // Action buttons (Update and Delete)
 QPushButton* updateButton = new QPushButton();
 updateButton->setIcon(QIcon(":/icons/icons/update.png"));

 QPushButton* deleteButton = new QPushButton();
 deleteButton->setIcon(QIcon(":/icons/icons/delete.png"));
 deleteButton->setStyleSheet("background-color:#ff3333");

 QWidget* cellAction = new QWidget();
 QHBoxLayout* layout = new QHBoxLayout(cellAction);
 layout->addWidget(updateButton);
 layout->addWidget(deleteButton);
 layout->setContentsMargins(0, 0, 0, 0);
 layout->setSpacing(5);
 ui->listTransaction->setCellWidget(row, 6, cellAction);

 // Delete button functionality
 QObject::connect(deleteButton, &QPushButton::clicked, [=]() {
 // Ask for confirmation before deleting
 QMessageBox::StandardButton reply;
 reply = QMessageBox::question(this, "Confirm Deletion",
 "Are you sure you want to delete this transaction?",
 QMessageBox::Yes | QMessageBox::No);
 if (reply != QMessageBox::Yes) {
 return; // User chose not to delete, exit the function
 }

    if (!db.isOpen()) {
        connectToDatabase();
    }
if (!db.open()) {
 qDebug() << "Error, unable to connect to database";
 qDebug() << db.lastError().text();
 return;
 }

 QSqlQuery query(db);
 query.prepare("DELETE FROM transactions WHERE id = :id");
 query.bindValue(":id", (ui->listTransaction->item(row, 0)->text()).toInt());

 QMessageBox msb;
 msb.setWindowTitle("Delete Transaction State");

 if (!query.exec()) {
 msb.setText("Error deleting transaction: " + query.lastError().text());
 } else {
 msb.setText("Transaction has been deleted successfully.");
 getTransactions();
 }
 db.close();
 msb.exec();
 });

 // Update button functionality
 QObject::connect(updateButton, &QPushButton::clicked, [=]() {
     if(!db.isOpen())
        connectToDatabase();


     if(!db.open())
     {
     qDebug()<<"error,unable to connect ro database";
     qDebug()<<db.lastError().text();
     return;
     }


     QSqlQuery query(db);
     QDate tdate=QDate::fromString(ui->listTransaction->item(row,1)->text(),"yyyy-MM-dd");
     if(!tdate.isValid()){
     qDebug()<< "invalid date format";
     return;
     }
     QString formatedDate=tdate.toString("yyyy-MM-dd");
     query.prepare("UPDATE transactions SET TANSACTION_DATE=to_date(:date,'YYYY-MM-DD'),transaction_type=:type, category=:category, amount=:amount,payment_method=:method WHERE id=:id");
     query.bindValue(":id",(ui->listTransaction->item(row,0)->text()).toInt());


     query.bindValue(":date",formatedDate);
     query.bindValue(":type",ui->listTransaction->item(row,2)->text());
     query.bindValue(":category",ui->listTransaction->item(row,3)->text());
     query.bindValue(":amount",(ui->listTransaction->item(row,4)->text()));
     query.bindValue(":method",ui->listTransaction->item(row,5)->text());
     QMessageBox msb;
     msb.setWindowTitle("update transaction state");

     if(!query.exec())
     {
     msb.setText("error updating transaction "+query.value(1).toString());

     }
     else
     {
     msb.setText(" transaction has been updated successfuly "+query.value(1).toString());
     getTransactions();

     }
     db.close();
     msb.exec();


     });

 row++;
 }
}

QList<int>* MainWindow::getTransactionYears(QVector<Transaction> transactions)
{
 QList<int>* years=new QList<int>();
 for(Transaction t:transactions)
 {
 int y=t.getDate().year();
 if(!years->contains(y))
 years->append(y);
 //qDebug()<<t.getDate().toString();

 }
 return years;

}


Statistics MainWindow::fillStatestics(QVector<Transaction> transactions, int year)
{
    Statistics stat;
          stat.year = year;


          stat.incomes.fill(0, 12);
          stat.expenses.fill(0, 12);
          stat.profitLoss.fill(0, 12);


          for (Transaction t : transactions) {
              if (t.getDate().year() == year) {
                  int monthIndex = t.getDate().month() - 1;
                  if (t.getType() == "income") {
                      stat.incomes[monthIndex] += t.getAmount();
                  } else if (t.getType() == "expense") {
                      stat.expenses[monthIndex] += t.getAmount();
                  }
              }
          }


          for (int i = 0; i < 12; ++i) {
              stat.profitLoss[i] = stat.incomes[i] - stat.expenses[i];
          }




          return stat;

   /* Statistics stat;kent haka
        stat.year = year;

        // Ensure the vectors have 12 elements for each month
        stat.incomes.fill(0, 12);
        stat.expenses.fill(0, 12);
        stat.profitLoss.fill(0, 12);

        // Process the transactions and calculate income/expense
        for (Transaction t : transactions) {
            if (t.getDate().year() == year) {
                int monthIndex = t.getDate().month() - 1; // 0-based index for month
                if (t.getType() == "income") {
                    stat.incomes[monthIndex] += t.getAmount();
                } else if (t.getType() == "expense") {
                    stat.expenses[monthIndex] += t.getAmount();
                }
            }
        }

        // Calculate profit/loss for each month
        for (int i = 0; i < 12; ++i) {
            stat.profitLoss[i] = stat.incomes[i] - stat.expenses[i];
        }

        // Check for any month with a loss and show notification after the window opens
        for (int i = 0; i < 12; ++i) {
            if (stat.profitLoss[i] < 0) {  // If there's a loss
                checkMonthlyLoss(stat.incomes[i], stat.expenses[i]);

                break;  // Show notification only once for the first loss month
            }
        }

        return stat;*/

}

void MainWindow::showNotification(const QString &title, const QString &message, const QString &iconPath) {
    // Check if the system supports tray notifications
   /* if (!QSystemTrayIcon::isSystemTrayAvailable()) {
        QMessageBox::critical(nullptr, "Error", "System tray is not available on this system.");
        return;
    }

    // Create the tray icon
    QSystemTrayIcon* trayIcon = new QSystemTrayIcon();
    trayIcon->setIcon(QIcon(iconPath));
    trayIcon->setVisible(true);  // Make sure the tray icon is visible

    // Debugging: Check if tray icon is visible
    qDebug() << "Tray icon is visible";

    // Display the notification
    trayIcon->showMessage(title, message, QSystemTrayIcon::Information);

    // Clean up: Ensure the tray icon is deleted after a delay
    QTimer::singleShot(5000, trayIcon, &QSystemTrayIcon::deleteLater);  // Auto delete after 5 seconds
}

void MainWindow::checkMonthlyLoss(double totalIncome, double totalExpenses) {
    if (totalIncome < totalExpenses) {
        QString title = "Monthly Loss Detected!";
        QString message = QString("Your hotel incurred a loss this month.\nIncome: $%1\nExpenses: $%2")
                              .arg(totalIncome, 0, 'f', 2)
                              .arg(totalExpenses, 0, 'f', 2);

        // Icon path for loss notification
        QString iconPath = ":/resources/loss_icon.ico";  // Replace with the actual path to your icon
        showNotification(title, message, iconPath);
    } else {
        qDebug() << "No loss detected. Income >= Expenses";
    }*/
}

void MainWindow::on_pushButton_3_clicked()
{

    ui->todoliste->setVisible(true);
    getExpenses();


}


/*void MainWindow::on_start_listening_clicked()
{
    QAudioDevice audioDevice=QMediaDevices::defaultAudioInput();
    QAudioFormat format;
    format.setSampleRate(160000);
    format.setChannelCount(1);
    //format.setSampleSize(16);
    //format.setCodec("audio/pcm");
    format.setSampleFormat(QAudioFormat::Int16);
    if(!audioDevice.isFormatSupported(format))
    {
        qWarning()<<"requested audio format is not supported by  the device";
        return;
    }
    QAudioSource *audioSource=new QAudioSource(audioDevice,format,this);
    QBuffer *audioBuffer=new QBuffer(this);
    audioBuffer->open(QIODevice::WriteOnly);
    audioSource->start(audioBuffer);
    QThread::sleep(5);
    audioSource->stop();
    QByteArray audioData=audioBuffer->buffer();
    audioBuffer->close();
    audioSource->deleteLater();
    startSpeechReconnition(audioData);
}*/


void MainWindow::startSpeechReconnition(const QByteArray &audioData) {
    QString apiKey = "8e3ead1a3344458fb42e0b09289241ff";
    QString url = "https://api.assemblyai.com/v2/transcript";

    QNetworkRequest request(QUrl(url));
  //  request.setRawHeader("Authorization", apiKey.toUtf8());
    //request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject json;
    json["audio_data"] = QString(audioData.toBase64());

   QNetworkReply *reply;// = networkManager->PostOperation(request, QJsonDocument(json).toJson());
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        if (reply->error() == QNetworkReply::NoError) {
            QJsonDocument response = QJsonDocument::fromJson(reply->readAll());
            QString text = response.object().value("text").toString();
            processVoiceCommand(text);
        } else {
            qWarning() << "Speech recognition error:" << reply->errorString();
        }
        reply->deleteLater();
    });
}

void MainWindow::on_start_listening_clicked() {
    QAudioDevice audioDevice = QMediaDevices::defaultAudioInput();
    QAudioFormat format;
    format.setSampleRate(16000);
    format.setChannelCount(1);
    format.setSampleFormat(QAudioFormat::Int16);

    if (!audioDevice.isFormatSupported(format)) {
        qWarning() << "Requested audio format is not supported by the device";
        return;
    }

    QAudioSource *audioSource = new QAudioSource(audioDevice, format, this);
    QBuffer *audioBuffer = new QBuffer(this);
    audioBuffer->open(QIODevice::WriteOnly);
    audioSource->start(audioBuffer);

    QTimer::singleShot(5000, this, [audioSource, audioBuffer, this]() {
        audioSource->stop();
        QByteArray audioData = audioBuffer->buffer();
        audioBuffer->close();
        audioSource->deleteLater();

        startSpeechReconnition(audioData);
    });
}

void MainWindow::processVoiceCommand(const QString &command) {
    QStringList parts = command.split(' ', Qt::SkipEmptyParts);
    if (parts.size() < 2) {
        QMessageBox::warning(this, "Invalid Command", "Command format: <field_name> <value>");
        return;
    }

    QString field = parts[0].toLower();
    QString value = parts.mid(1).join(' ');

    if (field == "amount") ui->amount_transaction->setText(value);
    else if (field == "date") ui->date_transaction->setDate(QDate::fromString(value,"yyyy-MM-dd"));
    else if (field == "category") ui->categoryRecord->setCurrentText(value);
    else if (field == "type") ui->typeRecord->setCurrentText(value);
    else if (command == "store on database") on_pushButton_17_clicked();
    else {
        QMessageBox::warning(this, "Unknown Field", "Field name not recognized: " + field);
    }
}


void MainWindow::onSpeechReconnitionFinished(QNetworkReply*){

}


void MainWindow::on_pushButton_4_clicked()
{
    ui->todoliste->setVisible(false);

}
QVector<Expense> MainWindow::getExpenses()
{
    QVector<Expense> expenses;
    if(!db.isOpen())
        connectToDatabase();
    if(!db.open())
    {
    qDebug()<<"error,unable to connect ro database";
    qDebug()<<db.lastError().text();
    return expenses;
    }


    QSqlQuery query(db);
    query.exec("select category,amount,expenses.* from transactions join expenses  on id=idexpense where transaction_type='expense';");

    QMessageBox msb;
    // msb.setWindowTitle("connection state");
    // msb.setText("connection established "+query.value(1).toString());
    // msb.exec();
    int row=0;
    ui->todoliste->clear();
    ui->todoliste->clearContents();
    ui->todoliste->setRowCount(0);
    QStringList titles;
    titles << " expense id" << "expense title" << "status" << "priority" << "due date" << "budget balance " << "remaining budget" << "notes" << "Actions";

    ui->todoliste->setHorizontalHeaderLabels(titles);

    while(query.next()){
        ui->todoliste->insertRow(row);
        ui->todoliste->setItem(row,0,new QTableWidgetItem(query.value(2).toString()));//expense id
        ui->todoliste->setItem(row,1,new QTableWidgetItem(query.value(0).toString()));//expensetitle(t.category)
        QComboBox* statusComboBox = new QComboBox();
        QStringList statusList = {"waiting", "in progress", "done"};
        statusComboBox->addItems(statusList);

        // Set the current index based on the value from the database
        int statusIndex = statusList.indexOf(query.value(3).toString());
        if (statusIndex != -1) {
            statusComboBox->setCurrentIndex(statusIndex);
        }
        ui->todoliste->setCellWidget(row, 2, statusComboBox);
        ui->todoliste->setItem(row,3,new QTableWidgetItem(query.value(4).toString()));//priority
        ui->todoliste->setItem(row,4,new QTableWidgetItem(query.value(5).toDate().toString("yyyy-MM-dd")));//due_date

        ui->todoliste->setItem(row,5,new QTableWidgetItem(query.value(1).toString()));//budget balance

        ui->todoliste->setItem(row,6,new QTableWidgetItem(query.value(6).toString()));//remaining budget
        ui->todoliste->setItem(row,7,new QTableWidgetItem(query.value(7).toString()));//notes
        Expense t1;
        t1.setTitle(query.value(0).toString());
            t1.setBalance(query.value(1).toDouble());
        t1.setIdexpense(query.value(2).toInt());
          t1.setStatus(query.value(3).toString());
           t1.setPriority(query.value(4).toInt());
        t1.setDue_date(query.value(5).toDate());
        t1.setRemain_budget(query.value(6).toDouble());
        t1.setNote(query.value(7).toString());
        expenses.append(t1);

        ui->todoliste->item(row,1)->setFlags(ui->todoliste->item(row,1)->flags() & ~Qt::ItemIsEditable);
        ui->todoliste->item(row,5)->setFlags(ui->todoliste->item(row,5)->flags() & ~Qt::ItemIsEditable);
        ui->todoliste->item(row,0)->setFlags(ui->todoliste->item(row,0)->flags() & ~Qt::ItemIsEditable);



        QPushButton* updateButton=new QPushButton();
        updateButton->setIcon(QIcon(":/icons/icons/update.png"));

        QPushButton* deleteButton=new QPushButton();
        deleteButton->setIcon(QIcon(":/icons/icons/delete.png"));
        deleteButton->setStyleSheet("background-color:#ff3333");


        QWidget* cellAction=new QWidget();
        QHBoxLayout* layout=new QHBoxLayout(cellAction);
        layout->addWidget(updateButton);
        layout->addWidget(deleteButton);
        layout->setContentsMargins(0,0,0,0);
        layout->setSpacing(5);
        ui->todoliste
                ->setCellWidget(row,8,cellAction);//edit (2 icons)

        QObject::connect(deleteButton, &QPushButton::clicked, [=]() {
        // Ask for confirmation before deleting
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Confirm Deletion",
        "Are you sure you want to delete this transaction?",
        QMessageBox::Yes | QMessageBox::No);
        if (reply != QMessageBox::Yes) {
            return; // User chose not to delete, exit the function
        }

        if (!db.isOpen()) {
            connectToDatabase();
        }
        if (!db.open()) {
            qDebug() << "Error, unable to connect to database";
            qDebug() << db.lastError().text();
            return;
        }

        QSqlQuery query(db);
        query.prepare("DELETE FROM expenses WHERE idexpense = :id");
        query.bindValue(":id", (ui->todoliste->item(row, 0)->text()).toInt());

        QMessageBox msb;
        msb.setWindowTitle("Delete expense State");

        if (!query.exec()) {
        msb.setText("Error deleting expense: " + query.lastError().text());
        } else {
        msb.setText("expense has been deleted successfully.");
        getExpenses();
        }
        db.close();
        msb.exec();
        });

        QObject::connect(updateButton,&QPushButton::clicked,[=]()
        {
        if(!db.isOpen())
            connectToDatabase();


        if(!db.open())
        {
        qDebug()<<"error,unable to connect ro database";
        qDebug()<<db.lastError().text();
        return;
        }


        QSqlQuery query(db);
        QDate tdate=QDate::fromString(ui->todoliste->item(row,4)->text(),"yyyy-MM-dd");
        if(!tdate.isValid()){
        qDebug()<< "invalid date format";
        return;
        }
        QString formatedDate=tdate.toString("yyyy-MM-dd");
        QComboBox* statusComboBox = qobject_cast<QComboBox*>(ui->todoliste->cellWidget(row, 2));
                QString selectedStatus = statusComboBox->currentText();
        query.prepare("UPDATE expenses SET DUE_DATE=to_date(:date,'YYYY-MM-DD'),status=:status, priority=:priority, remain_budget=:amount,notes=:notes WHERE idexpense=:id");
        query.bindValue(":id",(ui->todoliste->item(row,0)->text()).toInt());


        query.bindValue(":date",formatedDate);
        query.bindValue(":status",selectedStatus);
        query.bindValue(":priority",ui->todoliste->item(row,3)->text());
        query.bindValue(":amount",(ui->todoliste->item(row,6)->text()));
        query.bindValue(":notes",ui->todoliste->item(row,7)->text());
        QMessageBox msb;
        msb.setWindowTitle("update expense state");

        if(!query.exec())
        {
        msb.setText("error updating expense "+query.value(1).toString());

        }
        else
        {
        msb.setText(" expense has been updated successfuly "+query.value(1).toString());
        getExpenses();

        }
        db.close();
        msb.exec();


        });

        row++;


    }//fin while
    ui->todoliste->insertRow(row);
    QVector<QString> transactionIds=getTransactionIds();
    QComboBox *idC=new QComboBox();
    idC->addItems(transactionIds.toList());
    ui->todoliste->setCellWidget(row,0,idC);
    addExpense(row);
    QObject::connect(idC,&QComboBox::currentIndexChanged,[=](int index){
       onComboBoxIdValueChanged(row,index,idC->currentText().toInt());
    });
    // ui->listTransaction->resize(850,300);
    //ui->listTransaction->show();

    //sattus comboBox
    QComboBox *statusCombox=new QComboBox();
    QStringList statusList = {"wainting","in progress","done"};
    statusCombox->addItems(statusList.toList());
    ui->todoliste->setCellWidget(row,2,statusCombox);
    return expenses;




}
void MainWindow::onComboBoxIdValueChanged(int row,int index,const int &id)
{
    if(!db.isOpen())
        connectToDatabase();
    if(!db.open())
    {
    qDebug()<<"error,unable to connect ro database";
    qDebug()<<db.lastError().text();
    return;
    }


    QSqlQuery query(db);
    query.prepare("select category,amount from transactions where id=:id");
    query.bindValue(":id",id);
    query.exec();
    query.next();
    ui->todoliste->setItem(row,1,new QTableWidgetItem(query.value(0).toString()));//expensetitle(t.category)

    ui->todoliste->setItem(row,5,new QTableWidgetItem(query.value(1).toString()));//budget balance
    ui->todoliste->item(row,1)->setFlags(ui->todoliste->item(row,1)->flags() & ~Qt::ItemIsEditable);
    ui->todoliste->item(row,5)->setFlags(ui->todoliste->item(row,5)->flags() & ~Qt::ItemIsEditable);


}

void MainWindow::addExpense(int row)
{
    QPushButton* addButton=new QPushButton();
    addButton->setIcon(QIcon(":/icons/icons/add.png"));
    addButton->setStyleSheet("background-color:#55ff55");


    QWidget* cellAction=new QWidget();
    QHBoxLayout* layout=new QHBoxLayout(cellAction);
    layout->addWidget(addButton);
    //layout->setContentsMargins(0,0,0,0);
    //layout->setSpacing(5);
    ui->todoliste->setCellWidget(row,8,cellAction);//edit (2 icons)

    QObject::connect(addButton, &QPushButton::clicked, [=]() {
    // Ask for confirmation before deleting
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirm adding",
    "Are you sure you want to add this expense?",
    QMessageBox::Yes | QMessageBox::No);
    if (reply != QMessageBox::Yes) {
    return; // User chose not to add, exit the function
    }

    if (!db.isOpen()) {
        connectToDatabase();
    }
    if (!db.open()) {
    qDebug() << "Error, unable to connect to database";
    qDebug() << db.lastError().text();
    return;
    }

    QSqlQuery query(db);
    query.prepare("insert into expenses values (:id,:status,:priority,to_date(:date,'YYYY-MM-DD'),:remain_budget,:notes);");
    qDebug() << "request the database.\n";

    QDate tdate=QDate::fromString(ui->todoliste->item(row,4)->text(),"yyyy-MM-dd");
    if(!tdate.isValid()){
    qDebug()<< "invalid date format";
    return;
    }

    QWidget *cellwidgetId=ui->todoliste->cellWidget(row,0);
    QComboBox *idCombo=qobject_cast<QComboBox *>(cellwidgetId);

    QWidget *cellwidgetStatus=ui->todoliste->cellWidget(row,2);
    QComboBox *statusCombo=qobject_cast<QComboBox *>(cellwidgetStatus);

    query.bindValue(":id",(idCombo->currentText()).toInt());
    //qDebug() << "id = "<<(ui->todoliste->item(row, 0)->)<<"\n";

    QString formatedDate=tdate.toString("yyyy-MM-dd");
    query.bindValue(":date",formatedDate);
    query.bindValue(":status",statusCombo->currentText());
    query.bindValue(":priority",ui->todoliste->item(row,3)->text().toInt());
    query.bindValue(":remain_budget",(ui->todoliste->item(row,6)->text()).toDouble());
    query.bindValue(":notes",ui->todoliste->item(row,7)->text());


    QMessageBox msb;
    msb.setWindowTitle("adding expense State");

    if (!query.exec()) {
    msb.setText("Error adding expense: " + query.lastError().text());
    } else {
    msb.setText("expense has been added successfully.");
    getExpenses();
    }
    db.close();
    msb.exec();
    });


}
QVector<QString> MainWindow::getTransactionIds()
{
    QVector<QString> ids;
    if(!db.isOpen())
        connectToDatabase();
    if(!db.open())
    {
    qDebug()<<"error,unable to connect ro database";
    qDebug()<<db.lastError().text();
    return ids;
    }


    QSqlQuery query(db);
    query.exec("select id from transactions where transaction_type='expense';");



    while(query.next()){

    ids.append(query.value(0).toString());
    }
    return ids;
}




