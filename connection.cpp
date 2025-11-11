#include "connection.h"

Connection::Connection()
{
    db=QSqlDatabase::addDatabase("QODBC");
}
Connection::~Connection(){
    if(db.open()){
        db.close();
    }
}

Connection& Connection::createInstance(){
    static Connection instance;
    return instance;
}
bool Connection::createconnect(){
    bool test=false;
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("source_projet2A");//inserer le nom de la source de données
    db.setUserName("youssef");//inserer nom de l'utilisateur
    db.setPassword("yusf1234");//inserer mot de passe de cet utilisateur
    if (db.open()){
        qDebug()<<"connection etablie";
        return true;
    }
    qDebug()<<"echec de la connection:"<<db.lastError().text();
    return  false;
}
