#ifndef CONDIDAT_H
#define CONDIDAT_H

#include <QString>
#include <QSqlQueryModel>

class Condidat {
private:
    int ID_COND{0};
    QString NOM, PRENOM, MAIL;
    int NUM_TEL{0}, CIN{0};

public:
    Condidat() = default;

    // Pour AJOUTER (sans ID)
    Condidat(const QString& nom, const QString& prenom, int numTel,
             const QString& mail, int cin)
        : NOM(nom), PRENOM(prenom), MAIL(mail), NUM_TEL(numTel), CIN(cin) {}

    // Pour MODIFIER/SUPPRIMER (avec ID)
    Condidat(int id, const QString& nom, const QString& prenom, int numTel,
             const QString& mail, int cin)
        : ID_COND(id), NOM(nom), PRENOM(prenom), MAIL(mail), NUM_TEL(numTel), CIN(cin) {}

    // Setters / Getters
    void setIdCond(int id) { ID_COND = id; }
    int  getIdCond() const { return ID_COND; }
    QString getNom() const { return NOM; }
    QString getPrenom() const { return PRENOM; }
    int getNumTel() const { return NUM_TEL; }
    QString getMail() const { return MAIL; }
    int getCin() const { return CIN; }

    // CRUD
    bool ajouter();               // INSERT sans ID (auto-incrément en BD)
    bool modifier();              // UPDATE WHERE ID_COND=:id
    bool supprimer(int idCond);   // DELETE WHERE ID_COND=:id
    bool rechercher();            // SELECT * WHERE ID_COND=:id → remplit l'objet
    QSqlQueryModel* afficher();   // SELECT * pour tableView
};

#endif // CONDIDAT_H
