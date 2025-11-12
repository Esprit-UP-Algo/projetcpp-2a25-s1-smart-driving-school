#include "moniteur.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QRegularExpression>
#include <QDebug>
#include <QMessageBox>

Moniteur::Moniteur() {}

Moniteur::Moniteur(QString nom, QString prenom, QString cin,
                   QString tel, QString mail, QString voiture)
{
    this->nom = nom;
    this->prenom = prenom;
    this->cin = cin;
    this->tel = tel;
    this->mail = mail;
    this->voiture = voiture;
}

// ✅ Validation Rules
bool validerNomPrenom(QString s) {
    return QRegularExpression("^[A-Za-z]{2,}$").match(s).hasMatch();
}
bool validerCIN(QString s) {
    return QRegularExpression("^\\d{8}$").match(s).hasMatch();
}
bool validerTel(QString s) {
    return QRegularExpression("^\\d{8}$").match(s).hasMatch();
}
bool validerMail(QString s) {
    return QRegularExpression("^[\\w\\.]+@[\\w\\.]+\\.[A-Za-z]{2,4}$").match(s).hasMatch();
}

// CREATE ✅ + Validation
bool Moniteur::ajouter()
{
    // Empty field check
    if (nom.trimmed().isEmpty()) {
        QMessageBox::warning(nullptr, "Erreur", "Le champ NOM ne peut pas être vide.");
        return false;
    }
    if (prenom.trimmed().isEmpty()) {
        QMessageBox::warning(nullptr, "Erreur", "Le champ PRENOM ne peut pas être vide.");
        return false;
    }
    if (cin.trimmed().isEmpty()) {
        QMessageBox::warning(nullptr, "Erreur", "Le champ CIN ne peut pas être vide.");
        return false;
    }
    if (tel.trimmed().isEmpty()) {
        QMessageBox::warning(nullptr, "Erreur", "Le champ TEL ne peut pas être vide.");
        return false;
    }
    if (mail.trimmed().isEmpty()) {
        QMessageBox::warning(nullptr, "Erreur", "Le champ MAIL ne peut pas être vide.");
        return false;
    }
    if (voiture.trimmed().isEmpty()) {
        QMessageBox::warning(nullptr, "Erreur", "Le champ VOITURE ne peut pas être vide.");
        return false;
    }

    // Field validation
    if (!validerNomPrenom(nom)) {
        QMessageBox::warning(nullptr, "Erreur", "Nom invalide. Le nom doit contenir uniquement des lettres.");
        return false;
    }
    if (!validerNomPrenom(prenom)) {
        QMessageBox::warning(nullptr, "Erreur", "Prénom invalide.");
        return false;
    }
    if (!validerCIN(cin)) {
        QMessageBox::warning(nullptr, "Erreur", "CIN invalide. Il faut 8 chiffres.");
        return false;
    }
    if (!validerTel(tel)) {
        QMessageBox::warning(nullptr, "Erreur", "Numéro de téléphone invalide (8 chiffres).");
        return false;
    }
    if (!validerMail(mail)) {
        QMessageBox::warning(nullptr, "Erreur", "Adresse mail invalide.");
        return false;
    }

    // Insert into DB
    QSqlQuery query;
    query.prepare("INSERT INTO moniteur (NOM, PRENOM, CIN, TEL, MAIL, VOITURE) "
                  "VALUES (:nom, :prenom, :cin, :tel, :mail, :voiture)");

    query.bindValue(":nom", nom);
    query.bindValue(":prenom", prenom);
    query.bindValue(":cin", cin);
    query.bindValue(":tel", tel);
    query.bindValue(":mail", mail);
    query.bindValue(":voiture", voiture);

    if (!query.exec()) {
        QMessageBox::critical(nullptr, "Erreur", "Ajout échoué: " + query.lastError().text());
        return false;
    }

    return true;
}



// UPDATE ✅ + Validation
bool Moniteur::modifier(int id, QString champ, QString valeur)
{
    // Validation spécifique selon le champ modifié
    if (champ == "NOM" && !validerNomPrenom(valeur)) {
        QMessageBox::warning(nullptr, "Erreur de saisie", "Le nom est invalide ! (lettres uniquement)");
        return false;
    }
    if (champ == "PRENOM" && !validerNomPrenom(valeur)) {
        QMessageBox::warning(nullptr, "Erreur de saisie", "Le prénom est invalide ! (lettres uniquement)");
        return false;
    }
    if (champ == "CIN" && !validerCIN(valeur)) {
        QMessageBox::warning(nullptr, "Erreur de saisie", "Le CIN doit contenir exactement 8 chiffres !");
        return false;
    }
    if (champ == "TEL" && !validerTel(valeur)) {
        QMessageBox::warning(nullptr, "Erreur de saisie", "Le téléphone doit contenir exactement 8 chiffres !");
        return false;
    }
    if (champ == "MAIL" && !validerMail(valeur)) {
        QMessageBox::warning(nullptr, "Erreur de saisie", "L'adresse e-mail est invalide !");
        return false;
    }

    QSqlQuery query;
    QString req = "UPDATE moniteur SET " + champ + " = :val WHERE ID = :id";
    query.prepare(req);
    query.bindValue(":val", valeur);
    query.bindValue(":id", id);

    if (!query.exec()) {
        qDebug() << "❌ Modification échouée :" << query.lastError().text();
        QMessageBox::critical(nullptr, "Erreur SQL", query.lastError().text());
        return false;
    }

    return true;
}

// DELETE ✅
bool Moniteur::supprimer(int id)
{
    QSqlQuery query;
    query.prepare("DELETE FROM moniteur WHERE ID = :id");
    query.bindValue(":id", id);

    if (!query.exec()) {
        qDebug() << "❌ Suppression échouée :" << query.lastError().text();
        return false;
    }

    return true;
}
