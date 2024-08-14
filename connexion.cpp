#include "connexion.h"

connexion::connexion()
{

}
bool connexion::createconnect()
{
    db = QSqlDatabase::addDatabase("QODBC");
    bool test=false;
QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
db.setDatabaseName("Source_Projet2A");
db.setUserName("Rebai");//inserer nom de l'utilisateur
db.setPassword("esprit18");//inserer mot de passe de cet utilisateur

if (db.open()) test=true;
test=true;





    return  test;
}
void connexion::closeConnection(){db.close();}
