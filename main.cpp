#include "mainwindow.h"
#include <QApplication>
#include <QMessageBox>
#include "connection.h"

void showMessageBox(QMessageBox::Icon icon, const QString &title, const QString &text) {
    QMessageBox msgBox;
    msgBox.setIcon(icon);
    msgBox.setWindowTitle(title);
    msgBox.setText(text);
    msgBox.setStyleSheet(
        "QMessageBox { background-color: #2b2b2b; }"
        "QLabel, QMessageBox QLabel { color: white; }"  // <- this line forces white text
        "QPushButton { background-color: #3c3c3c; color: white; border-radius: 6px; padding: 4px 8px; }"
        );
    msgBox.exec();
}


int main(int argc, char *argv[])
{
    QApplication::setAttribute(Qt::AA_DontUseNativeDialogs);
    QApplication a(argc, argv);

    Connection c;
    bool test = c.createconnect();

    if (test) {
        showMessageBox(QMessageBox::Information, "Database Status", "✅ Connection successful.");

        MainWindow w;
        w.show();
        int result = a.exec();

        c.closeconnect();
        return result;
    } else {
        showMessageBox(QMessageBox::Critical, "Database Error", "❌ Connection failed.\nPlease check your database parameters.");
        return -1;
    }
}
