#include "exam.h"
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlError>
#include <QDebug>
#include <QDate>
#include <QDateTime>

// Noms Oracle
static const char* TABLE_NAME = "EXAMENS";
static const char* COL_TYPE   = "TYPE";
static const char* COL_DATE   = "DATE_EXAM";
static const char* COL_LIEU   = "LIEU";
static const char* COL_RES    = "RESULTAT";
static const char* COL_CIN    = "CINC";

// INSERT
bool Exam::ajouter() const
{
    QDate d = QDate::fromString(date, "dd/MM/yyyy");
    if (!d.isValid()) {
        qDebug() << "[Exam::ajouter] invalid date:" << date;
        return false;
    }
    QDateTime dt(d.startOfDay());
    {
        QSqlQuery q;
        q.prepare(QString("INSERT INTO %1 (%2, %3, %4, %5, %6) "
                          "VALUES (?, ?, ?, ?, ?)")
                      .arg(TABLE_NAME).arg(COL_TYPE).arg(COL_DATE)
                      .arg(COL_LIEU).arg(COL_RES).arg(COL_CIN));

        q.addBindValue(type);
        q.addBindValue(dt);
        q.addBindValue(lieu);
        q.addBindValue(resultat);
        q.addBindValue(cin);

        if (q.exec()) {
            return true;
        } else {
            const QString err = q.lastError().text();
            qDebug() << "[Exam::ajouter][A] error:" << err;
            if (!err.contains("S1010", Qt::CaseInsensitive) &&
                !err.contains("Erreur de séquence", Qt::CaseInsensitive)) {
                return false;
            }
        }
    }

    {
        const QString iso = d.toString("yyyy-MM-dd");

        QSqlQuery q2;
        q2.prepare(QString("INSERT INTO %1 (%2, %3, %4, %5, %6) "
                           "VALUES (?, TO_DATE(?, 'YYYY-MM-DD'), ?, ?, ?)")
                       .arg(TABLE_NAME).arg(COL_TYPE).arg(COL_DATE)
                       .arg(COL_LIEU).arg(COL_RES).arg(COL_CIN));

        q2.addBindValue(type);
        q2.addBindValue(iso);
        q2.addBindValue(lieu);
        q2.addBindValue(resultat);
        q2.addBindValue(cin);

        if (!q2.exec()) {
            qDebug() << "[Exam::ajouter][B] error:" << q2.lastError().text();
            return false;
        }
        return true;
    }
}


bool Exam::supprimer(const QString &cin, const QString &date_ddMMyyyy)
{
    QSqlQuery q;
    q.prepare(QString("DELETE FROM %1 "
                      "WHERE %2 = :cin AND TRUNC(%3) = TRUNC(TO_DATE(:date,'DD/MM/YYYY'))")
                  .arg(TABLE_NAME).arg(COL_CIN).arg(COL_DATE));
    q.bindValue(":cin",  cin);
    q.bindValue(":date", date_ddMMyyyy);

    if (!q.exec()) {
        qDebug() << "Exam::supprimer() error:" << q.lastError().text();
        return false;
    }
    return q.numRowsAffected() > 0;
}


QSqlQueryModel* Exam::afficherTous()
{
    QSqlQueryModel* model = new QSqlQueryModel();

    model->setQuery("SELECT TYPE, "
                    "TO_CHAR(DATE_EXAM,'DD/MM/YYYY') AS DATE_EXAM, "
                    "LIEU, RESULTAT, CINC "
                    "FROM EXAMENS");

    if (model->lastError().isValid()) {
        qDebug() << "[Exam::afficherTous] Erreur SQL:"
                 << model->lastError().text();
    }

    model->setHeaderData(0, Qt::Horizontal, "Type");
    model->setHeaderData(1, Qt::Horizontal, "Date");
    model->setHeaderData(2, Qt::Horizontal, "Lieu");
    model->setHeaderData(3, Qt::Horizontal, "Résultat");
    model->setHeaderData(4, Qt::Horizontal, "CIN");

    return model;
}

QSqlQueryModel* Exam::afficherParCin(const QString &cin)
{
    QSqlQueryModel* model = new QSqlQueryModel();
    QSqlQuery query;

    query.prepare("SELECT TYPE, "
                  "TO_CHAR(DATE_EXAM,'DD/MM/YYYY') AS DATE_EXAM, "
                  "LIEU, RESULTAT, CINC "
                  "FROM EXAMENS "
                  "WHERE CINC LIKE :cin");
    query.bindValue(":cin", "%" + cin + "%");
    query.exec();

    model->setQuery(query);

    if (model->lastError().isValid()) {
        qDebug() << "[Exam::afficherParCin] Erreur SQL:"
                 << model->lastError().text();
    }

    model->setHeaderData(0, Qt::Horizontal, "Type");
    model->setHeaderData(1, Qt::Horizontal, "Date");
    model->setHeaderData(2, Qt::Horizontal, "Lieu");
    model->setHeaderData(3, Qt::Horizontal, "Résultat");
    model->setHeaderData(4, Qt::Horizontal, "CIN");

    return model;
}
