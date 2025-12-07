#include "mainwindowk.h"
<<<<<<< HEAD
#include "mainwindowV.h"
#include "connexion.h"
#include "connection.h"
#include "logindialog.h"
#include "role.h"
=======
#include "connexion.h"
>>>>>>> origin/integrate-condidat

#include <QApplication>
#include <QMessageBox>
#include <QDebug>
#include <QtSql/QSqlDatabase>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

<<<<<<< HEAD
    // 1) Establish database connection
    Connexion c;
    if (!c.ouvrirConnexion()) {
=======
    Connexion c;
    const bool ok = c.ouvrirConnexion();

    if (!ok) {
>>>>>>> origin/integrate-condidat
        QMessageBox::critical(
            nullptr,
            QObject::tr("Échec"),
            QObject::tr("Échec de connexion à la base de données.\n"
                        "Cliquez OK pour quitter.")
            );
        return -1;
    }

<<<<<<< HEAD
    qDebug() << "Connexion établie avec succès!";

    // 2) Show login dialog
    LoginDialog dlg;
    if (dlg.exec() != QDialog::Accepted) {
        return 0;  // User cancelled login
    }

    Role role = dlg.selectedRole();
    qDebug() << "User role:" << static_cast<int>(role);

    // 3) Show MainWindow (Examens) as primary window
    try {
        MainWindow *w = new MainWindow();
        w->setRole(role);
        w->show();
    } catch (...) {
        QMessageBox::critical(nullptr, "Erreur",
                              "Impossible de charger la fenêtre principale (Examens).\n"
                              "Affichage de la fenêtre Véhicules à la place.");

        // Fallback to vehicle window if MainWindow crashes
        MainWindowV *vehicleWindow = new MainWindowV();
        vehicleWindow->setWindowTitle("Gestion Véhicules - " +
                                      QString(role == Role::Admin ? "Admin" : "Moniteur"));
        vehicleWindow->show();
    }
=======
    MainWindow w;
    w.show();

    QMessageBox::information(
        &w,
        QObject::tr("Base de données"),
        QObject::tr("FÉLICITATIONS, vous êtes connecté à la base !\n"
                    "Cliquez OK pour continuer.")
        );
>>>>>>> origin/integrate-condidat

    return a.exec();
}
