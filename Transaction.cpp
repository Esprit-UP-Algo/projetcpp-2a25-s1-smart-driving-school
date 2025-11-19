#include "transaction.h"
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlError>
#include <QDebug>
#include <QDate>
#include <QDateTime>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QMessageBox>
#include <QDebug>
// Oracle table and column names
static const char* TABLE_NAME = "TRANSACTION";
static const char* COL_ID     = "ID_T";
static const char* COL_TYPE   = "TYPE";
static const char* COL_DATE   = "DATE_TRANSACTION";
static const char* COL_AMOUNT = "AMOUNT";
static const char* COL_CATEGORY = "CATEGORY";
static const char* COL_METHODE  = "METHODE";

bool Transaction::ajouter() const
{
    // ======== CONTROLES DE SAISIE =========

    // 1. Vérifier que type, catégorie, méthode ne sont pas la valeur par défaut
    if (type.isEmpty() || type == "Choisir" || type == "Select") {
        QMessageBox::warning(nullptr, "Erreur","Type invalide");
        return false;
    }
    if (category.isEmpty() || category == "Choisir" || category == "Select") {
        QMessageBox::warning(nullptr, "Erreur","Catégorie invalide");
        return false;
    }
    if (methode.isEmpty() || methode == "Choisir" || methode == "Select") {
        QMessageBox::warning(nullptr, "Erreur","Méthode invalide");
        return false;
    }

    // 2. Vérifier date > aujourd’hui
    QDate d = QDate::fromString(date, "dd/MM/yyyy");
    if (!d.isValid()) {
        qDebug() << "[Transaction::ajouter] Date invalide:" << date;
        return false;
    }
    if (d <= QDate::currentDate()) {
               QMessageBox::warning(nullptr, "Erreur","Date doit être > date actuelle");
        return false;
    }

    // 3. Vérifier montant > 100
    if (amount <= 100) {
               QMessageBox::warning(nullptr, "Erreur","Montant doit être > 100");
        return false;
    }

    const QString iso = d.toString("yyyy-MM-dd");

    {
        QSqlQuery q;
        q.prepare("INSERT INTO TRANSACTION (TYPE, DATE_TRANSACTION, AMOUNT, CATEGORY, METHODE) "
                  "VALUES (:type, TO_DATE(:date, 'DD/MM/YYYY'), :amount, :category, :methode)");

        q.bindValue(":type", type);
        q.bindValue(":date", date);
        q.bindValue(":amount", amount);
        q.bindValue(":category", category);
        q.bindValue(":methode", methode);

        if (!q.exec()) {
            qDebug() << "[Transaction::ajouter][A] error:" << q.lastError().text();
            QMessageBox::critical(nullptr, "Erreur SQL", q.lastError().text());
            return false;
        }
        return true;


    }
}

bool Transaction::mettreAJour(int id) const
{
    // same validation as ajouter()
    if (type.isEmpty() || type == "Choisir" || type == "Select") {
        QMessageBox::warning(nullptr, "Erreur","Type invalide"); return false;
    }
    if (category.isEmpty() || category == "Choisir" || category == "Select") {
        QMessageBox::warning(nullptr, "Erreur","Catégorie invalide"); return false;
    }
    if (methode.isEmpty() || methode == "Choisir" || methode == "Select") {
        QMessageBox::warning(nullptr, "Erreur","Méthode invalide"); return false;
    }
    QDate d = QDate::fromString(date, "dd/MM/yyyy");
    if (!d.isValid()) { QMessageBox::warning(nullptr, "Erreur","Date invalide"); return false; }
    if (d <= QDate::currentDate()) {
        QMessageBox::warning(nullptr, "Erreur","Date doit être > date actuelle"); return false;
    }
    if (amount <= 100) {
        QMessageBox::warning(nullptr, "Erreur","Montant doit être > 100"); return false;
    }

    // Oracle-friendly UPDATE (convert date)
    QSqlQuery q;
    q.prepare("UPDATE TRANSACTION SET "
              "TYPE=:type, "
              "DATE_TRANSACTION=TO_DATE(:date,'DD/MM/YYYY'), "
              "AMOUNT=:amount, "
              "CATEGORY=:category, "
              "METHODE=:methode "
              "WHERE ID_T=:id");
    q.bindValue(":type", type);
    q.bindValue(":date", date);
    q.bindValue(":amount", amount);
    q.bindValue(":category", category);
    q.bindValue(":methode", methode);
    q.bindValue(":id", id);

    if (!q.exec()) {
        QMessageBox::critical(nullptr, "Erreur SQL", q.lastError().text());
        return false;
    }
    return q.numRowsAffected() > 0;
}

bool Transaction::supprimer(int id_t)
{
    QSqlQuery q;
    q.prepare(QString("DELETE FROM %1 WHERE %2 = :id")
                  .arg(TABLE_NAME).arg(COL_ID));
    q.bindValue(":id", id_t);

    if (!q.exec()) {
        qDebug() << "Transaction::supprimer() error:" << q.lastError().text();
        return false;
    }
    return q.numRowsAffected() > 0;
}


QSqlQueryModel* Transaction::afficherTous()
{
    QSqlQueryModel* model = new QSqlQueryModel();

    model->setQuery("SELECT ID_T, TYPE, "
                    "TO_CHAR(DATE_TRANSACTION,'DD/MM/YYYY') AS DATE_TRANSACTION, "
                    "AMOUNT, CATEGORY, METHODE "
                    "FROM TRANSACTION");

    if (model->lastError().isValid()) {
        qDebug() << "[Transaction::afficherTous] Erreur SQL:"
                 << model->lastError().text();
    }

    model->setHeaderData(0, Qt::Horizontal, "ID");
    model->setHeaderData(1, Qt::Horizontal, "Type");
    model->setHeaderData(2, Qt::Horizontal, "Date");
    model->setHeaderData(3, Qt::Horizontal, "Montant");
    model->setHeaderData(4, Qt::Horizontal, "Catégorie");
    model->setHeaderData(5, Qt::Horizontal, "Méthode");

    return model;
}

QSqlQueryModel* Transaction::afficherParCin(const QString &cin_c)
{
    QSqlQueryModel* model = new QSqlQueryModel();
    QSqlQuery query;

    query.prepare("SELECT ID_T, TYPE, "
                  "TO_CHAR(DATE_TRANSACTION,'DD/MM/YYYY') AS DATE_TRANSACTION, "
                  "AMOUNT, CATEGORY, METHODE "
                  "FROM TRANSACTION "
                  "WHERE ID_T LIKE :cin");
    query.bindValue(":cin", "%" + cin_c + "%");
    query.exec();

    model->setQuery(query);

    if (model->lastError().isValid()) {
        qDebug() << "[Transaction::afficherParCin] Erreur SQL:"
                 << model->lastError().text();
    }

    model->setHeaderData(0, Qt::Horizontal, "ID");
    model->setHeaderData(1, Qt::Horizontal, "Type");
    model->setHeaderData(2, Qt::Horizontal, "Date");
    model->setHeaderData(3, Qt::Horizontal, "Montant");
    model->setHeaderData(4, Qt::Horizontal, "Catégorie");
    model->setHeaderData(5, Qt::Horizontal, "Méthode");

    return model;
}
QSqlQueryModel* Transaction::tri(int tp)
{
    QSqlQueryModel *model = new QSqlQueryModel();
    QString orderByClause;

    switch(tp) {
    case 1:
        orderByClause = "ID_T ASC"; // Default sort by ID
        break;
    case 2:
        orderByClause = "DATE_TRANSACTION DESC"; // Sort by date
        break;
    case 3:
        orderByClause = "TYPE ASC"; // Sort by type
        break;
    case 4:
        orderByClause = "ID_T ASC"; // Sort by ID
        break;
    case 5:
        orderByClause = "AMOUNT DESC"; // Sort by amount (highest first)
        break;
    default:
        qDebug() << "Erreur : Type de tri invalide.";
        delete model;
        return nullptr;
    }

    QString queryString = QString("SELECT ID_T, TYPE, "
                                  "TO_CHAR(DATE_TRANSACTION,'DD/MM/YYYY') AS DATE_TRANSACTION, "
                                  "AMOUNT, CATEGORY, METHODE "
                                  "FROM TRANSACTION "
                                  "ORDER BY %1").arg(orderByClause);

    model->setQuery(queryString);

    if (model->lastError().isValid()) {
        qDebug() << "[Transaction::tri] Erreur SQL:" << model->lastError().text();
    }

    model->setHeaderData(0, Qt::Horizontal, "ID");
    model->setHeaderData(1, Qt::Horizontal, "Type");
    model->setHeaderData(2, Qt::Horizontal, "Date");
    model->setHeaderData(3, Qt::Horizontal, "Montant");
    model->setHeaderData(4, Qt::Horizontal, "Catégorie");
    model->setHeaderData(5, Qt::Horizontal, "Méthode");

    return model;
}
QSqlQueryModel* Transaction::afficherStat() {
    QSqlQueryModel *model = new QSqlQueryModel();

    // Oracle syntax uses EXTRACT(YEAR FROM date) instead of YEAR(date)
    model->setQuery("SELECT EXTRACT(YEAR FROM DATE_TRANSACTION) AS ANNEE, "
                    "SUM(AMOUNT) AS TOTAL "
                    "FROM TRANSACTION "
                    "GROUP BY EXTRACT(YEAR FROM DATE_TRANSACTION) "
                    "ORDER BY ANNEE DESC");

    if (model->lastError().isValid()) {
        qDebug() << "[Transaction::afficherStat] Erreur SQL:" << model->lastError().text();
    }

    model->setHeaderData(0, Qt::Horizontal, "Année");
    model->setHeaderData(1, Qt::Horizontal, "Total (DT)");

    return model;
}
