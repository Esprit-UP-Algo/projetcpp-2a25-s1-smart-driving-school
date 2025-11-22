#ifndef CONNECTION_H
#define CONNECTION_H
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QDebug>
class Connection
{
public:
    static Connection& createInstance();
    bool createconnect();
    QSqlDatabase& getDatabase() { return db; }
    Connection(const Connection&)= delete;
    Connection& operator=(const Connection&)=delete;
private:
    QSqlDatabase db;
    Connection();
    ~Connection();

};

#endif // CONNECTION_H
