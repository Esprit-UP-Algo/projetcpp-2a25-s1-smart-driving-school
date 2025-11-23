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
#include <QtWidgets/QTableView>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QLabel *label_8;
    QTabWidget *tabWidget;
    QWidget *tab;
    QGroupBox *groupBox;
    QLabel *label;
    QLineEdit *lineEdit_2;
    QLabel *label_2;
    QLineEdit *lineEdit_3;
    QLabel *label_4;
    QLineEdit *lineEdit_6;
    QLabel *label_5;
    QPushButton *ajouterButton;
    QLabel *label_9;
    QLineEdit *lineEdit_4;
    QLineEdit *lineEdit_5;
    QGroupBox *groupBox_2;
    QLabel *label_3;
    QLineEdit *lineEdit_searchGlobal;
    QGroupBox *groupBox_3;
    QLabel *label_7;
    QLineEdit *lineEdit_id_modify;
    QPushButton *modifierButton;
    QGroupBox *groupBox_4;
    QLabel *label_sort;
    QComboBox *comboBox_3;
    QPushButton *pushButton_9;
    QTableView *tableView;
    QPushButton *pushButton_8;
    QPushButton *pushButton_7;
    QGroupBox *groupBox_6;
    QLabel *label_10;
    QLineEdit *lineEdit_id_delete;
    QPushButton *supprimerButton;
    QWidget *tab_2;
    QLabel *label_stats_title;
    QLabel *label_stats_info;
    QGroupBox *groupBox_5;
    QPushButton *pushButton_3;
    QPushButton *pushButton_11;
    QPushButton *pushButton_12;
    QPushButton *pushButton_13;
    QPushButton *pushButton_14;
    QWidget *widget;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1485, 739);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName("centralWidget");
        label_8 = new QLabel(centralWidget);
        label_8->setObjectName("label_8");
        label_8->setGeometry(QRect(530, 20, 350, 40));
        label_8->setAlignment(Qt::AlignmentFlag::AlignCenter);
        tabWidget = new QTabWidget(centralWidget);
        tabWidget->setObjectName("tabWidget");
        tabWidget->setGeometry(QRect(250, 10, 1161, 701));
        tab = new QWidget();
        tab->setObjectName("tab");
        groupBox = new QGroupBox(tab);
        groupBox->setObjectName("groupBox");
        groupBox->setGeometry(QRect(10, 20, 350, 281));
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
        lineEdit_2 = new QLineEdit(groupBox);
        lineEdit_2->setObjectName("lineEdit_2");
        lineEdit_2->setGeometry(QRect(150, 30, 180, 31));
        label_2 = new QLabel(groupBox);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(20, 60, 120, 20));
        lineEdit_3 = new QLineEdit(groupBox);
        lineEdit_3->setObjectName("lineEdit_3");
        lineEdit_3->setGeometry(QRect(150, 60, 180, 31));
        label_4 = new QLabel(groupBox);
        label_4->setObjectName("label_4");
        label_4->setGeometry(QRect(20, 90, 130, 20));
        lineEdit_6 = new QLineEdit(groupBox);
        lineEdit_6->setObjectName("lineEdit_6");
        lineEdit_6->setGeometry(QRect(150, 90, 180, 31));
        label_5 = new QLabel(groupBox);
        label_5->setObjectName("label_5");
        label_5->setGeometry(QRect(20, 120, 71, 20));
        ajouterButton = new QPushButton(groupBox);
        ajouterButton->setObjectName("ajouterButton");
        ajouterButton->setGeometry(QRect(70, 230, 210, 40));
        label_9 = new QLabel(groupBox);
        label_9->setObjectName("label_9");
        label_9->setGeometry(QRect(20, 150, 50, 20));
        lineEdit_4 = new QLineEdit(groupBox);
        lineEdit_4->setObjectName("lineEdit_4");
        lineEdit_4->setGeometry(QRect(150, 120, 180, 31));
        lineEdit_5 = new QLineEdit(groupBox);
        lineEdit_5->setObjectName("lineEdit_5");
        lineEdit_5->setGeometry(QRect(150, 150, 180, 31));
        groupBox_2 = new QGroupBox(tab);
        groupBox_2->setObjectName("groupBox_2");
        groupBox_2->setGeometry(QRect(10, 310, 350, 140));
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
        lineEdit_searchGlobal = new QLineEdit(groupBox_2);
        lineEdit_searchGlobal->setObjectName("lineEdit_searchGlobal");
        lineEdit_searchGlobal->setGeometry(QRect(20, 59, 310, 31));
        groupBox_3 = new QGroupBox(tab);
        groupBox_3->setObjectName("groupBox_3");
        groupBox_3->setGeometry(QRect(10, 460, 350, 160));
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
        label_7 = new QLabel(groupBox_3);
        label_7->setObjectName("label_7");
        label_7->setGeometry(QRect(20, 70, 130, 20));
        lineEdit_id_modify = new QLineEdit(groupBox_3);
        lineEdit_id_modify->setObjectName("lineEdit_id_modify");
        lineEdit_id_modify->setGeometry(QRect(150, 64, 180, 41));
        modifierButton = new QPushButton(groupBox_3);
        modifierButton->setObjectName("modifierButton");
        modifierButton->setGeometry(QRect(100, 110, 150, 35));
        groupBox_4 = new QGroupBox(tab);
        groupBox_4->setObjectName("groupBox_4");
        groupBox_4->setGeometry(QRect(370, 0, 751, 451));
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
        tableView = new QTableView(groupBox_4);
        tableView->setObjectName("tableView");
        tableView->setGeometry(QRect(5, 111, 611, 291));
        pushButton_8 = new QPushButton(groupBox_4);
        pushButton_8->setObjectName("pushButton_8");
        pushButton_8->setGeometry(QRect(90, 410, 150, 40));
        pushButton_7 = new QPushButton(tab);
        pushButton_7->setObjectName("pushButton_7");
        pushButton_7->setGeometry(QRect(980, 530, 120, 40));
        pushButton_7->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background: qlineargradient(x1:0, y1:0, x2:0, y2:1,\n"
"        stop:0 #607D8B, stop:1 #455A64);\n"
"}\n"
"QPushButton:hover {\n"
"    background: qlineargradient(x1:0, y1:0, x2:0, y2:1,\n"
"        stop:0 #455A64, stop:1 #263238);\n"
"}"));
        groupBox_6 = new QGroupBox(tab);
        groupBox_6->setObjectName("groupBox_6");
        groupBox_6->setGeometry(QRect(380, 460, 350, 160));
        groupBox_6->setStyleSheet(QString::fromUtf8("QGroupBox {\n"
"    background-color: rgb(255, 85, 0);  /* Rouge clair (fond) */\n"
"    background: qlineargradient(x1:0, y1:0, x2:1, y2:1,\n"
"        stop:0 #FFCDD2, stop:1 #F44336); /* D\303\251grad\303\251 du rose clair vers le rouge */\n"
"    border: 2px solid #D32F2F;  /* Bordure rouge fonc\303\251 */\n"
"    border-radius: 10px;\n"
"    margin-top: 10px;\n"
"    font-weight: bold;\n"
"    padding-top: 15px;\n"
"}\n"
"\n"
"QGroupBox::title {\n"
"    color: #D32F2F;  /* Rouge fonc\303\251 pour le titre */\n"
"    subcontrol-origin: margin;\n"
"    left: 15px;\n"
"    padding: 0 5px;\n"
"}\n"
""));
        label_10 = new QLabel(groupBox_6);
        label_10->setObjectName("label_10");
        label_10->setGeometry(QRect(20, 70, 130, 20));
        lineEdit_id_delete = new QLineEdit(groupBox_6);
        lineEdit_id_delete->setObjectName("lineEdit_id_delete");
        lineEdit_id_delete->setGeometry(QRect(120, 60, 180, 41));
        supprimerButton = new QPushButton(groupBox_6);
        supprimerButton->setObjectName("supprimerButton");
        supprimerButton->setGeometry(QRect(100, 110, 150, 35));
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
        tabWidget->addTab(tab_2, QString());
        groupBox_5 = new QGroupBox(centralWidget);
        groupBox_5->setObjectName("groupBox_5");
        groupBox_5->setGeometry(QRect(40, 60, 211, 651));
        pushButton_3 = new QPushButton(groupBox_5);
        pushButton_3->setObjectName("pushButton_3");
        pushButton_3->setGeometry(QRect(30, 60, 151, 51));
        pushButton_11 = new QPushButton(groupBox_5);
        pushButton_11->setObjectName("pushButton_11");
        pushButton_11->setGeometry(QRect(30, 160, 151, 51));
        pushButton_12 = new QPushButton(groupBox_5);
        pushButton_12->setObjectName("pushButton_12");
        pushButton_12->setGeometry(QRect(30, 260, 151, 51));
        pushButton_13 = new QPushButton(groupBox_5);
        pushButton_13->setObjectName("pushButton_13");
        pushButton_13->setGeometry(QRect(30, 370, 151, 51));
        pushButton_14 = new QPushButton(groupBox_5);
        pushButton_14->setObjectName("pushButton_14");
        pushButton_14->setGeometry(QRect(30, 480, 151, 51));
        widget = new QWidget(groupBox_5);
        widget->setObjectName("widget");
        widget->setGeometry(QRect(0, 550, 101, 91));
        widget->setStyleSheet(QString::fromUtf8("QWidget {\n"
"    border-image: url(\"logo.JPG\") 0 0 0 0 stretch;\n"
"}"));
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName("menuBar");
        menuBar->setGeometry(QRect(0, 0, 1485, 26));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName("mainToolBar");
        MainWindow->addToolBar(Qt::ToolBarArea::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName("statusBar");
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "Gestion des Clients", nullptr));
        label_8->setText(QCoreApplication::translate("MainWindow", "Gestion condidat", nullptr));
        groupBox->setTitle(QCoreApplication::translate("MainWindow", "\342\236\225 Ajouter condidat", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "Nom:", nullptr));
        lineEdit_2->setText(QString());
        lineEdit_2->setPlaceholderText(QCoreApplication::translate("MainWindow", "Ex: Toyota Corolla", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "pr\303\251nom:", nullptr));
        lineEdit_3->setText(QCoreApplication::translate("MainWindow", "..", nullptr));
        lineEdit_3->setPlaceholderText(QCoreApplication::translate("MainWindow", "TUN-1234", nullptr));
        label_4->setText(QCoreApplication::translate("MainWindow", "CIN", nullptr));
        lineEdit_6->setText(QCoreApplication::translate("MainWindow", "..", nullptr));
        lineEdit_6->setPlaceholderText(QCoreApplication::translate("MainWindow", "45000", nullptr));
        label_5->setText(QCoreApplication::translate("MainWindow", "T\303\251l\303\251phone", nullptr));
        ajouterButton->setText(QCoreApplication::translate("MainWindow", "\342\236\225 Ajouter Condidat", nullptr));
        label_9->setText(QCoreApplication::translate("MainWindow", "Mail", nullptr));
        lineEdit_4->setText(QCoreApplication::translate("MainWindow", "..", nullptr));
        lineEdit_4->setPlaceholderText(QCoreApplication::translate("MainWindow", "45000", nullptr));
        lineEdit_5->setText(QCoreApplication::translate("MainWindow", "..", nullptr));
        lineEdit_5->setPlaceholderText(QCoreApplication::translate("MainWindow", "45000", nullptr));
        groupBox_2->setTitle(QCoreApplication::translate("MainWindow", "\360\237\224\215 Rechercher", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "CIN", nullptr));
        lineEdit_searchGlobal->setPlaceholderText(QCoreApplication::translate("MainWindow", "Rechercher...", nullptr));
        groupBox_3->setTitle(QCoreApplication::translate("MainWindow", "\342\234\217\357\270\217 Modifier condidat", nullptr));
        label_7->setText(QCoreApplication::translate("MainWindow", "Modification", nullptr));
        lineEdit_id_modify->setPlaceholderText(QCoreApplication::translate("MainWindow", "Nouvelle valeur", nullptr));
        modifierButton->setText(QCoreApplication::translate("MainWindow", "\342\234\217\357\270\217 Modifier", nullptr));
        groupBox_4->setTitle(QCoreApplication::translate("MainWindow", "\360\237\223\213 Liste des Condidats", nullptr));
        label_sort->setText(QCoreApplication::translate("MainWindow", "Trier par:", nullptr));
        comboBox_3->setItemText(0, QCoreApplication::translate("MainWindow", "Date d'inscriotion", nullptr));
        comboBox_3->setItemText(1, QCoreApplication::translate("MainWindow", "Alphabet", nullptr));

        pushButton_9->setText(QCoreApplication::translate("MainWindow", "\360\237\224\204 Trier", nullptr));
        pushButton_8->setText(QCoreApplication::translate("MainWindow", "\360\237\223\204 Exporter PDF", nullptr));
        pushButton_7->setText(QCoreApplication::translate("MainWindow", "\342\235\214 Quitter", nullptr));
        groupBox_6->setTitle(QCoreApplication::translate("MainWindow", "Supprimer condidat", nullptr));
        label_10->setText(QCoreApplication::translate("MainWindow", "Supprimer", nullptr));
        lineEdit_id_delete->setPlaceholderText(QCoreApplication::translate("MainWindow", "Nouvelle valeur", nullptr));
        supprimerButton->setText(QCoreApplication::translate("MainWindow", "Supprimer", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab), QCoreApplication::translate("MainWindow", "\360\237\217\240 Accueil", nullptr));
        label_stats_title->setText(QCoreApplication::translate("MainWindow", "\360\237\223\212 Statistiques ", nullptr));
        label_stats_info->setText(QString());
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QCoreApplication::translate("MainWindow", "\360\237\223\212 Statistiques", nullptr));
        groupBox_5->setTitle(QCoreApplication::translate("MainWindow", "Menu", nullptr));
        pushButton_3->setText(QCoreApplication::translate("MainWindow", "condidat", nullptr));
        pushButton_11->setText(QCoreApplication::translate("MainWindow", "moniteur", nullptr));
        pushButton_12->setText(QCoreApplication::translate("MainWindow", "v\303\251hicule", nullptr));
        pushButton_13->setText(QCoreApplication::translate("MainWindow", "finance", nullptr));
        pushButton_14->setText(QCoreApplication::translate("MainWindow", "PushButton", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
