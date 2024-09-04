#include "candidat.h"
#include <QtWidgets>
#include <QTableWidget>
#include <QSqlError>
#include <gescond.h>

// Constructor
candidat::candidat(int id, QString nom, QString prenom, int telephone, QString poste, int experience, QString mail, QDate dateN)
{
    this->id = id;
    this->nom = nom;
    this->prenom = prenom;
    this->telephone = telephone;
    this->poste = poste;
    this->experience = experience;
    this->mail = mail;
    this->dateN = dateN;

}

// Méthodes setter
void candidat::setId(const int n) { id = n; }
void candidat::setNom(const QString &n) { nom = n; }
void candidat::setPrenom(const QString &n) { prenom = n; }
void candidat::setTelephone(const int n) { telephone = n; }
void candidat::setPoste(const QString &n) { poste = n; }
void candidat::setExperience(const int n) { experience = n; }
void candidat::setMail(const QString &n) { mail = n; }
void candidat::setDateN(const QDate &n) { dateN = n; }

// Méthode pour ajouter un candidat à la base de données
bool candidat::ajouter() {
    QSqlQuery query;
    QString res = QString::number(id);
    query.prepare("INSERT INTO CANDIDATS (idc, nom, prenom, telephone, poste, experience, mail, daten, estarchive) "
                  "VALUES (:idc, :nom, :prenom, :telephone, :poste, :experience, :mail, :daten, :est)");
    query.bindValue(":idc", res);
    query.bindValue(":nom", nom);
    query.bindValue(":prenom", prenom);
    query.bindValue(":telephone", telephone);
    query.bindValue(":poste", poste);
    query.bindValue(":experience", experience);
    query.bindValue(":mail", mail);
    query.bindValue(":daten", dateN);
    query.bindValue(":est", 0);

    if (!query.exec()) {
        qWarning() << "Erreur lors de l'ajout du candidat:" << query.lastError().text();
        return false;
    }
    return true;
}

// Méthode pour supprimer un candidat de la base de données
bool candidat::supprimer(QString id) {
    QSqlQuery query;
    query.prepare("DELETE FROM CANDIDATS WHERE idc = :id");
    query.bindValue(":id", id);

    if (!query.exec()) {
        qWarning() << "Erreur lors de la suppression du candidat:" << query.lastError().text();
        return false;
    }
    return true;
}

// Méthode pour afficher les candidats
QSqlQueryModel* candidat::afficher() {
    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM CANDIDATS");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Prenom"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Téléphone"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Poste"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Expérience"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Mail"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("Date de Naissance"));

    return model;
}

// Méthode pour modifier un candidat dans la base de données
bool candidat::modifier() {
    QSqlQuery query;
    QString res = QString::number(id);

    query.prepare("UPDATE CANDIDATS SET nom = :nom, prenom = :prenom, telephone = :telephone, poste = :poste, "
                  "experience = :experience, mail = :mail, daten = :daten WHERE idc = :idc");

    query.bindValue(":idc", res);
    query.bindValue(":nom", nom);
    query.bindValue(":prenom", prenom);
    query.bindValue(":telephone", telephone);
    query.bindValue(":poste", poste);
    query.bindValue(":experience", experience);
    query.bindValue(":mail", mail);
    query.bindValue(":daten", dateN);

    return query.exec();
}

// Méthode pour rechercher un candidat par nom
bool candidat::recherche(const QDate &dateN) const {
    QSqlQuery query;
    query.prepare("SELECT * FROM CANDIDATS WHERE daten = :daten");
    query.bindValue(":daten", dateN.toString("dd-MM-yyyy"));  // Assurez-vous que le format correspond à celui de votre base de données

    if (!query.exec()) {
        qWarning() << "Erreur lors de la recherche du candidat:" << query.lastError().text();
        return false;
    }

    return query.next();  // Renvoie true si un enregistrement est trouvé
}

QSqlQueryModel* candidat::afficherTriNomA() {
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM CANDIDATS ORDER BY NOM ASC");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Prenom"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Téléphone"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Poste"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Expérience"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Mail"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("Date de Naissance"));

    return model;
}
QSqlQueryModel* candidat::afficherTriNomD() {
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM CANDIDATS ORDER BY NOM DESC");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Prenom"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Téléphone"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Poste"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Expérience"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Mail"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("Date de Naissance"));

    return model;
}

QSqlQueryModel* candidat::afficherTriPrenomA() {
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM CANDIDATS ORDER BY PRENOM ASC");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Prenom"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Téléphone"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Poste"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Expérience"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Mail"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("Date de Naissance"));

    return model;
}
QSqlQueryModel* candidat::afficherTriPrenomD() {
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM CANDIDATS ORDER BY PRENOM DESC");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Prenom"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Téléphone"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Poste"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Expérience"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Mail"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("Date de Naissance"));

    return model;
}

QSqlQueryModel* candidat::afficherTriDateNA() {
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM CANDIDATS ORDER BY DATEN ASC");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Prenom"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Téléphone"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Poste"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Expérience"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Mail"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("Date de Naissance"));

    return model;
}
QSqlQueryModel* candidat::afficherTriDateND() {
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM CANDIDATS ORDER BY DATEN DESC");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Prenom"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Téléphone"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Poste"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Expérience"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Mail"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("Date de Naissance"));

    return model;
}

bool candidat::archiver(const QString &id) {
    QSqlQuery query;
    query.prepare("UPDATE CANDIDATS SET estarchive = :est WHERE IDC = :id");
    query.bindValue(":est", 1);
    query.bindValue(":id", id); // Convert QString to int
    return query.exec();
}


bool candidat::desarchiver() {
    QSqlQuery query;
    query.prepare("UPDATE CANDIDATS SET estarchive = 0 WHERE idc = :id");
    query.bindValue(":id", id);  // Ensure it only unarchives the candidate with the matching ID
    return query.exec();
}


QSqlQueryModel* candidat::afficherCandidatsArchives() {
    QSqlQueryModel* model = new QSqlQueryModel();

    QSqlQuery query;
    if (!query.exec("SELECT idc, nom, prenom, telephone, poste, experience, mail, dateN FROM candidats WHERE estarchive = 1")) {
        qDebug() << "Query Error: " << query.lastError().text();
    }

    model->setQuery(query);

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Prénom"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Téléphone"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Poste"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Expérience"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Mail"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("Date de Naissance"));

    return model;
}
