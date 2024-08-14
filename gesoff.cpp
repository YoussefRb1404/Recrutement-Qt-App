#include "gesoff.h"
#include "ui_gesoff.h"

#include <QtWidgets>
#include <QTableWidget>
#include <QRegExp>
#include <QDebug>
#include <QtSql>
#include <QSqlQuery>
#include <QPieSeries>

#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QLegend>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QHorizontalStackedBarSeries>
#include <QtCharts/QLineSeries>
#include <QtCharts/QCategoryAxis>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QDate>
#include <QtDebug>
#include <QPrinter>
#include <QPainter>
#include <QFileDialog>
#include <QChart>
#include <QPieSlice>
#include <QChartView>

QT_CHARTS_USE_NAMESPACE

GesOff::GesOff(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::GesOff)
{
    ui->setupUi(this);
    ui->tableView->setModel(of.afficher());

    // Input validation
    QRegExp numberRegExp("[0-9]+");
    QRegExp letterRegExp("[a-zA-Z]+");

    ui->lineEdit_ID->setValidator(new QRegExpValidator(numberRegExp, this));
    ui->lineEdit_posteV->setValidator(new QRegExpValidator(numberRegExp, this));
    ui->lineEdit_salaire->setValidator(new QRegExpValidator(numberRegExp, this));

    ui->lineEdit_type->setValidator(new QRegExpValidator(letterRegExp, this));
    ui->lineEdit_nomE->setValidator(new QRegExpValidator(letterRegExp, this));
    ui->lineEdit_experience->setValidator(new QRegExpValidator(letterRegExp, this));

    connect(ui->pushButton_modifierO, &QPushButton::clicked, this, &GesOff::on_pushButton_modifierO_clicked);
    qDebug() << "3asba" << ui->pushButton_modifierO->text();


}

GesOff::~GesOff()
{
    delete ui;
}

void GesOff::on_pushButton_ajoutrO_clicked()
{
    // Check if any fields are empty
    if (ui->lineEdit_ID->text().isEmpty() ||
        ui->lineEdit_type->text().isEmpty() ||
        ui->lineEdit_nomE->text().isEmpty() ||
        ui->lineEdit_posteV->text().isEmpty() ||
        ui->lineEdit_salaire->text().isEmpty() ||
        ui->lineEdit_experience->text().isEmpty())
    {
        QMessageBox::warning(this, "Champs vides", "Veuillez remplir tous les champs.");
        return;
    }

    int id = ui->lineEdit_ID->text().toInt();

    // Check if ID already exists
    QSqlQuery query;
    query.prepare("SELECT * FROM OFFRES WHERE ido = :id");
    query.bindValue(":id", id);
    query.exec();

    if (query.next()) {
        QMessageBox::warning(this, "ID Existe", "Cet ID existe déjà dans la base de données.");
        ui->lineEdit_ID->clear();
        return;
    }

    QString type = ui->lineEdit_type->text();
    QString nomE = ui->lineEdit_nomE->text();
    float posteV = ui->lineEdit_posteV->text().toFloat();
    float salaire = ui->lineEdit_salaire->text().toFloat();
    QString experience = ui->lineEdit_experience->text();
    QDate dateE = ui->dateEdit->date();

    offre of(id, type, nomE, posteV, salaire, experience, dateE);
    bool test = of.ajouter();

    if (test) {
        ui->tableView->setModel(of.afficher());
        QMessageBox::information(this, "Succès", "Offre ajoutée avec succès.");

        ui->lineEdit_ID->clear();
        ui->lineEdit_type->clear();
        ui->lineEdit_nomE->clear();
        ui->lineEdit_posteV->clear();
        ui->lineEdit_salaire->clear();
        ui->lineEdit_experience->clear();
        ui->dateEdit->setDate(QDate::currentDate());
    } else {
        QMessageBox::warning(this, "Erreur", "Échec de l'ajout de l'offre à la base de données.");
    }
}


void GesOff::on_pushButton_supprimerO_clicked()
{
    QString id = ui->lineEdit_ID->text();

    // Check if ID exists before deletion
    QSqlQuery query;
    query.prepare("SELECT * FROM OFFRES WHERE ido = :id");
    query.bindValue(":id", id);
    query.exec();

    if (!query.next()) {
        QMessageBox::warning(this, "ID Non Trouvé", "Cet ID n'existe pas dans la base de données.");
        ui->lineEdit_ID->clear();
        return;
    }

    if (of.supprimer(id)) {
        ui->tableView->setModel(of.afficher());
        QMessageBox::information(this, "Succès", "Offre supprimée avec succès.");
        ui->lineEdit_ID->clear();
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de la suppression de l'offre.");
    }
}




void GesOff::on_pushButton_modifierO_clicked() {
    int id= ui->lineEdit_ID->text().toInt();
       rechercherOffreParId(id);
        ui->lineEdit_ID->setReadOnly(true);
}

void GesOff::rechercherOffreParId(int id) {
    qDebug() << "Querying offer with ID:" << id;  // Débogage pour vérifier l'ID utilisé dans la requête

    QSqlQuery qry;
    qry.prepare("SELECT * FROM OFFRES WHERE ido = :ido");
    qry.bindValue(":ido", id);

    if (qry.exec()) {
        if (qry.next()) {
            // Remplir les champs avec les données récupérées
            ui->lineEdit_ID->setText(qry.value(0).toString());
            ui->lineEdit_type->setText(qry.value(1).toString());
            ui->lineEdit_nomE->setText(qry.value(2).toString());
            ui->lineEdit_posteV->setText(qry.value(3).toString());
            ui->lineEdit_salaire->setText(qry.value(4).toString());
            ui->lineEdit_experience->setText(qry.value(5).toString());
            ui->dateEdit->setWhatsThis(qry.value(6).toString());
        } else {
            QMessageBox::warning(this, "Erreur", "Aucune information trouvée pour ce code.");
            ui->lineEdit_ID->clear();
        }
    } else {
        qDebug() << "SQL Error:" << qry.lastError().text();  // Afficher l'erreur SQL
        QMessageBox::warning(this, "Erreur", "Erreur lors de la récupération des informations.");
    }
}





void GesOff::on_pushButton_confirmer_clicked()
{



     // Récupérez les valeurs des autres champs
     QString type = ui->lineEdit_type->text();
     QString nomE = ui->lineEdit_nomE->text();
     float posteV = ui->lineEdit_posteV->text().toFloat();
     float salaire = ui->lineEdit_salaire->text().toFloat();
     QString experience = ui->lineEdit_experience->text();
     QDate dateE = ui->dateEdit->date();

     // Obtenez la valeur de l'ID
     int id = ui->lineEdit_ID->text().toInt();

     // Créez un objet Offre avec les nouvelles données
     offre o(id, type, nomE, posteV, salaire, experience, dateE);

     // Appelez la méthode de modification
     bool test = o.modifier();

     if (test) {
         // Actualisez le modèle affiché dans la vue
         ui->tableView->setModel(of.afficher());
         // Affichez un message de succès
         QMessageBox::information(this, "Succès", "Offre mise à jour avec succès.");
     } else {
         // Affichez un message d'erreur
         QMessageBox::warning(this, "Erreur", "Échec de la mise à jour de l'offre dans la base de données.");
     }

     // Réinitialisez le bouton à son état initial
     ui->pushButton_modifierO->setText("Modifier");

     // Nettoyez les champs après la modification
     ui->lineEdit_ID->clear();
     ui->lineEdit_type->clear();
     ui->lineEdit_nomE->clear();
     ui->lineEdit_posteV->clear();
     ui->lineEdit_salaire->clear();
     ui->lineEdit_experience->clear();
     ui->dateEdit->setDate(QDate::currentDate());
     ui->lineEdit_ID->setReadOnly(false);
}
