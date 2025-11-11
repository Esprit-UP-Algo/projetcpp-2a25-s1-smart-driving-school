#include "connexion.h"

Connexion::Connexion()
{
    db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("novadrive");
    db.setUserName("khalil");
    db.setPassword("novadrive");

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
