#ifndef CANDIDAT_H
#define CANDIDAT_H

#include <QString>
#include <QDate>
#include <QSqlQuery>
#include <QSqlQueryModel>

class candidat
{
public:
    // Setters
    void setId(const int n);
    void setNom(const QString &n);
    void setPrenom(const QString &n);
    void setTelephone(const int n);
    void setPoste(const QString &n);
    void setExperience(const int n);
    void setMail(const QString &n);
    void setDateN(const QDate &n);

    // Getters
    int getId() const;
    QString getNom() const;
    QString getPrenom() const;
    int getTelephone() const;
    QString getPoste() const;
    int getExperience() const;
    QString getMail() const;
    QDate getDateN() const;

    // Constructeurs
    candidat() {} // Constructeur par défaut
    candidat(int, QString, QString, int, QString, int, QString, QDate);

    // Méthodes
    bool ajouter();
    bool supprimer(QString);
    bool modifier();
    QSqlQueryModel* afficher();

    bool recherche(const QDate &dateN) const;

    QSqlQueryModel * afficherTriNomA();
    QSqlQueryModel * afficherTriNomD();
    QSqlQueryModel * afficherTriPrenomA();
    QSqlQueryModel * afficherTriPrenomD();
    QSqlQueryModel * afficherTriDateNA();
    QSqlQueryModel * afficherTriDateND();
    QSqlQueryModel *afficherCandidatsArchives();

    bool archiver(const QString &id);
    bool desarchiver();



private:
    int id;
    QString nom;
    QString prenom;
    int telephone;
    QString poste;
    int experience;
    QString mail;
    QDate dateN;
};

#endif // CANDIDAT_H
