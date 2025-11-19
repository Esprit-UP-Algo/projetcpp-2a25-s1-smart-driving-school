#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <QString>
#include <QSqlQueryModel>

class Transaction
{
private:
    int id_t;
    QString type;
    QString date;              // Format: "dd/MM/yyyy"
    double amount;
    QString category;
    QString methode;
    QString cin_c;

public:
    // Constructors
    Transaction() : id_t(0), amount(0.0) {}

    Transaction(int id, const QString &t, const QString &d, double amt,
                const QString &cat, const QString &meth, const QString &cin)
        : id_t(id), type(t), date(d), amount(amt),
        category(cat), methode(meth), cin_c(cin) {}

    // Getters
    int getIdT() const { return id_t; }
    QString getType() const { return type; }
    QString getDate() const { return date; }
    double getAmount() const { return amount; }
    QString getCategory() const { return category; }
    QString getMethode() const { return methode; }
    QString getCinC() const { return cin_c; }

    // Setters
    void setIdT(int id) { id_t = id; }
    void setType(const QString &t) { type = t; }
    void setDate(const QString &d) { date = d; }
    void setAmount(double amt) { amount = amt; }
    void setCategory(const QString &cat) { category = cat; }
    void setMethode(const QString &meth) { methode = meth; }
    void setCinC(const QString &cin) { cin_c = cin; }

    // CRUD Operations
    bool ajouter() const;
    bool mettreAJour(int id) const;
    static bool supprimer(int id_t);
    static QSqlQueryModel* afficherTous();
    static QSqlQueryModel* afficherParCin(const QString &cin_c);
    static QSqlQueryModel* tri(int tp);
static QSqlQueryModel* afficherStat();
};

#endif // TRANSACTION_H
