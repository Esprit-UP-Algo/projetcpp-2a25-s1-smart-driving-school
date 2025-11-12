#include "connection.h"

Connection::Connection() {}

bool Connection::createconnect()
{
    db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("projet2A");   // Name of your ODBC source
    db.setUserName("beha");           // Your DB username
    db.setPassword("bouha");          // Your DB password

    if (db.open()) {
        qDebug() << "✅ Database connection successful!";
        return true;
    } else {
        qDebug() << "❌ Database connection failed: " << db.lastError().text();
        return false;
    }
}

void Connection::closeconnect()
{
    if (db.isOpen()) {
        db.close();
        qDebug() << "🔒 Database connection closed.";
    }
}
