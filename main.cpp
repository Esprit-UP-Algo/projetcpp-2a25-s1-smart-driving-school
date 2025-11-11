#include "mainwindow.h"
#include "connection.h"

#include <QApplication>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // 1) Ouvrir la base AVANT toute requête/affichage
    Connection c;
    if (!c.createconnect()) {
        QMessageBox::critical(nullptr,
                              QObject::tr("Base de données"),
                              QObject::tr("Échec d'ouverture de la base.\nVérifiez DSN/driver/identifiants."));
        return 1; // on ne continue pas si la BD n'est pas ouverte
    }

    // 2) Lancer l'UI une fois connecté
    MainWindow w;
    w.show();

    // (Optionnel) message d’info non bloquant
    // QMessageBox::information(&w, QObject::tr("Base de données"),
    //                          QObject::tr("Connexion réussie."));

    return a.exec();
}
