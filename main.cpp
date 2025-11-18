#include "mainwindow.h"
#include "connexion.h"

#include <QApplication>
#include <QMessageBox>
#include <QDebug>
#include <QtSql/QSqlDatabase>
#include "role.h"
#include "logindialog.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // 1) Connexion DB
    Connexion c;
    if (!c.ouvrirConnexion()) {
        QMessageBox::critical(
            nullptr,
            QObject::tr("Échec"),
            QObject::tr("Échec de connexion à la base de données.\n"
                        "Cliquez OK pour quitter.")
            );
        return -1;
    }


    LoginDialog dlg;
    if (dlg.exec() != QDialog::Accepted) {
        return 0;
    }
    Role role = dlg.selectedRole();

    MainWindow w;
    w.setRole(role);
    w.show();

    return a.exec();
}













/*    QMessageBox::information(
        nullptr,
        QObject::tr("Base de données"),
        QObject::tr("Connexion établie avec succès.")
        );*/
