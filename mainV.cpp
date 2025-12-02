#include "mainwindowV.h"
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

        MainWindowV w;  // Changed from MainWindow to MainWindowV
        w.show();

        return a.exec();
    }
}
