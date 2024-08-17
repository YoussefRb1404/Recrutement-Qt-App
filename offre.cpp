#include "offre.h"
#include <QtWidgets>
#include <QTableWidget>
#include <QSqlError>
#include <gesoff.h>

// Constructeur de la classe offre
offre::offre(int id, QString type, QString nomE, QString nomP, int postV, int salaire, QString experience, QDate dateE)
{
    this->id = id;
    this->type = type;
    this->nomE = nomE;
    this->nomP = nomP;
    this->posteV = postV;
    this->salaire = salaire;
    this->experience = experience;
    this->dateE = dateE;
}

// Méthodes setter
void offre::setid(const int n) { id = n; }
void offre::settype(const QString &n) { type = n; }
void offre::setnomE(const QString &n) { nomE = n; }
void offre::setnomP(const QString &n) { nomP = n; }
void offre::setpostV(const int n) { posteV = n; }
void offre::setsalaire(const int n) { salaire = n; }
void offre::setexperience(const QString &n) { experience = n; }
void offre::setdateE(const QDate &n) { dateE = n; }

// Méthode pour ajouter une offre à la base de données
bool offre::ajouter() {
    QSqlQuery query;
    QString res = QString::number(id);
    query.prepare("INSERT INTO OFFRES (ido, type, nome, nomp, postesv, salaire, experience, datee) "
                  "VALUES (:ido, :type, :nome, :nomp, :postesv, :salaire, :experience, :datee)");
    query.bindValue(":ido", res);
    query.bindValue(":type", type);
    query.bindValue(":nome", nomE);
    query.bindValue(":nomp", nomP);
    query.bindValue(":postesv", posteV);
    query.bindValue(":salaire", salaire);
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
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("NomP"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("PostesV"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Salaire"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Experience"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("DateE"));

    return model;
}

bool offre::modifier() {
    QSqlQuery query;
    QString res = QString::number(id);

    query.prepare("UPDATE OFFRES SET type = :type, nomE = :nomE, nomP = :nomP, postesV = :postesV, salaire = :salaire, experience = :experience, dateE = :dateE "
                  "WHERE ido = :ido");

    query.bindValue(":ido", res);
    query.bindValue(":type", type);
    query.bindValue(":nomE", nomE);
    query.bindValue(":nomP", nomP);
    query.bindValue(":postesV", posteV);
    query.bindValue(":salaire", salaire);
    query.bindValue(":experience", experience);
    query.bindValue(":dateE", dateE);

    return query.exec();
}

bool offre::recherche(const QString &nomP) const {
    QSqlQuery query;
    query.prepare("SELECT * FROM offres WHERE nomP = :nomP");
    query.bindValue(":nomP", nomP);

    if (!query.exec()) {
        qWarning() << "Database error during recherche:" << query.lastError().text();
        return false;
    }

    return query.next();  // Returns true if a record is found
}


// Tri par ID Ascendant
QSqlQueryModel* offre::afficherTriId() {
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM OFFRES ORDER BY ido ASC");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Type"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("NomE"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("NomP"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("PostesV"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Salaire"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Experience"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("DateE"));

    return model;
}

// Tri par ID Descendant
QSqlQueryModel* offre::afficherTriIdD() {
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM OFFRES ORDER BY ido DESC");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Type"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("NomE"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("NomP"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("PostesV"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Salaire"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Experience"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("DateE"));

    return model;
}

// Tri par Type Ascendant
QSqlQueryModel* offre::afficherTriType() {
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM OFFRES ORDER BY type ASC");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Type"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("NomE"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("NomP"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("PostesV"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Salaire"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Experience"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("DateE"));

    return model;
}

// Tri par Type Descendant
QSqlQueryModel* offre::afficherTriTypeD() {
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM OFFRES ORDER BY type DESC");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Type"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("NomE"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("NomP"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("PostesV"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Salaire"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Experience"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("DateE"));

    return model;
}

// Tri par NomE Ascendant
QSqlQueryModel* offre::afficherTriNomE() {
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM OFFRES ORDER BY nomE ASC");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Type"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("NomE"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("NomP"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("PostesV"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Salaire"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Experience"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("DateE"));

    return model;
}

// Tri par NomE Descendant
QSqlQueryModel* offre::afficherTriNomED() {
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM OFFRES ORDER BY nomE DESC");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Type"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("NomE"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("NomP"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("PostesV"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Salaire"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Experience"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("DateE"));

    return model;}

    // Tri par Salaire Descendant (High to Low)
    QSqlQueryModel* offre::afficherTriSalaireH() {
        QSqlQueryModel* model = new QSqlQueryModel();
        model->setQuery("SELECT * FROM OFFRES ORDER BY salaire DESC");
        model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
        model->setHeaderData(1, Qt::Horizontal, QObject::tr("Type"));
        model->setHeaderData(2, Qt::Horizontal, QObject::tr("NomE"));
        model->setHeaderData(3, Qt::Horizontal, QObject::tr("NomP"));
        model->setHeaderData(4, Qt::Horizontal, QObject::tr("PostesV"));
        model->setHeaderData(5, Qt::Horizontal, QObject::tr("Salaire"));
        model->setHeaderData(6, Qt::Horizontal, QObject::tr("Experience"));
        model->setHeaderData(7, Qt::Horizontal, QObject::tr("DateE"));

        return model;
    }

    // Tri par Salaire Ascendant (Low to High)
    QSqlQueryModel* offre::afficherTriSalaireB() {
        QSqlQueryModel* model = new QSqlQueryModel();
        model->setQuery("SELECT * FROM OFFRES ORDER BY salaire ASC");
        model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
        model->setHeaderData(1, Qt::Horizontal, QObject::tr("Type"));
        model->setHeaderData(2, Qt::Horizontal, QObject::tr("NomE"));
        model->setHeaderData(3, Qt::Horizontal, QObject::tr("NomP"));
        model->setHeaderData(4, Qt::Horizontal, QObject::tr("PostesV"));
        model->setHeaderData(5, Qt::Horizontal, QObject::tr("Salaire"));
        model->setHeaderData(6, Qt::Horizontal, QObject::tr("Experience"));
        model->setHeaderData(7, Qt::Horizontal, QObject::tr("DateE"));

        return model;
    }



// Tri par PostesV Ascendant
QSqlQueryModel* offre::afficherTriPostesV() {
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM OFFRES ORDER BY postesv ASC");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Type"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("NomE"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("NomP"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("PostesV"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Salaire"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Experience"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("DateE"));

    return model;
}

// Tri par PostesV Descendant
QSqlQueryModel* offre::afficherTriPostesVD() {
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM OFFRES ORDER BY postesv DESC");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Type"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("NomE"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("NomP"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("PostesV"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Salaire"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Experience"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("DateE"));

    return model;
}

// Tri par NomP Ascendant
QSqlQueryModel* offre::afficherTriNomP() {
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM OFFRES ORDER BY nomp ASC");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Type"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("NomE"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("NomP"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("PostesV"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Salaire"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Experience"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("DateE"));

    return model;
}

// Tri par NomP Descendant
QSqlQueryModel* offre::afficherTriNomPD() {
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM OFFRES ORDER BY nomp DESC");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Type"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("NomE"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("NomP"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("PostesV"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Salaire"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Experience"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("DateE"));

    return model;
}

