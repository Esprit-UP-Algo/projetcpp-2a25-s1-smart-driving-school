#include "connexion.h"

Connexion::Connexion()
{
    db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("source_projet2A");
    db.setUserName("malik");
    db.setPassword("14544286");

}

bool Connexion::ouvrirConnexion()
{
    if(db.open()) {
        qDebug() << "Connexion réussie à Oracle !";
        return true;
    } else {
        qDebug() << "Erreur de connexion : " << db.lastError().text();
        return false;
    }
}

void Connexion::fermerConnexion()
{
    db.close();
}
