#include "connection.h"

Connection::Connection()
{

}

bool Connection::createconnect()
{bool test=false;
QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
db.setDatabaseName("projet_cpp");//inserer le nom de la source de données
db.setUserName("hamma");//inserer nom de l'utilisateur
db.setPassword("hamma2005");//inserer mot de passe de cet utilisateur

if (db.open())
test=true;





    return  test;
}
