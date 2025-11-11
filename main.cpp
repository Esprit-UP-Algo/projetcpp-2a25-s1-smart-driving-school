#include "mainwindow.h"
#include "connection.h"
#include <QApplication>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Établir la connexion AVANT d'instancier MainWindow
    Connection c;
    bool test = c.createconnect();

    if (test) {
        qDebug() << "Connexion établie avec succès!";

        // Instancier MainWindow (appelle le constructeur -> afficher())
        MainWindow w;
        w.show();

        return a.exec();
    } else {
        QMessageBox::critical(nullptr, "Erreur", "Impossible de se connecter à la base de données!");
        return 0;
    }
}
