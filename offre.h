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
    void setnomP(const QString &n);
    void setpostV(const int n);
    void setsalaire(const int n);
    void setexperience(const QString &n); // Passer par référence constante
    void setdateE(const QDate &n); // Passer par référence constante

    // Getters
    int getid() const;
    QString gettype() const;
    QString getnomE() const;
    QString getnomP() const;
    int getpostV() const;
    int getsalaire() const;
    QString getexperience() const;
    QDate getdateE() const;

    // Constructeurs
    offre() {} // Constructeur par défaut
    offre(int, QString, QString, QString, int, int, QString, QDate);

    // Méthodes
    bool ajouter();
    bool supprimer(QString);
    bool modifier();
    QSqlQueryModel* afficher();


    bool recherche(const QString &nomP) const;

    bool rechercheParNomP(const QString &input);
    QSqlQueryModel * afficherTriId();
    QSqlQueryModel * afficherTriIdD();
    QSqlQueryModel * afficherTriType();
    QSqlQueryModel * afficherTriTypeD();
    QSqlQueryModel * afficherTriNomE();
    QSqlQueryModel * afficherTriNomED();
    QSqlQueryModel * afficherTriSalaireH();
    QSqlQueryModel * afficherTriSalaireB();
    QSqlQueryModel * afficherTriPostesV();
    QSqlQueryModel * afficherTriPostesVD();
    QSqlQueryModel * afficherTriNomP();
    QSqlQueryModel * afficherTriNomPD();


private:
    int id;
    QString type;
    QString nomE;
    QString nomP;
    int posteV;
    int salaire;
    QString experience;
    QDate dateE;
};

#endif // OFFRE_H
