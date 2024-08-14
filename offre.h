#ifndef OFFRE_H
#define OFFRE_H

#include <QString>
#include <QDate>
#include <QSqlQuery>
#include <QSqlQueryModel>

class offre
{
public:
    // Setters
    void setid(const int n);
    void settype(const QString &n); // Passer par référence constante
    void setnomE(const QString &n); // Passer par référence constante
    void setpostV(const int n);
    void setsalaire(const int n);
    void setexperience(const QString &n); // Passer par référence constante
    void setdateE(const QDate &n); // Passer par référence constante

    // Getters
    int getid() const;
    QString gettype() const;
    QString getnomE() const;
    int getpostV() const;
    int getsalaire() const;
    QString getexperience() const;
    QDate getdateE() const;

    // Constructeurs
    offre() {} // Constructeur par défaut
    offre(int, QString, QString, int, int, QString, QDate);

    // Méthodes
    bool ajouter();
    bool supprimer(QString);
    bool modifier();
    QSqlQueryModel* afficher();

private:
    int id;
    QString type;
    QString nomE;
    int posteV;
    int salaire;
    QString experience;
    QDate dateE;
};

#endif // OFFRE_H
