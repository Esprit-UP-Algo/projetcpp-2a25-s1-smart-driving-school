#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include "role.h"

namespace Ui { class LoginDialog; }

class LoginDialog : public QDialog
{
    Q_OBJECT
public:
    explicit LoginDialog(QWidget *parent = nullptr);
    ~LoginDialog();

    Role selectedRole() const { return selectedRole_; }

private slots:
    void on_btnLogin_clicked();
    void on_btnCancel_clicked();

private:
    Ui::LoginDialog *ui= nullptr;
    Role selectedRole_ = Role::None;
};

#endif // LOGINDIALOG_H
