#ifndef CONNECTION_H
#define CONNECTION_H

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>

class Connection
{
public:
    Connection();
    bool createconnect();   // Opens the database connection
    void closeconnect();    // Closes the database connection
private:
    QSqlDatabase db;
};

#endif // CONNECTION_H
