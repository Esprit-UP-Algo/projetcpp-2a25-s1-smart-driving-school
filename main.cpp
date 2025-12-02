#include "mainwindow.h"
#include "mainwindowV.h"
#include "connection.h"
#include "connexion.h"
#include "logindialog.h"
#include "role.h"

#include <QApplication>
#include <QMessageBox>
#include <QDebug>
#include <QPushButton>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // 1) Establish database connection using Connexion class
    Connexion c;
    bool connected = c.ouvrirConnexion();

    // If first method fails, try Connection class
    if (!connected) {
        qDebug() << "Trying alternative connection method...";
        Connection& conn = Connection::createInstance();
        connected = conn.createconnect();
    }

    // If both failed, exit
    if (!connected) {
        QMessageBox::critical(
            nullptr,
            QObject::tr("Échec de connexion"),
            QObject::tr("Impossible de se connecter à la base de données.\n"
                        "Vérifiez vos paramètres de connexion.")
            );
        return -1;
    }

    qDebug() << "Connexion établie avec succès!";

    // 2) Show login dialog
    LoginDialog loginDlg;
    if (loginDlg.exec() != QDialog::Accepted) {
        qDebug() << "Login cancelled by user";
        return 0;  // User cancelled login
    }

    Role userRole = loginDlg.selectedRole();
    qDebug() << "User logged in with role:" << static_cast<int>(userRole);

    // 3) Show the main window
    MainWindow w;
    w.setRole(userRole);
    w.setWindowTitle(userRole == Role::Admin ?
                         "Gestion - Admin" :
                         "Gestion - Moniteur");

    // Connect vehicle button to open vehicle window
    MainWindowV *vehicleWindow = nullptr;
    QObject::connect(w.findChild<QPushButton*>("btnVehicule"), &QPushButton::clicked, [&](){
        if (!vehicleWindow) {
            vehicleWindow = new MainWindowV();
            vehicleWindow->setAttribute(Qt::WA_DeleteOnClose);
            QObject::connect(vehicleWindow, &QObject::destroyed, [&](){ vehicleWindow = nullptr; });
        }
        vehicleWindow->show();
        vehicleWindow->raise();
        vehicleWindow->activateWindow();
    });

    w.show();

    return a.exec();
}
