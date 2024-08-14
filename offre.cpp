#include "offre.h"
#include <QtWidgets>
#include <QTableWidget>
#include <QSqlError>
#include <gesoff.h>

// Constructeur de la classe offre
offre::offre(int id, QString type, QString nomE, int postV, int salaire, QString experience, QDate dateE)
{
    this->id = id;
    this->type = type;
    this->nomE = nomE;
    this->posteV = postV;
    this->salaire = salaire;
    this->experience = experience;
    this->dateE = dateE;
}

// Méthodes setter
void offre::setid(const int n) { id = n; }
void offre::settype(const QString &n) { type = n; }
void offre::setnomE(const QString &n) { nomE = n; }
void offre::setpostV(const int n) { posteV = n; }
void offre::setsalaire(const int n) { salaire = n; }
void offre::setexperience(const QString &n) { experience = n; }
void offre::setdateE(const QDate &n) { dateE = n; }

// Méthode pour ajouter une offre à la base de données
bool offre::ajouter() {
    QSqlQuery query;
    QString res = QString::number(id);
    query.prepare("INSERT INTO OFFRES (ido, type, nome, postesv, salaire, experience, datee) "
                  "VALUES (:ido, :type, :nome, :postesv, :salaire, :experience, :datee)");
    query.bindValue(":ido", res);
    query.bindValue(":type", type);
    query.bindValue(":nome", nomE);
    query.bindValue(":postesv", posteV);
    query.bindValue(":salaire", salaire);  // Correction de la faute de frappe ici
    query.bindValue(":experience", experience);
    query.bindValue(":datee", dateE);

    if (!query.exec()) {
        qWarning() << "Erreur lors de l'ajout de l'offre:" << query.lastError().text();
        return false;
    }
    return true;
}

// Méthode pour supprimer une offre de la base de données
bool offre::supprimer(QString id) {
    QSqlQuery query;
    query.prepare("DELETE FROM OFFRES WHERE ido = :id");
    query.bindValue(":id", id);

    if (!query.exec()) {
        qWarning() << "Erreur lors de la suppression de l'offre:" << query.lastError().text();
        return false;
    }
    return true;
}

// Méthode pour afficher les offres
QSqlQueryModel* offre::afficher() {
    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM OFFRES");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Type"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("NomE"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("PosteV"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Salaire"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Experience"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("DateE"));

    return model;
}


bool offre::modifier() {
    QSqlQuery query;
    QString res = QString::number(id);

    query.prepare("UPDATE OFFRES SET type = :type, nomE = :nomE, postesV = :postesV, salaire = :salaire, experience = :experience, dateE = :dateE "
                  "WHERE ido = :ido");

    query.bindValue(":ido", res);
    query.bindValue(":type", type);
    query.bindValue(":nomE", nomE);  // Corrected from nome to nomE
    query.bindValue(":postesV", posteV);  // Corrected from postesv to postesV
    query.bindValue(":salaire", salaire);
    query.bindValue(":experience", experience);
    query.bindValue(":dateE", dateE);

    return query.exec();
}

