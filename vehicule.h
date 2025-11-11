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
    QString nom;
    QString immatricule;
    int kilometrage;
    QString etat;

public:

    Vehicule();
    Vehicule(int, QString, QString, int, QString);


    int Getid() { return id; }
    QString Getnom() { return nom; }
    QString Getimmatricule() { return immatricule; }
    int Getkilometrage() { return kilometrage; }
    QString Getetat() { return etat; }


    void Setid(int val) { id = val; }
    void Setnom(QString val) { nom = val; }
    void Setimmatricule(QString val) { immatricule = val; }
    void Setkilometrage(int val) { kilometrage = val; }
    void Setetat(QString val) { etat = val; }


    bool ajouter();
    QSqlQueryModel* afficher();
    bool supprimer(int);
    bool modifier(int, QString, QString);
    QSqlQueryModel* rechercher(QString);
    QSqlQueryModel* trier(QString);


    QString toString();
    bool estEnBonEtat();
};

#endif // VEHICULE_H
