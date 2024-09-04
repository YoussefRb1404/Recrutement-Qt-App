#ifndef LOGINC_H
#define LOGINC_H

#include <QAbstractItemModel>

#include <QString>
#include <QSqlQuery>
#include <QSqlQueryModel>

class loginC
{
public:
    // Setters
    void setid(const int n);
    void setmdp(const int n); // Passer par référence constante
     // Passer par référence constante

    // Getters
    int getid() const;
    int getmdp() const;

    loginC() {} // Constructeur par défaut
    loginC(int, int);

private:
    int id;
    int mdp;
};
#endif
