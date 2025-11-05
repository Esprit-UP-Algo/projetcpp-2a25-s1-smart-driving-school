/********************************************************************************
** Form generated from reading UI file 'connexion.ui'
**
** Created by: Qt User Interface Compiler version 6.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CONNEXION_H
#define UI_CONNEXION_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Form
{
public:
    QGroupBox *groupBox;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLineEdit *lineEdit;
    QLineEdit *lineEdit_2;
    QLineEdit *lineEdit_3;
    QPushButton *pushButton;

    void setupUi(QWidget *Form)
    {
        if (Form->objectName().isEmpty())
            Form->setObjectName("Form");
        Form->resize(400, 300);
        groupBox = new QGroupBox(Form);
        groupBox->setObjectName("groupBox");
        groupBox->setGeometry(QRect(30, 50, 311, 161));
        label = new QLabel(groupBox);
        label->setObjectName("label");
        label->setGeometry(QRect(10, 40, 47, 13));
        label_2 = new QLabel(groupBox);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(10, 80, 47, 13));
        label_3 = new QLabel(groupBox);
        label_3->setObjectName("label_3");
        label_3->setGeometry(QRect(10, 120, 47, 13));
        lineEdit = new QLineEdit(groupBox);
        lineEdit->setObjectName("lineEdit");
        lineEdit->setGeometry(QRect(140, 40, 113, 20));
        lineEdit_2 = new QLineEdit(groupBox);
        lineEdit_2->setObjectName("lineEdit_2");
        lineEdit_2->setGeometry(QRect(140, 80, 113, 20));
        lineEdit_3 = new QLineEdit(groupBox);
        lineEdit_3->setObjectName("lineEdit_3");
        lineEdit_3->setGeometry(QRect(140, 120, 113, 20));
        pushButton = new QPushButton(Form);
        pushButton->setObjectName("pushButton");
        pushButton->setGeometry(QRect(260, 230, 75, 23));

        retranslateUi(Form);

        QMetaObject::connectSlotsByName(Form);
    } // setupUi

    void retranslateUi(QWidget *Form)
    {
        Form->setWindowTitle(QCoreApplication::translate("Form", "Form", nullptr));
        groupBox->setTitle(QCoreApplication::translate("Form", "Ajouter Client", nullptr));
        label->setText(QCoreApplication::translate("Form", "Nom", nullptr));
        label_2->setText(QCoreApplication::translate("Form", "Pr\303\251nom", nullptr));
        label_3->setText(QCoreApplication::translate("Form", "Age", nullptr));
        pushButton->setText(QCoreApplication::translate("Form", "Ajouter", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Form: public Ui_Form {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONNEXION_H
