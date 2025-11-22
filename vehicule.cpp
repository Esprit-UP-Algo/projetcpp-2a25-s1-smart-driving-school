#include "vehicule.h"
#include <QSqlError>
#include <QDebug>


Vehicule::Vehicule()
{
    this->id = 0;
    this->immatricule = "";
    this->nom = "";
    this->kilometrage = 0;
    this->etat = "bonne etat";
}


Vehicule::Vehicule(int id, QString immat, QString nom, int km, QString etat)
{
    this->id = id;
    this->immatricule = immat;
    this->nom = nom;
    this->kilometrage = km;
    this->etat = etat;
}


bool Vehicule::ajouter()
{
    QSqlQuery query;

    qDebug() << "=== AJOUT VEHICULE ===";
    qDebug() << "ID:" << this->id;
    qDebug() << "Immatricule:" << this->immatricule;
    qDebug() << "Nom:" << this->nom;
    qDebug() << "Kilometrage:" << this->kilometrage;
    qDebug() << "Etat:" << this->etat;

    query.prepare("INSERT INTO VEHICULE (ID_U, MATRICULE, MARQUE, KILOMETRAGE, ETAT) "
                  "VALUES (:id, :matricule, :marque, :km, :etat)");

    query.bindValue(":id", this->id);
    query.bindValue(":matricule", this->immatricule);
    query.bindValue(":marque", this->nom);
    query.bindValue(":km", this->kilometrage);
    query.bindValue(":etat", this->etat);

    if (query.exec()) {
        qDebug() << "Ajout réussi!";
        return true;
    } else {
        qDebug() << "=== ERREUR AJOUT ===";
        qDebug() << "Message d'erreur:" << query.lastError().text();
        qDebug() << "Type d'erreur:" << query.lastError().type();
        qDebug() << "Erreur driver:" << query.lastError().driverText();
        qDebug() << "Erreur DB:" << query.lastError().databaseText();
        return false;
    }
}


QSqlQueryModel* Vehicule::afficher()
{
    QSqlQueryModel* model = new QSqlQueryModel();

    model->setQuery("SELECT ID_U, MATRICULE, MARQUE, KILOMETRAGE, ETAT FROM VEHICULE");

    if (model->lastError().isValid()) {
        qDebug() << "Erreur affichage:" << model->lastError().text();
    }

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Matricule"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Marque"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Kilométrage"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("État"));

    return model;
}


bool Vehicule::supprimer(int id)
{
    QSqlQuery query;

    query.prepare("DELETE FROM VEHICULE WHERE ID_U = :id");
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

    QString requete = QString("UPDATE VEHICULE SET %1 = :valeur WHERE ID_U = :id").arg(champ);
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
    query.prepare("SELECT ID_U, MATRICULE, MARQUE, KILOMETRAGE, ETAT FROM VEHICULE "
                  "WHERE LOWER(MARQUE) LIKE :terme OR LOWER(MATRICULE) LIKE :terme");
    query.bindValue(":terme", "%" + terme.toLower() + "%");

    if (query.exec()) {
        model->setQuery(std::move(query));
        model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
        model->setHeaderData(1, Qt::Horizontal, QObject::tr("Matricule"));
        model->setHeaderData(2, Qt::Horizontal, QObject::tr("Marque"));
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

    QString requete = "SELECT ID_U, MATRICULE, MARQUE, KILOMETRAGE, ETAT FROM VEHICULE ORDER BY " + critere;
    model->setQuery(requete);

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Matricule"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Marque"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Kilométrage"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("État"));

    return model;
}

QString Vehicule::toString()
{
    return "Vehicule [ID: " + QString::number(id) +
           ", Immatricule: " + immatricule +
           ", Nom: " + nom +
           ", Kilometrage: " + QString::number(kilometrage) + " km" +
           ", Etat: " + etat + "]";
}

bool Vehicule::estEnBonEtat()
{
    return this->etat == "bonne etat";
}
