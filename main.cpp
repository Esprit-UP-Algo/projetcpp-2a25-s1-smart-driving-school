#include "mainwindow.h"
#include "connexion.h"

#include <QApplication>
#include <QMessageBox>
#include <QDebug>
#include <QtSql/QSqlDatabase>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Connexion c;
    const bool ok = c.ouvrirConnexion();

    if (!ok) {
        QMessageBox::critical(
            nullptr,
            QObject::tr("Échec"),
            QObject::tr("Échec de connexion à la base de données.\n"
                        "Cliquez OK pour quitter.")
            );
        return -1;
    }

    MainWindow w;
    w.show();

    QMessageBox::information(
        &w,
        QObject::tr("Base de données"),
        QObject::tr("FÉLICITATIONS, vous êtes connecté à la base !\n"
                    "Cliquez OK pour continuer.")
        );

    return a.exec();
}
