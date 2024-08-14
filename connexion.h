//DoNE
#ifndef CONNEXION_H
#define CONNEXION_H
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>



class connexion
{
    QSqlDatabase db;
public:
    connexion();
    bool createconnect();
    void closeConnection();

};

#endif // CONNEXION_H
