#ifndef EXAM_H
#define EXAM_H

#include <QString>
#include <QList>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlQueryModel>
#include <QDebug>

// on travaille : classe simple, C++ basique
class Exam
{
public:
    Exam() = default;
    Exam(const QString &type,
         const QString &date_ddMMyyyy,
         const QString &lieu,
         const QString &resultat,
         const QString &cin)
        : type(type), date(date_ddMMyyyy), lieu(lieu), resultat(resultat), cin(cin) {}

    // CRUD
    bool ajouter() const;                                 // INSERT
    static bool supprimer(const QString &cin, const QString &date_ddMMyyyy); // DELETE par (CINC, DATE_EXAM)

    // Modèles pour QTableView
    static QSqlQueryModel* afficherTous();                // SELECT *
    static QSqlQueryModel* afficherParCin(const QString &cin); // SELECT filtré
    static bool modifierTexte(const QString &cin, const QDate &oldDate, const QString &col, const QString &val);
    static bool modifierDate(const QString &cin, const QDate &oldDate,const QDate &newDate);
    static QSqlQueryModel* afficherParDate(bool asc);

private:
    QString type, date, lieu, resultat, cin;
};

#endif // EXAM_H
