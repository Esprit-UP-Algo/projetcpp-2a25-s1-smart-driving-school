#include "logindialog.h"
#include "ui_logindialog.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>

LoginDialog::LoginDialog(QWidget *parent)
    : QDialog(parent), ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
    setWindowTitle("Connexion");

    connect(ui->btnLogin,  &QPushButton::clicked, this, &LoginDialog::on_btnLogin_clicked);
    connect(ui->btnCancel, &QPushButton::clicked, this, &LoginDialog::on_btnCancel_clicked);
}

LoginDialog::~LoginDialog() { delete ui; }

void LoginDialog::on_btnLogin_clicked()
{
    const QString user = ui->editUser->text().trimmed();
    const QString pass = ui->editPass->text();

    if (user.isEmpty() || pass.isEmpty()) {
        QMessageBox::warning(this, "Login", "Veuillez saisir identifiant et mot de passe.");
        return;
    }

    QSqlQuery q;
    q.prepare(R"(
        SELECT ROLE
        FROM MALIK.USERS
        WHERE LOWER(USERNAME) = LOWER(:u) AND PASSWORD = :p
    )");


    q.bindValue(":u", user);
    q.bindValue(":p", pass);

    if (!q.exec()) {
        QMessageBox::critical(this, "Login", "Erreur SQL:\n" + q.lastError().text());
        return;
    }

    if (!q.next()) {
        QMessageBox::warning(this, "Login", "Identifiants incorrects.");
        return;
    }

    const QString roleStr = q.value(0).toString();

    if (roleStr.compare("Admin", Qt::CaseInsensitive) == 0) {
        selectedRole_ = Role::Admin;
    } else if (roleStr.compare("Moniteur", Qt::CaseInsensitive) == 0) {
        selectedRole_ = Role::Moniteur;
    } else {
        QMessageBox::warning(this, "Login", "Rôle inconnu dans la base.");
        return;
    }

    accept();
}

void LoginDialog::on_btnCancel_clicked()
{
    reject();
}
