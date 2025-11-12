#ifndef MONITEUR_H
#define MONITEUR_H

#include <QString>
#include <QSqlQueryModel>

class Moniteur
{
public:
    Moniteur();
    Moniteur(QString nom, QString prenom, QString cin,
             QString tel, QString mail, QString voiture);

    bool ajouter();
    QSqlQueryModel* afficher();
    bool supprimer(int id);
    bool modifier(int id, QString champ, QString valeur);
    QSqlQueryModel* rechercher(QString cin);

private:
    QString nom, prenom, cin, tel, mail, voiture;
};

#endif // MONITEUR_H
