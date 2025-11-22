/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QLabel *label_8;
    QTabWidget *tabWidget;
    QWidget *tab;
    QGroupBox *groupBox;
    QLabel *label;
    QLineEdit *lineEdit;
    QLabel *label_2;
    QLineEdit *lineEdit_2;
    QLabel *label_4;
    QLineEdit *lineEdit_4;
    QLabel *label_5;
    QComboBox *comboBox;
    QPushButton *pushButton;
    QGroupBox *groupBox_2;
    QLabel *label_3;
    QLineEdit *lineEdit_3;
    QPushButton *pushButton_2;
    QGroupBox *groupBox_3;
    QLabel *label_6;
    QComboBox *comboBox_2;
    QLabel *label_7;
    QLineEdit *lineEdit_5;
    QPushButton *pushButton_10;
    QGroupBox *groupBox_4;
    QPushButton *pushButton_4;
    QPushButton *pushButton_6;
    QPushButton *pushButton_5;
    QLabel *label_sort;
    QComboBox *comboBox_3;
    QPushButton *pushButton_9;
    QTableWidget *tableWidget;
    QPushButton *pushButton_8;
    QWidget *tab_2;
    QLabel *label_stats_title;
    QLabel *label_stats_info;
    QWidget *widget;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1200, 750);
        MainWindow->setStyleSheet(QString::fromUtf8("QMainWindow {\n"
"    background: qlineargradient(x1:0, y1:0, x2:1, y2:1,\n"
"        stop:0 #E0F2F1, stop:1 #B2DFDB);\n"
"}\n"
"\n"
"QTabWidget::pane {\n"
"    border: 2px solid #009688;\n"
"    border-radius: 8px;\n"
"    background: white;\n"
"    padding: 5px;\n"
"}\n"
"\n"
"QTabBar::tab {\n"
"    background: qlineargradient(x1:0, y1:0, x2:0, y2:1,\n"
"        stop:0 #B2DFDB, stop:1 #80CBC4);\n"
"    border: 2px solid #009688;\n"
"    padding: 12px 25px;\n"
"    margin-right: 2px;\n"
"    border-top-left-radius: 8px;\n"
"    border-top-right-radius: 8px;\n"
"    color: #004D40;\n"
"    font-weight: bold;\n"
"    font-size: 14px;\n"
"}\n"
"\n"
"QTabBar::tab:selected {\n"
"    background: qlineargradient(x1:0, y1:0, x2:0, y2:1,\n"
"        stop:0 #009688, stop:1 #00796B);\n"
"    color: white;\n"
"    border-bottom: 4px solid #004D40;\n"
"}\n"
"\n"
"QTabBar::tab:hover {\n"
"    background: #00796B;\n"
"    color: white;\n"
"}\n"
"\n"
"QLineEdit {\n"
"    background-color: white;\n"
"    border: 2px solid #B0"
                        "BEC5;\n"
"    border-radius: 6px;\n"
"    padding: 8px;\n"
"    font-size: 13px;\n"
"    selection-background-color: #009688;\n"
"}\n"
"\n"
"QLineEdit:focus {\n"
"    border: 2px solid #009688;\n"
"    background-color: #E0F7FA;\n"
"}\n"
"\n"
"QComboBox {\n"
"    background-color: white;\n"
"    border: 2px solid #B0BEC5;\n"
"    border-radius: 6px;\n"
"    padding: 6px;\n"
"    font-size: 13px;\n"
"}\n"
"\n"
"QComboBox:focus {\n"
"    border: 2px solid #009688;\n"
"}\n"
"\n"
"QComboBox::drop-down {\n"
"    border: none;\n"
"    width: 30px;\n"
"}\n"
"\n"
"QComboBox::down-arrow {\n"
"    image: none;\n"
"    border-left: 5px solid transparent;\n"
"    border-right: 5px solid transparent;\n"
"    border-top: 7px solid #009688;\n"
"}\n"
"\n"
"QComboBox QAbstractItemView {\n"
"    background-color: white;\n"
"    border: 2px solid #009688;\n"
"    selection-background-color: #009688;\n"
"    selection-color: white;\n"
"}\n"
"\n"
"QPushButton {\n"
"    background: qlineargradient(x1:0, y1:0, x2:0, y2:1,\n"
"      "
                        "  stop:0 #009688, stop:1 #00796B);\n"
"    color: white;\n"
"    border: none;\n"
"    border-radius: 8px;\n"
"    padding: 10px 20px;\n"
"    font-weight: bold;\n"
"    font-size: 13px;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background: qlineargradient(x1:0, y1:0, x2:0, y2:1,\n"
"        stop:0 #00796B, stop:1 #004D40);\n"
"}\n"
"\n"
"QPushButton:pressed {\n"
"    background: #004D40;\n"
"    padding-top: 12px;\n"
"    padding-bottom: 8px;\n"
"}\n"
"\n"
"QPushButton#pushButton_5, QPushButton#pushButton_6 {\n"
"    background: qlineargradient(x1:0, y1:0, x2:0, y2:1,\n"
"        stop:0 #F44336, stop:1 #D32F2F);\n"
"}\n"
"\n"
"QPushButton#pushButton_5:hover, QPushButton#pushButton_6:hover {\n"
"    background: qlineargradient(x1:0, y1:0, x2:0, y2:1,\n"
"        stop:0 #D32F2F, stop:1 #B71C1C);\n"
"}\n"
"\n"
"QPushButton#pushButton_8 {\n"
"    background: qlineargradient(x1:0, y1:0, x2:0, y2:1,\n"
"        stop:0 #4CAF50, stop:1 #388E3C);\n"
"}\n"
"\n"
"QPushButton#pushButton_8:hover {\n"
"    background: qli"
                        "neargradient(x1:0, y1:0, x2:0, y2:1,\n"
"        stop:0 #388E3C, stop:1 #2E7D32);\n"
"}\n"
"\n"
"QTableWidget {\n"
"    background-color: white;\n"
"    border: 2px solid #009688;\n"
"    border-radius: 8px;\n"
"    gridline-color: #E0E0E0;\n"
"    font-size: 12px;\n"
"}\n"
"\n"
"QTableWidget::item {\n"
"    padding: 8px;\n"
"}\n"
"\n"
"QTableWidget::item:selected {\n"
"    background-color: #80CBC4;\n"
"    color: #004D40;\n"
"}\n"
"\n"
"QTableWidget::item:hover {\n"
"    background-color: #E0F2F1;\n"
"}\n"
"\n"
"QHeaderView::section {\n"
"    background: qlineargradient(x1:0, y1:0, x2:0, y2:1,\n"
"        stop:0 #009688, stop:1 #00796B);\n"
"    color: white;\n"
"    padding: 10px;\n"
"    border: none;\n"
"    font-weight: bold;\n"
"    font-size: 13px;\n"
"}\n"
"\n"
"QHeaderView::section:hover {\n"
"    background: #00796B;\n"
"}\n"
"\n"
"QLabel {\n"
"    color: #263238;\n"
"    font-weight: bold;\n"
"    font-size: 13px;\n"
"}\n"
"\n"
"QLabel#label_8 {\n"
"    color: #009688;\n"
"    font-size: 26px;\n"
""
                        "    font-weight: bold;\n"
"}"));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        label_8 = new QLabel(centralwidget);
        label_8->setObjectName("label_8");
        label_8->setGeometry(QRect(420, 10, 350, 40));
        label_8->setAlignment(Qt::AlignmentFlag::AlignCenter);
        tabWidget = new QTabWidget(centralwidget);
        tabWidget->setObjectName("tabWidget");
        tabWidget->setGeometry(QRect(20, 20, 1160, 650));
        tab = new QWidget();
        tab->setObjectName("tab");
        groupBox = new QGroupBox(tab);
        groupBox->setObjectName("groupBox");
        groupBox->setGeometry(QRect(20, 20, 350, 250));
        groupBox->setStyleSheet(QString::fromUtf8("QGroupBox {\n"
"    background: qlineargradient(x1:0, y1:0, x2:1, y2:1,\n"
"        stop:0 #E0F2F1, stop:1 #B2DFDB);\n"
"    border: 2px solid #009688;\n"
"    border-radius: 10px;\n"
"    margin-top: 10px;\n"
"    font-weight: bold;\n"
"    padding-top: 15px;\n"
"}\n"
"QGroupBox::title {\n"
"    color: #00796B;\n"
"    subcontrol-origin: margin;\n"
"    left: 15px;\n"
"    padding: 0 5px;\n"
"}"));
        label = new QLabel(groupBox);
        label->setObjectName("label");
        label->setGeometry(QRect(20, 35, 120, 20));
        lineEdit = new QLineEdit(groupBox);
        lineEdit->setObjectName("lineEdit");
        lineEdit->setGeometry(QRect(150, 20, 180, 41));
        label_2 = new QLabel(groupBox);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(20, 70, 120, 20));
        lineEdit_2 = new QLineEdit(groupBox);
        lineEdit_2->setObjectName("lineEdit_2");
        lineEdit_2->setGeometry(QRect(150, 60, 180, 41));
        label_4 = new QLabel(groupBox);
        label_4->setObjectName("label_4");
        label_4->setGeometry(QRect(20, 105, 130, 20));
        lineEdit_4 = new QLineEdit(groupBox);
        lineEdit_4->setObjectName("lineEdit_4");
        lineEdit_4->setGeometry(QRect(150, 99, 180, 41));
        label_5 = new QLabel(groupBox);
        label_5->setObjectName("label_5");
        label_5->setGeometry(QRect(20, 140, 50, 20));
        comboBox = new QComboBox(groupBox);
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->setObjectName("comboBox");
        comboBox->setGeometry(QRect(150, 140, 180, 30));
        pushButton = new QPushButton(groupBox);
        pushButton->setObjectName("pushButton");
        pushButton->setGeometry(QRect(70, 190, 210, 40));
        groupBox_2 = new QGroupBox(tab);
        groupBox_2->setObjectName("groupBox_2");
        groupBox_2->setGeometry(QRect(20, 270, 350, 140));
        groupBox_2->setStyleSheet(QString::fromUtf8("QGroupBox {\n"
"    background: qlineargradient(x1:0, y1:0, x2:1, y2:1,\n"
"        stop:0 #E3F2FD, stop:1 #BBDEFB);\n"
"    border: 2px solid #2196F3;\n"
"    border-radius: 10px;\n"
"    margin-top: 10px;\n"
"    font-weight: bold;\n"
"    padding-top: 15px;\n"
"}\n"
"QGroupBox::title {\n"
"    color: #1976D2;\n"
"    subcontrol-origin: margin;\n"
"    left: 15px;\n"
"    padding: 0 5px;\n"
"}"));
        label_3 = new QLabel(groupBox_2);
        label_3->setObjectName("label_3");
        label_3->setGeometry(QRect(20, 35, 150, 20));
        lineEdit_3 = new QLineEdit(groupBox_2);
        lineEdit_3->setObjectName("lineEdit_3");
        lineEdit_3->setGeometry(QRect(20, 59, 310, 31));
        pushButton_2 = new QPushButton(groupBox_2);
        pushButton_2->setObjectName("pushButton_2");
        pushButton_2->setGeometry(QRect(110, 100, 131, 31));
        groupBox_3 = new QGroupBox(tab);
        groupBox_3->setObjectName("groupBox_3");
        groupBox_3->setGeometry(QRect(20, 410, 350, 160));
        groupBox_3->setStyleSheet(QString::fromUtf8("QGroupBox {\n"
"    background: qlineargradient(x1:0, y1:0, x2:1, y2:1,\n"
"        stop:0 #FFF3E0, stop:1 #FFE0B2);\n"
"    border: 2px solid #FF9800;\n"
"    border-radius: 10px;\n"
"    margin-top: 10px;\n"
"    font-weight: bold;\n"
"    padding-top: 15px;\n"
"}\n"
"QGroupBox::title {\n"
"    color: #F57C00;\n"
"    subcontrol-origin: margin;\n"
"    left: 15px;\n"
"    padding: 0 5px;\n"
"}"));
        label_6 = new QLabel(groupBox_3);
        label_6->setObjectName("label_6");
        label_6->setGeometry(QRect(20, 35, 150, 20));
        comboBox_2 = new QComboBox(groupBox_3);
        comboBox_2->addItem(QString());
        comboBox_2->addItem(QString());
        comboBox_2->addItem(QString());
        comboBox_2->addItem(QString());
        comboBox_2->setObjectName("comboBox_2");
        comboBox_2->setGeometry(QRect(150, 29, 180, 31));
        label_7 = new QLabel(groupBox_3);
        label_7->setObjectName("label_7");
        label_7->setGeometry(QRect(20, 70, 130, 20));
        lineEdit_5 = new QLineEdit(groupBox_3);
        lineEdit_5->setObjectName("lineEdit_5");
        lineEdit_5->setGeometry(QRect(150, 64, 180, 41));
        pushButton_10 = new QPushButton(groupBox_3);
        pushButton_10->setObjectName("pushButton_10");
        pushButton_10->setGeometry(QRect(100, 110, 150, 35));
        groupBox_4 = new QGroupBox(tab);
        groupBox_4->setObjectName("groupBox_4");
        groupBox_4->setGeometry(QRect(390, 20, 730, 430));
        groupBox_4->setStyleSheet(QString::fromUtf8("QGroupBox {\n"
"    background: white;\n"
"    border: 2px solid #009688;\n"
"    border-radius: 10px;\n"
"    margin-top: 10px;\n"
"    font-weight: bold;\n"
"    padding-top: 15px;\n"
"}\n"
"QGroupBox::title {\n"
"    color: #00796B;\n"
"    subcontrol-origin: margin;\n"
"    left: 15px;\n"
"    padding: 0 5px;\n"
"}"));
        pushButton_4 = new QPushButton(groupBox_4);
        pushButton_4->setObjectName("pushButton_4");
        pushButton_4->setGeometry(QRect(20, 30, 161, 35));
        pushButton_6 = new QPushButton(groupBox_4);
        pushButton_6->setObjectName("pushButton_6");
        pushButton_6->setGeometry(QRect(400, 30, 141, 35));
        pushButton_5 = new QPushButton(groupBox_4);
        pushButton_5->setObjectName("pushButton_5");
        pushButton_5->setGeometry(QRect(549, 30, 171, 35));
        label_sort = new QLabel(groupBox_4);
        label_sort->setObjectName("label_sort");
        label_sort->setGeometry(QRect(20, 75, 80, 20));
        comboBox_3 = new QComboBox(groupBox_4);
        comboBox_3->addItem(QString());
        comboBox_3->addItem(QString());
        comboBox_3->setObjectName("comboBox_3");
        comboBox_3->setGeometry(QRect(100, 73, 150, 31));
        pushButton_9 = new QPushButton(groupBox_4);
        pushButton_9->setObjectName("pushButton_9");
        pushButton_9->setGeometry(QRect(260, 70, 80, 30));
        tableWidget = new QTableWidget(groupBox_4);
        if (tableWidget->columnCount() < 5)
            tableWidget->setColumnCount(5);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(4, __qtablewidgetitem4);
        tableWidget->setObjectName("tableWidget");
        tableWidget->setGeometry(QRect(20, 110, 690, 300));
        tableWidget->setAlternatingRowColors(true);
        tableWidget->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);
        tableWidget->setColumnCount(5);
        tableWidget->horizontalHeader()->setVisible(true);
        pushButton_8 = new QPushButton(tab);
        pushButton_8->setObjectName("pushButton_8");
        pushButton_8->setGeometry(QRect(500, 470, 150, 40));
        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName("tab_2");
        label_stats_title = new QLabel(tab_2);
        label_stats_title->setObjectName("label_stats_title");
        label_stats_title->setGeometry(QRect(20, 20, 1100, 40));
        label_stats_title->setStyleSheet(QString::fromUtf8("font-size: 22px; color: #009688; font-weight: bold;"));
        label_stats_title->setAlignment(Qt::AlignmentFlag::AlignCenter);
        label_stats_info = new QLabel(tab_2);
        label_stats_info->setObjectName("label_stats_info");
        label_stats_info->setGeometry(QRect(50, 80, 1050, 500));
        label_stats_info->setStyleSheet(QString::fromUtf8("font-size: 16px;\n"
"color: #37474F;\n"
"background: white;\n"
"border: 2px solid #009688;\n"
"border-radius: 10px;\n"
"padding: 30px;"));
        label_stats_info->setAlignment(Qt::AlignmentFlag::AlignLeading|Qt::AlignmentFlag::AlignLeft|Qt::AlignmentFlag::AlignTop);
        label_stats_info->setWordWrap(true);
        widget = new QWidget(tab_2);
        widget->setObjectName("widget");
        widget->setGeometry(QRect(919, 449, 151, 111));
        widget->setStyleSheet(QString::fromUtf8("QWidget {\n"
"    border-image: url(\"C:/Users/Fethia Braiki/Downloads/logo.jpeg\") 0 0 0 0 stretch stretch;\n"
"}"));
        tabWidget->addTab(tab_2, QString());
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1200, 22));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "Gestion des V\303\251hicules - Syst\303\250me Moderne", nullptr));
        label_8->setText(QCoreApplication::translate("MainWindow", "\360\237\232\227 Gestion des V\303\251hicules", nullptr));
        groupBox->setTitle(QCoreApplication::translate("MainWindow", "\342\236\225 Ajouter un V\303\251hicule", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "Marque:", nullptr));
        lineEdit->setPlaceholderText(QCoreApplication::translate("MainWindow", "Ex: Toyota", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "Matricule:", nullptr));
        lineEdit_2->setPlaceholderText(QCoreApplication::translate("MainWindow", "TUN-1234", nullptr));
        label_4->setText(QCoreApplication::translate("MainWindow", "Kilom\303\251trage (km):", nullptr));
        lineEdit_4->setPlaceholderText(QCoreApplication::translate("MainWindow", "45000", nullptr));
        label_5->setText(QCoreApplication::translate("MainWindow", "\303\211tat:", nullptr));
        comboBox->setItemText(0, QCoreApplication::translate("MainWindow", "bonne etat", nullptr));
        comboBox->setItemText(1, QCoreApplication::translate("MainWindow", "en panne", nullptr));

        pushButton->setText(QCoreApplication::translate("MainWindow", "\342\236\225 Ajouter V\303\251hicule", nullptr));
        groupBox_2->setTitle(QCoreApplication::translate("MainWindow", "\360\237\224\215 Rechercher", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "Marque ou Matricule:", nullptr));
        lineEdit_3->setPlaceholderText(QCoreApplication::translate("MainWindow", "Rechercher...", nullptr));
        pushButton_2->setText(QCoreApplication::translate("MainWindow", "\360\237\224\215 Chercher", nullptr));
        groupBox_3->setTitle(QCoreApplication::translate("MainWindow", "\342\234\217\357\270\217 Modifier un V\303\251hicule", nullptr));
        label_6->setText(QCoreApplication::translate("MainWindow", "Champ \303\240 modifier:", nullptr));
        comboBox_2->setItemText(0, QCoreApplication::translate("MainWindow", "marque", nullptr));
        comboBox_2->setItemText(1, QCoreApplication::translate("MainWindow", "matricule", nullptr));
        comboBox_2->setItemText(2, QCoreApplication::translate("MainWindow", "kilometrage", nullptr));
        comboBox_2->setItemText(3, QCoreApplication::translate("MainWindow", "etat", nullptr));

        label_7->setText(QCoreApplication::translate("MainWindow", "Nouvelle valeur:", nullptr));
        lineEdit_5->setPlaceholderText(QCoreApplication::translate("MainWindow", "Nouvelle valeur", nullptr));
        pushButton_10->setText(QCoreApplication::translate("MainWindow", "\342\234\217\357\270\217 Modifier", nullptr));
        groupBox_4->setTitle(QCoreApplication::translate("MainWindow", "\360\237\223\213 Liste des V\303\251hicules", nullptr));
        pushButton_4->setText(QCoreApplication::translate("MainWindow", "\360\237\221\201\357\270\217 Afficher Tous", nullptr));
        pushButton_6->setText(QCoreApplication::translate("MainWindow", "\360\237\227\221\357\270\217 Supprimer", nullptr));
        pushButton_5->setText(QCoreApplication::translate("MainWindow", "\360\237\227\221\357\270\217 Tout Supprimer", nullptr));
        label_sort->setText(QCoreApplication::translate("MainWindow", "Trier par:", nullptr));
        comboBox_3->setItemText(0, QCoreApplication::translate("MainWindow", "en kilometrage", nullptr));
        comboBox_3->setItemText(1, QCoreApplication::translate("MainWindow", "en etat", nullptr));

        pushButton_9->setText(QCoreApplication::translate("MainWindow", "\360\237\224\204 Trier", nullptr));
        QTableWidgetItem *___qtablewidgetitem = tableWidget->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("MainWindow", "ID", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = tableWidget->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("MainWindow", "Matricule", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = tableWidget->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("MainWindow", "Marque", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = tableWidget->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("MainWindow", "Kilom\303\251trage", nullptr));
        QTableWidgetItem *___qtablewidgetitem4 = tableWidget->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QCoreApplication::translate("MainWindow", "\303\211tat", nullptr));
        pushButton_8->setText(QCoreApplication::translate("MainWindow", "\360\237\223\204 Exporter PDF", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab), QCoreApplication::translate("MainWindow", "\360\237\217\240 Accueil", nullptr));
        label_stats_title->setText(QCoreApplication::translate("MainWindow", "\360\237\223\212 Statistiques ", nullptr));
        label_stats_info->setText(QString());
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QCoreApplication::translate("MainWindow", "\360\237\223\212 Statistiques", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
