#ifndef VEHICULE_H
#define VEHICULE_H

#include <QString>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlError>

class Vehicule
{
private:
    int id;
    QString immatricule;
    QString nom;
    int kilometrage;
    QString etat;

public:
    // Constructeurs
    Vehicule();
    Vehicule(int, QString, QString, int, QString);

    // Getters
    int Getid() { return id; }
    QString Getimmatricule() { return immatricule; }
    QString Getnom() { return nom; }
    int Getkilometrage() { return kilometrage; }
    QString Getetat() { return etat; }

    // Setters
    void Setid(int val) { id = val; }
    void Setimmatricule(QString val) { immatricule = val; }
    void Setnom(QString val) { nom = val; }
    void Setkilometrage(int val) { kilometrage = val; }
    void Setetat(QString val) { etat = val; }

    // Méthodes CRUD
    bool ajouter();
    QSqlQueryModel* afficher();
    bool supprimer(int);
    bool modifier(int, QString, QString);
    QSqlQueryModel* rechercher(QString);
    QSqlQueryModel* trier(QString);

    // Méthodes utiles
    QString toString();
    bool estEnBonEtat();
};

#endif // VEHICULE_H
