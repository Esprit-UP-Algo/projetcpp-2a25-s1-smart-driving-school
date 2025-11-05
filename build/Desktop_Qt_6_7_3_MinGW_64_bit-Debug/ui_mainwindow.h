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
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDateEdit>
#include <QtWidgets/QFrame>
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
    QTabWidget *tabWidget;
    QWidget *tab;
    QWidget *widget;
    QGroupBox *groupBox_add;
    QLabel *leType;
    QComboBox *comboBox_3;
    QLabel *leLieu;
    QLineEdit *lineEdit_3;
    QLabel *deDate;
    QDateEdit *dateEdit;
    QLabel *cbResult;
    QComboBox *comboBox_6;
    QPushButton *addButton;
    QLabel *leType_2;
    QLineEdit *lineEdit_5;
    QGroupBox *groupBox_search;
    QTableWidget *tableExams;
    QPushButton *exportButton;
    QPushButton *deleteButton;
    QLabel *label_4;
    QComboBox *comboBox_5;
    QPushButton *exportButton_2;
    QGroupBox *groupBox_modify;
    QPushButton *btnSearch;
    QLineEdit *lineEdit_6;
    QLabel *label;
    QGroupBox *groupBox_list;
    QPushButton *modifyButton;
    QLabel *label_2;
    QComboBox *comboBox_4;
    QLineEdit *lineEdit_7;
    QLabel *label_5;
    QWidget *tab_2;
    QGroupBox *groupBox_search_2;
    QWidget *widget_2;
    QFrame *sideMenu;
    QPushButton *btnExamens;
    QPushButton *btnFinance;
    QPushButton *btnMoniteur;
    QPushButton *btnVehicule;
    QPushButton *btnAccueil;
    QPushButton *btnCandidat;
    QLabel *label_3;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1349, 729);
        MainWindow->setStyleSheet(QString::fromUtf8("/* === Fen\303\252tre principale === */\n"
"QMainWindow, QDialog {\n"
"    background-color: #f5f7f4;\n"
"}\n"
"\n"
"/* === Boutons lat\303\251raux === */\n"
"QPushButton {\n"
"    background-color: #3c7a6d;\n"
"    color: white;\n"
"    border-radius: 10px;\n"
"    font-weight: bold;\n"
"    font-size: 14px;\n"
"    padding: 6px;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: #2f665a;\n"
"}\n"
"\n"
"QPushButton:pressed {\n"
"    background-color: #28584e;\n"
"}\n"
"\n"
"/* === GroupBox === */\n"
"QGroupBox {\n"
"    background-color: #e8f0ed;\n"
"    border: 2px solid #3c7a6d;\n"
"    border-radius: 10px;\n"
"    margin-top: 10px;\n"
"    font-weight: bold;\n"
"    padding: 10px;\n"
"}\n"
"\n"
"/* === Labels === */\n"
"QLabel {\n"
"    color: #1f4d42;\n"
"    font-weight: bold;\n"
"}\n"
"\n"
"/* === QLineEdit, QComboBox === */\n"
"QLineEdit, QComboBox {\n"
"    border: 1px solid #3c7a6d;\n"
"    border-radius: 6px;\n"
"    padding: 4px 6px;\n"
"    background-color: white;\n"
"    color: #1f4d42"
                        ";\n"
"}\n"
"\n"
"/* === Table === */\n"
"QTableWidget {\n"
"    background-color: white;\n"
"    gridline-color: #3c7a6d;\n"
"    border: 1px solid #3c7a6d;\n"
"    border-radius: 8px;\n"
"}\n"
"\n"
"QHeaderView::section {\n"
"    background-color: #3c7a6d;\n"
"    color: white;\n"
"    font-weight: bold;\n"
"    padding: 5px;\n"
"    border: none;\n"
"}\n"
"\n"
"/* === Onglets === */\n"
"QTabWidget::pane {\n"
"    border: 2px solid #3c7a6d;\n"
"    border-radius: 10px;\n"
"}\n"
"\n"
"QTabBar::tab {\n"
"    background: #dbe7e3;\n"
"    color: #1f4d42;\n"
"    padding: 6px 12px;\n"
"    border-top-left-radius: 8px;\n"
"    border-top-right-radius: 8px;\n"
"}\n"
"\n"
"QTabBar::tab:selected {\n"
"    background: #3c7a6d;\n"
"    color: white;\n"
"}\n"
""));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        tabWidget = new QTabWidget(centralwidget);
        tabWidget->setObjectName("tabWidget");
        tabWidget->setGeometry(QRect(260, 61, 1081, 901));
        tabWidget->setStyleSheet(QString::fromUtf8("/* === Palette et base === */\n"
"QMainWindow, QWidget {\n"
"    background-color: #F3F1E7;\n"
"    font-family: \"Poppins\", \"Segoe UI\", sans-serif;\n"
"    color: #2E3A59;\n"
"}\n"
"\n"
"/* Side menu */\n"
"#sideMenu {\n"
"    background-color: #1E4D2B;\n"
"}\n"
"#sideMenu QPushButton {\n"
"    background: transparent;\n"
"    color: #F3F1E7;\n"
"    border: none;\n"
"    padding: 12px;\n"
"    text-align: left;\n"
"    font-weight: 600;\n"
"}\n"
"#sideMenu QPushButton:hover { background-color: #2E3A59; border-radius: 6px; }\n"
"#sideMenu QPushButton:checked { background-color: #F3F1E7; color: #1E4D2B; border-radius: 6px; }\n"
"\n"
"/* Title */\n"
"QLabel#titleLabel {\n"
"    font: bold 20pt \"Poppins\";\n"
"    color: #1E4D2B;\n"
"    margin-bottom: 8px;\n"
"}\n"
"\n"
"/* GroupBoxes (base) */\n"
"QGroupBox {\n"
"    background-color: #FFFFFF;\n"
"    border: 2px solid #E2DCC5;\n"
"    border-radius: 8px;\n"
"    margin-top: 10px;\n"
"    padding: 12px;\n"
"}\n"
"QGroupBox::title { color: #1E4D2B; font-wei"
                        "ght: bold; }\n"
"\n"
"/* Specific colors per module */\n"
"QGroupBox#groupBox_add {\n"
"    border-color: #CFE8DC;\n"
"    background: qlineargradient(x1:0,y1:0,x2:1,y2:1, stop:0 #E9F7F0, stop:1 #DFF3E8);\n"
"}\n"
"QGroupBox#groupBox_search {\n"
"    border-color: #D6EAF8;\n"
"    background: qlineargradient(x1:0,y1:0,x2:1,y2:1, stop:0 #EEF7FB, stop:1 #E3F2FD);\n"
"}\n"
"QGroupBox#groupBox_modify {\n"
"    border-color: #FFE7CD;\n"
"    background: qlineargradient(x1:0,y1:0,x2:1,y2:1, stop:0 #FFF6EB, stop:1 #FFF0DB);\n"
"}\n"
"QGroupBox#groupBox_list {\n"
"    border-color: #CFE8DC;\n"
"    background: white;\n"
"}\n"
"\n"
"/* Form fields */\n"
"QLineEdit, QDateEdit, QComboBox {\n"
"    background: white;\n"
"    border: 1px solid #C5C5C5;\n"
"    border-radius: 6px;\n"
"    padding: 6px;\n"
"}\n"
"QLineEdit:focus, QDateEdit:focus, QComboBox:focus {\n"
"    border: 2px solid #1E4D2B;\n"
"}\n"
"\n"
"/* Buttons */\n"
"QPushButton {\n"
"    background-color: #2E3A59;\n"
"    color: #F3F1E7;\n"
"    border-radius:"
                        " 8px;\n"
"    padding: 8px 14px;\n"
"    font-weight: 600;\n"
"}\n"
"QPushButton:hover { background-color: #1E4D2B; }\n"
"\n"
"QPushButton#addButton { background-color: #1E4D2B; }\n"
"QPushButton#addButton:hover { background-color: #2E3A59; }\n"
"\n"
"QPushButton#btnSearch { background-color: #2E3A59; }\n"
"QPushButton#modifyButton { background-color: #F57C00; }\n"
"QPushButton#deleteButton { background-color: #C62828; }\n"
"QPushButton#exportButton { background-color: #166534; }\n"
"QPushButton#quitButton { background-color: #455A64; }\n"
"\n"
"/* Table */\n"
"QTableWidget {\n"
"    background: white;\n"
"    border: 1px solid #D7E6DE;\n"
"    border-radius: 6px;\n"
"    gridline-color: #EAEAEA;\n"
"}\n"
"QHeaderView::section {\n"
"    background-color: #1E4D2B;\n"
"    color: white;\n"
"    padding: 6px;\n"
"    font-weight: bold;\n"
"}\n"
""));
        tabWidget->setTabPosition(QTabWidget::TabPosition::North);
        tabWidget->setTabShape(QTabWidget::TabShape::Rounded);
        tab = new QWidget();
        tab->setObjectName("tab");
        widget = new QWidget(tab);
        widget->setObjectName("widget");
        widget->setGeometry(QRect(0, 0, 1071, 581));
        groupBox_add = new QGroupBox(widget);
        groupBox_add->setObjectName("groupBox_add");
        groupBox_add->setGeometry(QRect(80, 0, 841, 271));
        leType = new QLabel(groupBox_add);
        leType->setObjectName("leType");
        leType->setGeometry(QRect(10, 50, 131, 40));
        leType->setStyleSheet(QString::fromUtf8("QLabel {\n"
"    /* normal state */\n"
"	background-color:rgb(227, 245, 235);\n"
"    color: rgb(41, 127, 110);\n"
"    border: none;\n"
"    border-radius: 8px;\n"
"    padding: 8px 20px;\n"
"    font-size: 18px;\n"
"    font-weight: bold;\n"
"}"));
        comboBox_3 = new QComboBox(groupBox_add);
        comboBox_3->addItem(QString());
        comboBox_3->addItem(QString());
        comboBox_3->setObjectName("comboBox_3");
        comboBox_3->setGeometry(QRect(110, 50, 201, 41));
        comboBox_3->setStyleSheet(QString::fromUtf8("QComboBox {\n"
"    background-color: white;                  \n"
"    color: black;                            \n"
"    border: 1px solid rgb(41,127,110);     \n"
"    border-radius: 6px;                    \n"
"    padding: 4px 8px;             \n"
"    font-size: 14px;\n"
"}\n"
"\n"
"QComboBox:focus {\n"
"    border: 2px solid rgb(41,127,110);       \n"
"    background-color: rgb(250, 250, 250);    \n"
"}\n"
"\n"
"QComboBox::drop-down {\n"
"    border: none;                            \n"
"    width: 24px;\n"
"}\n"
""));
        comboBox_3->setEditable(false);
        leLieu = new QLabel(groupBox_add);
        leLieu->setObjectName("leLieu");
        leLieu->setGeometry(QRect(10, 100, 91, 40));
        leLieu->setStyleSheet(QString::fromUtf8("QLabel {\n"
"    /* normal state */\n"
"	background-color:rgb(227, 245, 235);\n"
"    color: rgb(41, 127, 110);\n"
"    border: none;\n"
"    border-radius: 8px;\n"
"    padding: 8px 20px;\n"
"    font-size: 18px;\n"
"    font-weight: bold;\n"
"}"));
        lineEdit_3 = new QLineEdit(groupBox_add);
        lineEdit_3->setObjectName("lineEdit_3");
        lineEdit_3->setGeometry(QRect(110, 100, 201, 41));
        lineEdit_3->setStyleSheet(QString::fromUtf8("QLineEdit {\n"
"    background-color: white;   /* fond blanc */\n"
"    color: black;              /* texte noir */\n"
"    border: 1px solid rgb(41,127,110);  /* contour vert */\n"
"    border-radius: 6px;        /* coins arrondis */\n"
"    padding: 4px 8px;          /* marges internes */\n"
"    font-size: 14px;\n"
"}\n"
"\n"
"QLineEdit:focus {\n"
"    border: 2px solid rgb(41,127,110); /* contour plus \303\251pais quand actif */\n"
"    background-color: rgb(250, 250, 250); /* tr\303\250s l\303\251ger gris pour focus */\n"
"}\n"
""));
        deDate = new QLabel(groupBox_add);
        deDate->setObjectName("deDate");
        deDate->setGeometry(QRect(440, 50, 121, 40));
        deDate->setStyleSheet(QString::fromUtf8("QLabel {\n"
"    /* normal state */\n"
"	background-color:rgb(227, 245, 235);\n"
"    color: rgb(41, 127, 110);\n"
"    border: none;\n"
"    border-radius: 8px;\n"
"    padding: 8px 20px;\n"
"    font-size: 18px;\n"
"    font-weight: bold;\n"
"}"));
        dateEdit = new QDateEdit(groupBox_add);
        dateEdit->setObjectName("dateEdit");
        dateEdit->setGeometry(QRect(580, 50, 151, 41));
        dateEdit->setStyleSheet(QString::fromUtf8("QDateEdit {\n"
"    background-color: white;                  \n"
"    color: black;                            \n"
"    border: 1px solid rgb(41,127,110);     \n"
"    border-radius: 6px;                    \n"
"    padding: 4px 8px;             \n"
"    font-size: 14px;\n"
"}\n"
"\n"
"QDateEdit :focus {\n"
"    border: 2px solid rgb(41,127,110);       \n"
"    background-color: rgb(250, 250, 250);    \n"
"}\n"
"\n"
"QDateEdit ::drop-down {\n"
"    border: none;                            \n"
"    width: 24px;\n"
"}\n"
""));
        cbResult = new QLabel(groupBox_add);
        cbResult->setObjectName("cbResult");
        cbResult->setGeometry(QRect(440, 100, 141, 40));
        cbResult->setStyleSheet(QString::fromUtf8("QLabel {\n"
"    /* normal state */\n"
"	background-color:rgb(227, 245, 235);\n"
"    color: rgb(41, 127, 110);\n"
"    border: none;\n"
"    border-radius: 8px;\n"
"    padding: 8px 20px;\n"
"    font-size: 18px;\n"
"    font-weight: bold;\n"
"}\n"
""));
        comboBox_6 = new QComboBox(groupBox_add);
        comboBox_6->addItem(QString());
        comboBox_6->addItem(QString());
        comboBox_6->setObjectName("comboBox_6");
        comboBox_6->setGeometry(QRect(580, 100, 151, 41));
        comboBox_6->setStyleSheet(QString::fromUtf8("QComboBox {\n"
"    background-color: white;                  \n"
"    color: black;                            \n"
"    border: 1px solid rgb(41,127,110);     \n"
"    border-radius: 6px;                    \n"
"    padding: 4px 8px;             \n"
"    font-size: 14px;\n"
"}\n"
"\n"
"QComboBox:focus {\n"
"    border: 2px solid rgb(41,127,110);       \n"
"    background-color: rgb(250, 250, 250);    \n"
"}\n"
"\n"
"QComboBox::drop-down {\n"
"    border: none;                            \n"
"    width: 24px;\n"
"}\n"
""));
        addButton = new QPushButton(groupBox_add);
        addButton->setObjectName("addButton");
        addButton->setGeometry(QRect(620, 220, 181, 41));
        addButton->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: rgb(41, 127, 110); /* vert par d\303\251faut */\n"
"    color: white; /* texte blanc */\n"
"    border: none;\n"
"    border-radius: 8px;\n"
"    padding: 8px 20px;\n"
"    font-size: 14px;\n"
"    font-weight: bold;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: rgb(209, 209, 209); /* gris clair en hover */\n"
"    color: rgb(41, 127, 110); /* texte vert */\n"
"}\n"
""));
        QIcon icon(QIcon::fromTheme(QIcon::ThemeIcon::ListAdd));
        addButton->setIcon(icon);
        leType_2 = new QLabel(groupBox_add);
        leType_2->setObjectName("leType_2");
        leType_2->setGeometry(QRect(190, 160, 171, 40));
        leType_2->setStyleSheet(QString::fromUtf8("QLabel {\n"
"    /* normal state */\n"
"	background-color:rgb(227, 245, 235);\n"
"    color: rgb(41, 127, 110);\n"
"    border: none;\n"
"    border-radius: 8px;\n"
"    padding: 8px 20px;\n"
"    font-size: 18px;\n"
"    font-weight: bold;\n"
"}"));
        lineEdit_5 = new QLineEdit(groupBox_add);
        lineEdit_5->setObjectName("lineEdit_5");
        lineEdit_5->setGeometry(QRect(380, 160, 231, 41));
        lineEdit_5->setStyleSheet(QString::fromUtf8("QLineEdit {\n"
"    background-color: white;   /* fond blanc */\n"
"    color: black;              /* texte noir */\n"
"    border: 1px solid rgb(41,127,110);  /* contour vert */\n"
"    border-radius: 6px;        /* coins arrondis */\n"
"    padding: 4px 8px;          /* marges internes */\n"
"    font-size: 14px;\n"
"}\n"
"\n"
"QLineEdit:focus {\n"
"    border: 2px solid rgb(41,127,110); /* contour plus \303\251pais quand actif */\n"
"    background-color: rgb(250, 250, 250); /* tr\303\250s l\303\251ger gris pour focus */\n"
"}\n"
""));
        groupBox_search = new QGroupBox(widget);
        groupBox_search->setObjectName("groupBox_search");
        groupBox_search->setGeometry(QRect(370, 270, 701, 311));
        tableExams = new QTableWidget(groupBox_search);
        if (tableExams->columnCount() < 5)
            tableExams->setColumnCount(5);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableExams->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableExams->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tableExams->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        tableExams->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        tableExams->setHorizontalHeaderItem(4, __qtablewidgetitem4);
        if (tableExams->rowCount() < 5)
            tableExams->setRowCount(5);
        tableExams->setObjectName("tableExams");
        tableExams->setGeometry(QRect(10, 30, 681, 191));
        tableExams->setStyleSheet(QString::fromUtf8("QTableWidget {\n"
"    background-color: white;                 \n"
"    color: rgb(41,127,110);                  /* texte en vert */\n"
"    border: 1px solid rgb(41,127,110);     \n"
"    border-radius: 6px;                    \n"
"    padding: 4px 8px;             \n"
"    font-size: 14px;\n"
"}\n"
"\n"
"QTableWidget:focus {\n"
"    border: 2px solid rgb(41,127,110);       \n"
"    background-color: rgb(250, 250, 250);    \n"
"}\n"
"\n"
"QHeaderView::section {\n"
"    background-color: rgb(41,127,110);       /* ent\303\252te vert */\n"
"    color: white;                            /* texte blanc dans l'ent\303\252te */\n"
"    padding: 4px;\n"
"    border: none;\n"
"}\n"
""));
        exportButton = new QPushButton(groupBox_search);
        exportButton->setObjectName("exportButton");
        exportButton->setGeometry(QRect(370, 270, 271, 35));
        exportButton->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: rgb(41, 127, 110); /* vert par d\303\251faut */\n"
"    color: white; /* texte blanc */\n"
"    border: none;\n"
"    border-radius: 8px;\n"
"    padding: 8px 20px;\n"
"    font-size: 14px;\n"
"    font-weight: bold;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: rgb(209, 209, 209); /* gris clair en hover */\n"
"    color: rgb(41, 127, 110); /* texte vert */\n"
"}\n"
""));
        QIcon icon1(QIcon::fromTheme(QIcon::ThemeIcon::DocumentPageSetup));
        exportButton->setIcon(icon1);
        deleteButton = new QPushButton(groupBox_search);
        deleteButton->setObjectName("deleteButton");
        deleteButton->setGeometry(QRect(440, 230, 132, 35));
        deleteButton->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: rgb(170, 0, 0); /* vert par d\303\251faut */\n"
"    color: white; /* texte blanc */\n"
"    border: none;\n"
"    border-radius: 8px;\n"
"    padding: 8px 20px;\n"
"    font-size: 14px;\n"
"    font-weight: bold;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: rgb(209, 209, 209); /* gris clair en hover */\n"
"    color: rgb(41, 127, 110); /* texte vert */\n"
"}\n"
""));
        QIcon icon2(QIcon::fromTheme(QIcon::ThemeIcon::EditDelete));
        deleteButton->setIcon(icon2);
        label_4 = new QLabel(groupBox_search);
        label_4->setObjectName("label_4");
        label_4->setGeometry(QRect(20, 240, 141, 20));
        label_4->setStyleSheet(QString::fromUtf8("QLabel {\n"
"    /* normal state */\n"
"	background-color:rgb(229, 243, 253);\n"
"    color:rgb(46, 58, 114);\n"
"    border: none;\n"
"    border-radius: 1px;\n"
"    padding: 2px 2px;\n"
"    font-size: 14px;\n"
"    font-weight: bold;\n"
"}\n"
""));
        comboBox_5 = new QComboBox(groupBox_search);
        comboBox_5->addItem(QString());
        comboBox_5->addItem(QString());
        comboBox_5->setObjectName("comboBox_5");
        comboBox_5->setGeometry(QRect(130, 230, 121, 41));
        comboBox_5->setStyleSheet(QString::fromUtf8("QComboBox {\n"
"    background-color: white;                  \n"
"    color: black;                            \n"
"    border: 1px solid rgb(41,127,110);     \n"
"    border-radius: 6px;                    \n"
"    padding: 4px 8px;             \n"
"    font-size: 14px;\n"
"}\n"
"\n"
"QComboBox:focus {\n"
"    border: 2px solid rgb(41,127,110);       \n"
"    background-color: rgb(250, 250, 250);    \n"
"}\n"
"\n"
"QComboBox::drop-down {\n"
"    border: none;                            \n"
"    width: 24px;\n"
"}\n"
""));
        comboBox_5->setEditable(false);
        exportButton_2 = new QPushButton(groupBox_search);
        exportButton_2->setObjectName("exportButton_2");
        exportButton_2->setGeometry(QRect(30, 270, 91, 35));
        exportButton_2->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: rgb(41, 127, 110); /* vert par d\303\251faut */\n"
"    color: white; /* texte blanc */\n"
"    border: none;\n"
"    border-radius: 8px;\n"
"    padding: 8px 20px;\n"
"    font-size: 14px;\n"
"    font-weight: bold;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: rgb(209, 209, 209); /* gris clair en hover */\n"
"    color: rgb(41, 127, 110); /* texte vert */\n"
"}\n"
""));
        QIcon icon3(QIcon::fromTheme(QIcon::ThemeIcon::MediaPlaylistRepeat));
        exportButton_2->setIcon(icon3);
        groupBox_modify = new QGroupBox(widget);
        groupBox_modify->setObjectName("groupBox_modify");
        groupBox_modify->setGeometry(QRect(0, 280, 361, 141));
        btnSearch = new QPushButton(groupBox_modify);
        btnSearch->setObjectName("btnSearch");
        btnSearch->setGeometry(QRect(10, 100, 171, 35));
        btnSearch->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: rgb(41, 127, 110); /* vert par d\303\251faut */\n"
"    color: white; /* texte blanc */\n"
"    border: none;\n"
"    border-radius: 8px;\n"
"    padding: 8px 20px;\n"
"    font-size: 14px;\n"
"    font-weight: bold;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: rgb(209, 209, 209); /* gris clair en hover */\n"
"    color: rgb(41, 127, 110); /* texte vert */\n"
"}\n"
""));
        QIcon icon4(QIcon::fromTheme(QIcon::ThemeIcon::SystemSearch));
        btnSearch->setIcon(icon4);
        lineEdit_6 = new QLineEdit(groupBox_modify);
        lineEdit_6->setObjectName("lineEdit_6");
        lineEdit_6->setGeometry(QRect(10, 60, 261, 31));
        lineEdit_6->setStyleSheet(QString::fromUtf8("QLineEdit {\n"
"    background-color: white;   /* fond blanc */\n"
"    color: black;              /* texte noir */\n"
"    border: 1px solid rgb(41,127,110);  /* contour vert */\n"
"    border-radius: 6px;        /* coins arrondis */\n"
"    padding: 4px 8px;          /* marges internes */\n"
"    font-size: 14px;\n"
"}\n"
"\n"
"QLineEdit:focus {\n"
"    border: 2px solid rgb(41,127,110); /* contour plus \303\251pais quand actif */\n"
"    background-color: rgb(250, 250, 250); /* tr\303\250s l\303\251ger gris pour focus */\n"
"}\n"
""));
        label = new QLabel(groupBox_modify);
        label->setObjectName("label");
        label->setGeometry(QRect(10, 30, 71, 21));
        label->setStyleSheet(QString::fromUtf8("QLabel {\n"
"    /* normal state */\n"
"	background-color:rgb(254, 241, 223);\n"
"    color: rgb(41, 127, 110);\n"
"    border: none;\n"
"    border-radius: 1px;\n"
"    padding: 2px 2px;\n"
"    font-size: 14px;\n"
"    font-weight: bold;\n"
"}\n"
""));
        groupBox_list = new QGroupBox(widget);
        groupBox_list->setObjectName("groupBox_list");
        groupBox_list->setGeometry(QRect(0, 420, 361, 161));
        modifyButton = new QPushButton(groupBox_list);
        modifyButton->setObjectName("modifyButton");
        modifyButton->setGeometry(QRect(10, 120, 171, 35));
        modifyButton->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: rgb(41, 127, 110); /* vert par d\303\251faut */\n"
"    color: white; /* texte blanc */\n"
"    border: none;\n"
"    border-radius: 8px;\n"
"    padding: 8px 20px;\n"
"    font-size: 14px;\n"
"    font-weight: bold;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: rgb(209, 209, 209); /* gris clair en hover */\n"
"    color: rgb(41, 127, 110); /* texte vert */\n"
"}\n"
""));
        QIcon icon5(QIcon::fromTheme(QIcon::ThemeIcon::MailMessageNew));
        modifyButton->setIcon(icon5);
        label_2 = new QLabel(groupBox_list);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(0, 30, 141, 20));
        label_2->setStyleSheet(QString::fromUtf8("QLabel {\n"
"    /* normal state */\n"
"	background-color:rgb(255, 255, 255);\n"
"    color:rgb(46, 58, 114);\n"
"    border: none;\n"
"    border-radius: 1px;\n"
"    padding: 2px 2px;\n"
"    font-size: 14px;\n"
"    font-weight: bold;\n"
"}\n"
""));
        comboBox_4 = new QComboBox(groupBox_list);
        comboBox_4->addItem(QString());
        comboBox_4->addItem(QString());
        comboBox_4->addItem(QString());
        comboBox_4->addItem(QString());
        comboBox_4->addItem(QString());
        comboBox_4->setObjectName("comboBox_4");
        comboBox_4->setGeometry(QRect(150, 30, 191, 31));
        comboBox_4->setStyleSheet(QString::fromUtf8("QComboBox {\n"
"    background-color: white;                  \n"
"    color: black;                            \n"
"    border: 1px solid rgb(41,127,110);     \n"
"    border-radius: 6px;                    \n"
"    padding: 4px 8px;             \n"
"    font-size: 14px;\n"
"}\n"
"\n"
"QComboBox:focus {\n"
"    border: 2px solid rgb(41,127,110);       \n"
"    background-color: rgb(250, 250, 250);    \n"
"}\n"
"\n"
"QComboBox::drop-down {\n"
"    border: none;                            \n"
"    width: 24px;\n"
"}\n"
""));
        comboBox_4->setEditable(false);
        lineEdit_7 = new QLineEdit(groupBox_list);
        lineEdit_7->setObjectName("lineEdit_7");
        lineEdit_7->setGeometry(QRect(150, 70, 191, 31));
        lineEdit_7->setStyleSheet(QString::fromUtf8("QLineEdit {\n"
"    background-color: white;   /* fond blanc */\n"
"    color: black;              /* texte noir */\n"
"    border: 1px solid rgb(41,127,110);  /* contour vert */\n"
"    border-radius: 6px;        /* coins arrondis */\n"
"    padding: 4px 8px;          /* marges internes */\n"
"    font-size: 14px;\n"
"}\n"
"\n"
"QLineEdit:focus {\n"
"    border: 2px solid rgb(41,127,110); /* contour plus \303\251pais quand actif */\n"
"    background-color: rgb(250, 250, 250); /* tr\303\250s l\303\251ger gris pour focus */\n"
"}\n"
""));
        label_5 = new QLabel(groupBox_list);
        label_5->setObjectName("label_5");
        label_5->setGeometry(QRect(0, 70, 131, 20));
        label_5->setStyleSheet(QString::fromUtf8("QLabel {\n"
"    /* normal state */\n"
"	background-color:rgb(255, 255, 255);\n"
"    color:rgb(46, 58, 114);\n"
"    border: none;\n"
"    border-radius: 1px;\n"
"    padding: 2px 2px;\n"
"    font-size: 14px;\n"
"    font-weight: bold;\n"
"}\n"
""));
        groupBox_search->raise();
        groupBox_add->raise();
        groupBox_modify->raise();
        groupBox_list->raise();
        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName("tab_2");
        groupBox_search_2 = new QGroupBox(tab_2);
        groupBox_search_2->setObjectName("groupBox_search_2");
        groupBox_search_2->setGeometry(QRect(10, 10, 1051, 551));
        widget_2 = new QWidget(groupBox_search_2);
        widget_2->setObjectName("widget_2");
        widget_2->setGeometry(QRect(190, 90, 701, 391));
        widget_2->setStyleSheet(QString::fromUtf8("QWidget {\n"
"    background-image: url(\"C:/Users/Malek/Downloads/logo2.jpg\");\n"
"    background-repeat: no-repeat;\n"
"    background-position: center;\n"
"    background-attachment: fixed; /* optional */\n"
"}\n"
""));
        tabWidget->addTab(tab_2, QString());
        sideMenu = new QFrame(centralwidget);
        sideMenu->setObjectName("sideMenu");
        sideMenu->setGeometry(QRect(0, -10, 261, 711));
        sideMenu->setStyleSheet(QString::fromUtf8("QFrame{\n"
" border: 2px solid black;   /* blue border */\n"
"    border-radius: 8px;\n"
" font-weight: bold;\n"
"    font-size: 14px;\n"
"background-color: rgb(41, 127, 110);}"));
        sideMenu->setFrameShape(QFrame::Shape::StyledPanel);
        sideMenu->setFrameShadow(QFrame::Shadow::Raised);
        btnExamens = new QPushButton(sideMenu);
        btnExamens->setObjectName("btnExamens");
        btnExamens->setGeometry(QRect(10, 440, 231, 51));
        btnExamens->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: rgb(209, 209, 209);\n"
"    color: rgb(41, 127, 110);\n"
"    border: none;\n"
"    border-radius: 8px;\n"
"    padding: 8px 20px;\n"
"    font-size: 14px;\n"
"    font-weight: bold;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: rgb(41, 127, 110);\n"
"    color: white;\n"
"    border: 2px solid white;   /* \342\234\205 visible white border on hover */\n"
"    border-radius: 8px;\n"
"    padding: 6px 18px;         /* \342\234\205 keeps button size consistent */\n"
"}\n"
""));
        btnFinance = new QPushButton(sideMenu);
        btnFinance->setObjectName("btnFinance");
        btnFinance->setGeometry(QRect(10, 520, 231, 51));
        btnFinance->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: rgb(209, 209, 209);\n"
"    color: rgb(41, 127, 110);\n"
"    border: none;\n"
"    border-radius: 8px;\n"
"    padding: 8px 20px;\n"
"    font-size: 14px;\n"
"    font-weight: bold;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: rgb(41, 127, 110);\n"
"    color: white;\n"
"    border: 2px solid white;   /* \342\234\205 visible white border on hover */\n"
"    border-radius: 8px;\n"
"    padding: 6px 18px;         /* \342\234\205 keeps button size consistent */\n"
"}\n"
""));
        btnMoniteur = new QPushButton(sideMenu);
        btnMoniteur->setObjectName("btnMoniteur");
        btnMoniteur->setGeometry(QRect(10, 280, 231, 51));
        btnMoniteur->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: rgb(209, 209, 209);\n"
"    color: rgb(41, 127, 110);\n"
"    border: none;\n"
"    border-radius: 8px;\n"
"    padding: 8px 20px;\n"
"    font-size: 14px;\n"
"    font-weight: bold;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: rgb(41, 127, 110);\n"
"    color: white;\n"
"    border: 2px solid white;   /* \342\234\205 visible white border on hover */\n"
"    border-radius: 8px;\n"
"    padding: 6px 18px;         /* \342\234\205 keeps button size consistent */\n"
"}\n"
""));
        btnVehicule = new QPushButton(sideMenu);
        btnVehicule->setObjectName("btnVehicule");
        btnVehicule->setGeometry(QRect(10, 360, 231, 51));
        btnVehicule->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: rgb(209, 209, 209);\n"
"    color: rgb(41, 127, 110);\n"
"    border: none;\n"
"    border-radius: 8px;\n"
"    padding: 8px 20px;\n"
"    font-size: 14px;\n"
"    font-weight: bold;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: rgb(41, 127, 110);\n"
"    color: white;\n"
"    border: 2px solid white;   /* \342\234\205 visible white border on hover */\n"
"    border-radius: 8px;\n"
"    padding: 6px 18px;         /* \342\234\205 keeps button size consistent */\n"
"}\n"
""));
        btnAccueil = new QPushButton(sideMenu);
        btnAccueil->setObjectName("btnAccueil");
        btnAccueil->setGeometry(QRect(10, 120, 231, 51));
        btnAccueil->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: rgb(209, 209, 209);\n"
"    color: rgb(41, 127, 110);\n"
"    border: none;\n"
"    border-radius: 8px;\n"
"    padding: 8px 20px;\n"
"    font-size: 14px;\n"
"    font-weight: bold;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: rgb(41, 127, 110);\n"
"    color: white;\n"
"    border: 2px solid white;   /* \342\234\205 visible white border on hover */\n"
"    border-radius: 8px;\n"
"    padding: 6px 18px;         /* \342\234\205 keeps button size consistent */\n"
"}\n"
""));
        QIcon icon6(QIcon::fromTheme(QIcon::ThemeIcon::GoHome));
        btnAccueil->setIcon(icon6);
        btnCandidat = new QPushButton(sideMenu);
        btnCandidat->setObjectName("btnCandidat");
        btnCandidat->setGeometry(QRect(10, 200, 231, 51));
        btnCandidat->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: rgb(209, 209, 209);\n"
"    color: rgb(41, 127, 110);\n"
"    border: none;\n"
"    border-radius: 8px;\n"
"    padding: 8px 20px;\n"
"    font-size: 14px;\n"
"    font-weight: bold;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: rgb(41, 127, 110);\n"
"    color: white;\n"
"    border: 2px solid white;   /* \342\234\205 visible white border on hover */\n"
"    border-radius: 8px;\n"
"    padding: 6px 18px;         /* \342\234\205 keeps button size consistent */\n"
"}\n"
""));
        label_3 = new QLabel(centralwidget);
        label_3->setObjectName("label_3");
        label_3->setGeometry(QRect(630, 20, 301, 51));
        label_3->setStyleSheet(QString::fromUtf8("QLabel{\n"
" border: 2px solid black;   /* blue border */\n"
"    border-radius: 8px;\n"
" font-weight: bold;\n"
"    font-size: 28px;\n"
"background-color: rgb(41, 127, 110);}"));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1349, 25));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
#if QT_CONFIG(tooltip)
        tabWidget->setToolTip(QCoreApplication::translate("MainWindow", "<html><head/><body><p><br/></p></body></html>", nullptr));
#endif // QT_CONFIG(tooltip)
        groupBox_add->setTitle(QCoreApplication::translate("MainWindow", "Ajout d'examens", nullptr));
        leType->setText(QCoreApplication::translate("MainWindow", "Type :", nullptr));
        comboBox_3->setItemText(0, QCoreApplication::translate("MainWindow", "Conduite", nullptr));
        comboBox_3->setItemText(1, QCoreApplication::translate("MainWindow", "Code", nullptr));

        comboBox_3->setCurrentText(QCoreApplication::translate("MainWindow", "Conduite", nullptr));
        leLieu->setText(QCoreApplication::translate("MainWindow", "Lieu :", nullptr));
        deDate->setText(QCoreApplication::translate("MainWindow", "Date :", nullptr));
        cbResult->setText(QCoreApplication::translate("MainWindow", "R\303\251sultat :", nullptr));
        comboBox_6->setItemText(0, QCoreApplication::translate("MainWindow", "R\303\251ussite", nullptr));
        comboBox_6->setItemText(1, QCoreApplication::translate("MainWindow", "Echec", nullptr));

        addButton->setText(QCoreApplication::translate("MainWindow", "Ajouter examen", nullptr));
        leType_2->setText(QCoreApplication::translate("MainWindow", "CIN condidat :", nullptr));
        lineEdit_5->setText(QCoreApplication::translate("MainWindow", "123XXXXX", nullptr));
        groupBox_search->setTitle(QCoreApplication::translate("MainWindow", "Tableau", nullptr));
        QTableWidgetItem *___qtablewidgetitem = tableExams->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("MainWindow", "Type", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = tableExams->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("MainWindow", "Date", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = tableExams->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("MainWindow", "Lieu", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = tableExams->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("MainWindow", "R\303\251sultat", nullptr));
        QTableWidgetItem *___qtablewidgetitem4 = tableExams->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QCoreApplication::translate("MainWindow", "CIN", nullptr));
        exportButton->setText(QCoreApplication::translate("MainWindow", "Export\303\251 en pdf", nullptr));
        deleteButton->setText(QCoreApplication::translate("MainWindow", "Supprimer ", nullptr));
        label_4->setText(QCoreApplication::translate("MainWindow", "Trier par date :", nullptr));
        comboBox_5->setItemText(0, QCoreApplication::translate("MainWindow", "Croissant", nullptr));
        comboBox_5->setItemText(1, QCoreApplication::translate("MainWindow", "Decroissant", nullptr));

        comboBox_5->setCurrentText(QCoreApplication::translate("MainWindow", "Croissant", nullptr));
        exportButton_2->setText(QCoreApplication::translate("MainWindow", "Trier", nullptr));
        groupBox_modify->setTitle(QCoreApplication::translate("MainWindow", "Chercher", nullptr));
        btnSearch->setText(QCoreApplication::translate("MainWindow", "Chercher", nullptr));
        lineEdit_6->setInputMask(QString());
        lineEdit_6->setText(QCoreApplication::translate("MainWindow", "Rechercher...", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "CIN :", nullptr));
        groupBox_list->setTitle(QCoreApplication::translate("MainWindow", "Modifier", nullptr));
        modifyButton->setText(QCoreApplication::translate("MainWindow", "Modifier", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "Champ \303\240 modifier :", nullptr));
        comboBox_4->setItemText(0, QCoreApplication::translate("MainWindow", "Type", nullptr));
        comboBox_4->setItemText(1, QCoreApplication::translate("MainWindow", "Date", nullptr));
        comboBox_4->setItemText(2, QCoreApplication::translate("MainWindow", "Lieu", nullptr));
        comboBox_4->setItemText(3, QCoreApplication::translate("MainWindow", "R\303\251sultat", nullptr));
        comboBox_4->setItemText(4, QCoreApplication::translate("MainWindow", "CIN", nullptr));

        comboBox_4->setCurrentText(QCoreApplication::translate("MainWindow", "Type", nullptr));
        lineEdit_7->setInputMask(QString());
        lineEdit_7->setText(QCoreApplication::translate("MainWindow", "Nouvelle valeur", nullptr));
        label_5->setText(QCoreApplication::translate("MainWindow", "Nouvelle valeur :", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab), QCoreApplication::translate("MainWindow", "Examens", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QCoreApplication::translate("MainWindow", "Statistiques", nullptr));
        btnExamens->setText(QCoreApplication::translate("MainWindow", "Examens", nullptr));
        btnFinance->setText(QCoreApplication::translate("MainWindow", "Finance", nullptr));
        btnMoniteur->setText(QCoreApplication::translate("MainWindow", "Moniteur", nullptr));
        btnVehicule->setText(QCoreApplication::translate("MainWindow", "V\303\251hicules", nullptr));
        btnAccueil->setText(QCoreApplication::translate("MainWindow", "Acceuil", nullptr));
        btnCandidat->setText(QCoreApplication::translate("MainWindow", "Condidat", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "   Gestion examens ", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
