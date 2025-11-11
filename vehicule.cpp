#include "vehicule.h"
#include <QSqlError>
#include <QDebug>


Vehicule::Vehicule()
{
    this->id = 0;
    this->nom = "";
    this->immatricule = "";
    this->kilometrage = 0;
    this->etat = "bonne etat";
}


Vehicule::Vehicule(int id, QString nom, QString immat, int km, QString etat)
{
    this->id = id;
    this->nom = nom;
    this->immatricule = immat;
    this->kilometrage = km;
    this->etat = etat;
}


bool Vehicule::ajouter()
{
    QSqlQuery query;


    query.prepare("INSERT INTO VEHICULES (ID, NOM, IMMATRICULE, KILOMETRAGE, ETAT) "
                  "VALUES (:id, :nom, :immat, :km, :etat)");


    query.bindValue(":id", this->id);
    query.bindValue(":nom", this->nom);
    query.bindValue(":immat", this->immatricule);
    query.bindValue(":km", this->kilometrage);
    query.bindValue(":etat", this->etat);


    if (query.exec()) {
        return true;
    } else {
        qDebug() << "Erreur ajout:" << query.lastError().text();
        return false;
    }
}


QSqlQueryModel* Vehicule::afficher()
{
    QSqlQueryModel* model = new QSqlQueryModel();

    model->setQuery("SELECT ID, NOM, IMMATRICULE, KILOMETRAGE, ETAT FROM VEHICULES");


    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom Véhicule"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Immatricule"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Kilométrage"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("État"));

    return model;
}


bool Vehicule::supprimer(int id)
{
    QSqlQuery query;


    query.prepare("DELETE FROM VEHICULES WHERE ID = :id");
    query.bindValue(":id", id);

    if (query.exec()) {
        return true;
    } else {
        qDebug() << "Erreur suppression:" << query.lastError().text();
        return false;
    }
}


bool Vehicule::modifier(int id, QString champ, QString valeur)
{
    QSqlQuery query;


    QString requete = QString("UPDATE VEHICULES SET %1 = :valeur WHERE ID = :id").arg(champ);
    query.prepare(requete);
    query.bindValue(":valeur", valeur);
    query.bindValue(":id", id);

    if (query.exec()) {
        return true;
    } else {
        qDebug() << "Erreur modification:" << query.lastError().text();
        return false;
    }
}


QSqlQueryModel* Vehicule::rechercher(QString terme)
{
    QSqlQueryModel* model = new QSqlQueryModel();
    QSqlQuery query;


    query.prepare("SELECT ID, NOM, IMMATRICULE, KILOMETRAGE, ETAT FROM VEHICULES "
                  "WHERE LOWER(NOM) LIKE :terme OR LOWER(IMMATRICULE) LIKE :terme");
    query.bindValue(":terme", "%" + terme.toLower() + "%");

    if (query.exec()) {
        model->setQuery(query);
        model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
        model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom Véhicule"));
        model->setHeaderData(2, Qt::Horizontal, QObject::tr("Immatricule"));
        model->setHeaderData(3, Qt::Horizontal, QObject::tr("Kilométrage"));
        model->setHeaderData(4, Qt::Horizontal, QObject::tr("État"));
    } else {
        qDebug() << "Erreur recherche:" << query.lastError().text();
    }

    return model;
}


QSqlQueryModel* Vehicule::trier(QString critere)
{
    QSqlQueryModel* model = new QSqlQueryModel();

    QString requete = "SELECT ID, NOM, IMMATRICULE, KILOMETRAGE, ETAT FROM VEHICULES ORDER BY " + critere;
    model->setQuery(requete);

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom Véhicule"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Immatricule"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Kilométrage"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("État"));

    return model;
}

QString Vehicule::toString()
{
    return "Vehicule [ID: " + QString::number(id) +
           ", Nom: " + nom +
           ", Immatricule: " + immatricule +
           ", Kilometrage: " + QString::number(kilometrage) + " km" +
           ", Etat: " + etat + "]";
}

bool Vehicule::estEnBonEtat()
{
    return this->etat == "bonne etat";
}
