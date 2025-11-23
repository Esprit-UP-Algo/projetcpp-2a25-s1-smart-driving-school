#include "condidat.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDebug>

bool Condidat::ajouter() {
    // ⚠️ INSERT SANS ID : la BD génère l'ID (IDENTITY/AUTO_INCREMENT/AUTOINCREMENT ou séquence côté SQL)
    QSqlQuery q;
    q.prepare(R"(INSERT INTO condidat (NOM, PRENOM, NUM_TEL, MAIL, CIN)
                 VALUES (:nom, :prenom, :tel, :mail, :cin))");
    q.bindValue(":nom", NOM);
    q.bindValue(":prenom", PRENOM);
    q.bindValue(":tel", NUM_TEL);
    q.bindValue(":mail", MAIL);
    q.bindValue(":cin", CIN);

    if (!q.exec()) {
        qDebug() << "INSERT error:" << q.lastError().text();
        return false;
    }

    // Récupérer l'ID généré (méthode simple/portable)
    QSqlQuery qid("SELECT MAX(ID_COND) FROM condidat");
    if (qid.next()) ID_COND = qid.value(0).toInt();

    return true;
}

bool Condidat::modifier() {
    QSqlQuery q;
    q.prepare(R"(UPDATE condidat
                 SET NOM=:nom, PRENOM=:prenom, NUM_TEL=:tel, MAIL=:mail, CIN=:cin
                 WHERE ID_COND=:id)");
    q.bindValue(":nom", NOM);
    q.bindValue(":prenom", PRENOM);
    q.bindValue(":tel", NUM_TEL);
    q.bindValue(":mail", MAIL);
    q.bindValue(":cin", CIN);
    q.bindValue(":id", ID_COND);

    if (!q.exec()) {
        qDebug() << "UPDATE error:" << q.lastError().text();
        return false;
    }
    return (q.numRowsAffected() > 0);
}

bool Condidat::supprimer(int idCond) {
    QSqlQuery q;
    q.prepare("DELETE FROM condidat WHERE ID_COND=:id");
    q.bindValue(":id", idCond);
    if (!q.exec()) {
        qDebug() << "DELETE error:" << q.lastError().text();
        return false;
    }
    return (q.numRowsAffected() > 0);
}

bool Condidat::rechercher() {
    QSqlQuery q;
    q.prepare("SELECT NOM, PRENOM, NUM_TEL, MAIL, CIN FROM condidat WHERE ID_COND=:id");
    q.bindValue(":id", ID_COND);

    if (!q.exec()) {
        qDebug() << "SELECT error:" << q.lastError().text();
        return false;
    }
    if (!q.next()) return false;

    NOM     = q.value(0).toString();
    PRENOM  = q.value(1).toString();
    NUM_TEL = q.value(2).toInt();
    MAIL    = q.value(3).toString();
    CIN     = q.value(4).toInt();
    return true;
}

QSqlQueryModel* Condidat::afficher() {
    auto *m = new QSqlQueryModel();
    m->setQuery("SELECT ID_COND, NOM, PRENOM, NUM_TEL, MAIL, CIN FROM condidat");
    m->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    m->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom"));
    m->setHeaderData(2, Qt::Horizontal, QObject::tr("Prénom"));
    m->setHeaderData(3, Qt::Horizontal, QObject::tr("Tel"));
    m->setHeaderData(4, Qt::Horizontal, QObject::tr("Mail"));
    m->setHeaderData(5, Qt::Horizontal, QObject::tr("CIN"));
    return m;
}
