#include "connection.h"

Connection::Connection()
{
    db=QSqlDatabase::addDatabase("QODBC");
}
Connection::~Connection(){
    if(db.isOpen()){
        db.close();
    }
}

Connection& Connection::createInstance(){
    static Connection instance;
    return instance;
}
bool Connection::createconnect(){
    db.setDatabaseName("source_projet2A");
    db.setUserName("system");
    db.setPassword("yusf1234");
    if (db.open()){
        qDebug()<<"connection etablie";
        return true;
    }
    qDebug()<<"echec de la connection:"<<db.lastError().text();
    return  false;
}
