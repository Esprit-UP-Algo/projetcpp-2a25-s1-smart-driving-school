#include "exam.h"
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlError>
#include <QDebug>
#include <QDate>
#include <QDateTime>
#include <utility>



// Noms Oracle
static const char* TABLE_NAME = "EXAMENS";
static const char* COL_TYPE   = "TYPE";
static const char* COL_DATE   = "DATE_EXAM";
static const char* COL_LIEU   = "LIEU";
static const char* COL_RES    = "RESULTAT";
static const char* COL_CIN    = "CINC";
static inline bool isExactly8Digits(const QString &s) {
    const QString t = s.trimmed();
    if (t.size() != 8) return false;
    for (const QChar &c : t) if (!c.isDigit()) return false;
    return true;
}
static inline bool isReussite(const QString &s) {
    const QString t = s.trimmed().toLower();
    return (t == "réussite" || t == "reussite");
}
static inline bool isEchec(const QString &s) {
    const QString t = s.trimmed().toLower();
    return (t == "échec" || t == "echec");
}



// Récupère (date, résultat) de la DERNIÈRE tentative pour (CIN, TYPE).
static bool fetchLastAttempt(const QString &cin, const QString &type,
                             QDate *lastDate, QString *lastRes)
{
    QSqlQuery q;
    q.prepare(
        "SELECT TRUNC(DATE_EXAM), RESULTAT "
        "FROM EXAMENS "
        "WHERE CINC=:cin AND UPPER(TYPE)=UPPER(:t) "
        "  AND TRUNC(DATE_EXAM) = ("
        "        SELECT MAX(TRUNC(DATE_EXAM)) "
        "        FROM EXAMENS "
        "        WHERE CINC=:cin AND UPPER(TYPE)=UPPER(:t)"
        "  )"
        );
    q.bindValue(":cin", cin);
    q.bindValue(":t",   type);

    if (!q.exec()) {
        qDebug() << "[fetchLastAttempt] SQL:" << q.lastError().text();
        return false;
    }
    if (q.next()) {
        *lastDate = q.value(0).toDate();
        *lastRes  = q.value(1).toString();
        return true;            // il y a une précédente tentative
    }
    return false;               // aucune précédente tentative
}
static bool existsAnyReussite(const QString &cin, const QString &type)
{
    QSqlQuery v;
    v.prepare("SELECT COUNT(*) FROM EXAMENS "
              "WHERE CINC=:cin AND UPPER(TYPE)=UPPER(:t) "
              "AND UPPER(RESULTAT) IN ('REUSSITE','RÉUSSITE')");
    v.bindValue(":cin", cin.trimmed());
    v.bindValue(":t",   type.trimmed());
    if (!v.exec() || !v.next()) { qDebug() << "[existsAnyReussite] SQL:" << v.lastError().text(); return true; }
    return v.value(0).toInt() > 0;
}

// A-t-il un Code réussi avant (ou le même jour) ?
static bool hasCodeReussiBeforeOrSame(const QString &cin, const QDate &d)
{
    QSqlQuery qc;
    qc.prepare("SELECT COUNT(*) FROM EXAMENS "
               "WHERE CINC=:cin AND UPPER(TYPE)='CODE' "
               "AND UPPER(RESULTAT) IN ('REUSSITE','RÉUSSITE') "
               "AND TRUNC(DATE_EXAM) <= TRUNC(:d)");
    qc.bindValue(":cin", cin.trimmed());
    qc.bindValue(":d",  QDateTime(d.startOfDay()));
    if (!qc.exec() || !qc.next()) { qDebug() << "[hasCodeReussiBeforeOrSame] SQL:" << qc.lastError().text(); return false; }
    return qc.value(0).toInt() > 0;
}


bool Exam::ajouter() const
{
    const QDate d = QDate::fromString(date, "dd/MM/yyyy");
    if (!d.isValid()) { qDebug() << "[ajouter] date invalide:" << date; return false; }
    if (!isExactly8Digits(cin)) { qDebug() << "[ajouter] CIN invalide (8 chiffres):" << cin; return false; }

    const QString typeNorm = type.trimmed();
    const QString resNorm  = resultat.trimmed();

    if (existsAnyReussite(cin, typeNorm)) {
        qDebug() << "[ajouter] refus: une Réussite existe déjà pour (CIN, TYPE).";
        return false;
    }

    if (typeNorm.compare("conduite", Qt::CaseInsensitive) == 0 && isReussite(resNorm)) {
        if (!hasCodeReussiBeforeOrSame(cin, d)) {
            qDebug() << "[ajouter] refus: Conduite+Réussite sans Code Réussi avant.";
            return false;
        }
    }

    QDate lastDate; QString lastRes;
    if (fetchLastAttempt(cin, typeNorm, &lastDate, &lastRes)) {
        if (!isEchec(lastRes)) {
            qDebug() << "[ajouter] refus: dernière tentative n’est pas un Échec (" << lastRes << ")";
            return false;
        }
        if (d < lastDate.addMonths(1)) {
            qDebug() << "[ajouter] refus: moins d’un mois depuis la dernière tentative (" << lastDate << " -> " << lastDate.addMonths(1) << ")";
            return false;
        }
    }
    const QDateTime dt(d.startOfDay());
    {
        QSqlQuery q;
        q.prepare(QString("INSERT INTO %1 (%2,%3,%4,%5,%6) VALUES (?,?,?,?,?)")
                      .arg(TABLE_NAME).arg(COL_TYPE).arg(COL_DATE).arg(COL_LIEU).arg(COL_RES).arg(COL_CIN));
        q.addBindValue(typeNorm);
        q.addBindValue(dt);
        q.addBindValue(lieu);
        q.addBindValue(resNorm);
        q.addBindValue(cin);
        if (q.exec()) return true;

        const QString err = q.lastError().text();
        if (!err.contains("S1010", Qt::CaseInsensitive) &&
            !err.contains("Erreur de séquence", Qt::CaseInsensitive)) {
            qDebug() << "[ajouter][A]:" << err;
            return false;
        }
        qDebug() << "[ajouter][A->B fallback]:" << err;
    }

    {
        QSqlQuery q2;
        q2.prepare(QString("INSERT INTO %1 (%2,%3,%4,%5,%6) "
                           "VALUES (?, TO_DATE(?, 'YYYY-MM-DD'), ?, ?, ?)")
                       .arg(TABLE_NAME).arg(COL_TYPE).arg(COL_DATE).arg(COL_LIEU).arg(COL_RES).arg(COL_CIN));
        q2.addBindValue(typeNorm);
        q2.addBindValue(d.toString("yyyy-MM-dd"));
        q2.addBindValue(lieu);
        q2.addBindValue(resNorm);
        q2.addBindValue(cin);
        if (!q2.exec()) { qDebug() << "[ajouter][B]:" << q2.lastError().text(); return false; }
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

    model->setQuery(std::move(query));

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


bool Exam::modifierTexte(const QString &cin, const QDate &oldDate,const QString &col, const QString &val)
{
    if (col != "TYPE" && col != "LIEU" && col != "RESULTAT" && col != "CINC") return false;
    if (col == "CINC" && !isExactly8Digits(val)) {
        qDebug() << "modifierTexte: CIN invalide (8 chiffres)";
        return false;
    }
    QSqlQuery q;
    q.prepare(QString("UPDATE EXAMENS SET %1 = :v WHERE CINC=:c AND DATE_EXAM=:d").arg(col));
    q.bindValue(":v", val);
    q.bindValue(":c", cin);
    q.bindValue(":d", QDateTime(oldDate.startOfDay())); // Oracle DATE

    if (!q.exec()) { qDebug() << "modifierTexte:" << q.lastError().text(); return false; }
    return true;
}

bool Exam::modifierDate(const QString &cin, const QDate &oldDate, const QDate &newDate)
{
    if (!newDate.isValid()) return false;

    QSqlQuery q;
    q.prepare("UPDATE EXAMENS SET DATE_EXAM=:nd WHERE CINC=:c AND DATE_EXAM=:od");
    q.bindValue(":nd", QDateTime(newDate.startOfDay()));
    q.bindValue(":c",  cin);
    q.bindValue(":od", QDateTime(oldDate.startOfDay()));

    if (!q.exec()) { qDebug() << "modifierDate:" << q.lastError().text(); return false; }
    return true;
}
QSqlQueryModel* Exam::afficherParDate(bool asc)
{
    auto *m = new QSqlQueryModel;
    m->setQuery(QString(
                    "SELECT TYPE, "
                    "       TO_CHAR(DATE_EXAM,'DD/MM/YYYY') AS DATE_AFF, "
                    "       LIEU, RESULTAT, CINC "
                    "FROM EXAMENS "
                    "ORDER BY EXAMENS.DATE_EXAM %1"
                    ).arg(asc ? "ASC" : "DESC"));

    if (m->lastError().isValid())
        qDebug() << "[afficherParDate] SQL:" << m->lastError().text();

    m->setHeaderData(0, Qt::Horizontal, "Type");
    m->setHeaderData(1, Qt::Horizontal, "Date");
    m->setHeaderData(2, Qt::Horizontal, "Lieu");
    m->setHeaderData(3, Qt::Horizontal, "Résultat");
    m->setHeaderData(4, Qt::Horizontal, "CIN");
    return m;
}

