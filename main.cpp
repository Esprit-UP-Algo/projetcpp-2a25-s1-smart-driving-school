#include "mainwindow.h"
#include "mainwindowV.h"
#include "connexion.h"
#include "connection.h"
#include "logindialog.h"
#include "role.h"

#include <QApplication>
#include <QMessageBox>
#include <QDebug>
#include <QtSql/QSqlDatabase>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // 1) Establish database connection
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

    qDebug() << "Connexion établie avec succès!";

    // 2) Show login dialog
    LoginDialog dlg;
    if (dlg.exec() != QDialog::Accepted) {
        return 0;  // User cancelled login
    }

    Role role = dlg.selectedRole();
    qDebug() << "User role:" << static_cast<int>(role);

    // 3) Show MainWindow (Examens) as primary window

        MainWindow *w = new MainWindow();
        w->setRole(role);
        w->show();


    return a.exec();
}
