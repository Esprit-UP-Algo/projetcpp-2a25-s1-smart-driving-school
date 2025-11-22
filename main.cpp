#include "mainwindow.h"
#include "connection.h"
#include <QApplication>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Connection& test = Connection::createInstance();
    if (!test.createconnect()) {
        qDebug() << "Impossible de se connecter";
        return 0;
    } else{
        qDebug() << "Connexion établie avec succès!";

        MainWindow w;
        w.show();

        return a.exec();
    }
}





   /* if (test) {
        qDebug() << "Connexion établie avec succès!";

        MainWindow w;
        w.show();

        return a.exec();
    } else {
        QMessageBox::critical("Impossible de se connecter");
        return 0;
    }*/

