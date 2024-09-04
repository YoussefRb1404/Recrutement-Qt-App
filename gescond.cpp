#include "gescond.h"
#include "ui_gescond.h"
#include "candidat.h"



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

GesCond::GesCond(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::GesCond)
{
    ui->setupUi(this);
    ui->tableView->setModel(cad.afficher());
    ui->table->setModel(cad.afficherCandidatsArchives());




    QRegExp numberRegExp("[0-9]+");
    QRegExp letterRegExp("[a-zA-Z]+");
    QRegExp emailRegExp("[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,4}");



    ui->lineEdit_ID->setValidator(new QRegExpValidator(numberRegExp, this));
    ui->lineEdit_telephone->setValidator(new QRegExpValidator(numberRegExp, this));
    ui->lineEdit_experience->setValidator(new QRegExpValidator(numberRegExp, this));

    ui->lineEdit_nom->setValidator(new QRegExpValidator(letterRegExp, this));
    ui->lineEdit_prenom->setValidator(new QRegExpValidator(letterRegExp, this));
    ui->lineEdit_poste->setValidator(new QRegExpValidator(letterRegExp, this));

    ui->lineEdit_mail->setValidator(new QRegExpValidator(emailRegExp, this));


}

GesCond::~GesCond()
{
    delete ui;

}


void GesCond::on_pushButton_ajoutrC_clicked()
{
    // Step 1: Check if any fields are empty
    if (ui->lineEdit_ID->text().isEmpty() ||
        ui->lineEdit_nom->text().isEmpty() ||
        ui->lineEdit_prenom->text().isEmpty() ||
        ui->lineEdit_telephone->text().isEmpty() ||
        ui->lineEdit_poste->text().isEmpty() ||
        ui->lineEdit_experience->text().isEmpty() ||
        ui->lineEdit_mail->text().isEmpty())
    {
        // Show a warning message if any fields are empty
        QMessageBox::warning(this, "Champs vides", "Veuillez remplir tous les champs.");
        return; // Stop the process here if any field is empty
    }

    // Step 2: Check if the ID already exists in the database
    int id = ui->lineEdit_ID->text().toInt(); // Convert the ID to an integer

    QSqlQuery query;
    query.prepare("SELECT * FROM CANDIDATS WHERE idc = :id");
    query.bindValue(":id", id);
    if (!query.exec()) {
        // Handle query execution failure (e.g., database connection issues)
        QMessageBox::warning(this, "Erreur", "Erreur lors de la vérification de l'ID.");
        return;
    }

    if (query.next()) {
        // Show a warning message if the ID already exists
        QMessageBox::warning(this, "ID Existe", "Cet ID existe déjà dans la base de données.");
        ui->lineEdit_ID->clear(); // Clear the ID field
        return; // Stop the process if the ID exists
    }

    // Step 3: Extract data from the input fields
    QString nom = ui->lineEdit_nom->text();
    QString prenom = ui->lineEdit_prenom->text();
    int telephone = ui->lineEdit_telephone->text().toInt();
    QString poste = ui->lineEdit_poste->text();
    int experience = ui->lineEdit_experience->text().toInt(); // Convert experience to an integer
    QString mail = ui->lineEdit_mail->text();
    QDate dateN = ui->dateEdit->date(); // Get the selected date

    // Step 4: Create an instance of the 'candidat' class and attempt to add it to the database
    candidat cad(id, nom, prenom, telephone, poste, experience, mail, dateN);
    bool test = cad.ajouter(); // Attempt to add the candidate to the database

    // Step 5: Handle the result of the insertion
    if (test) {
        // Refresh the table view with the new data
        ui->tableView->setModel(cad.afficher());

        // Show a success message
        QMessageBox::information(this, "Succès", "Candidat ajouté avec succès.");

        // Clear the input fields
        ui->lineEdit_ID->clear();
        ui->lineEdit_nom->clear();
        ui->lineEdit_prenom->clear();
        ui->lineEdit_telephone->clear();
        ui->lineEdit_poste->clear();
        ui->lineEdit_experience->clear();
        ui->lineEdit_mail->clear();
        ui->dateEdit->setDate(QDate::currentDate());

        // Refresh the ComboBox if applicable
        // Assuming this method refreshes the ComboBox with relevant data
    } else {
        // Show an error message if the insertion fails
        QMessageBox::warning(this, "Erreur", "Échec de l'ajout du candidat à la base de données.");
    }
}

void GesCond::on_tableView_clicked(const QModelIndex &index)
{
    selected = ui->tableView->model()->data(ui->tableView->model()->index(index.row(), 0)).toInt();
}

void GesCond::on_pushButton_supprimerC_clicked()
{
    if (selected == 0)
    {
        QMessageBox::warning(this, "Suppression", "Aucune offre sélectionnée pour suppression.");
        return;
    }

    // Confirmation avant la suppression
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Suppression", "Êtes-vous sûr de vouloir supprimer cette offre?",
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes)
    {
        candidat can;
        QString idString = QString::number(selected);

          // Appeler la méthode 'supprimer' avec la QString convertie

        if (can.supprimer(idString))
        {
            show_tables();
            QMessageBox::information(this, "Suppression", "L'offre a été supprimée avec succès.");
        }
        else
        {
            QMessageBox::warning(this, "Erreur", "La suppression a échoué.");
        }

        // Refresh du tableau pour afficher les données mises à jour

    }

    // Réinitialisation du champ sélectionné
    selected = 0;
}

void GesCond::show_tables(){
//creation model (masque du tableau) : permet recherche et tri
    proxy = new QSortFilterProxyModel();

 //definir la source (tableau original)
    proxy->setSourceModel(tmp.afficher());

 //pour la recherche
    proxy->setFilterCaseSensitivity(Qt::CaseInsensitive); // S=s (pas de difference entre majiscule et miniscule)
    proxy->setFilterKeyColumn(-1); // chercher dans tout le tableau (-1) ou donner le numero de la colone
   //remplissage tableau avec le masque
    ui->tableView->setModel(proxy);

}

void GesCond::show_tablesA() {
    // Create a model to display archived candidates
    QSqlQueryModel *archivedModel = new QSqlQueryModel();
    archivedModel->setQuery("SELECT * FROM candidats WHERE est_archive = 1");

    // Set headers for better readability
    archivedModel->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    archivedModel->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom"));
    archivedModel->setHeaderData(2, Qt::Horizontal, QObject::tr("Prenom"));
    archivedModel->setHeaderData(3, Qt::Horizontal, QObject::tr("Téléphone"));
    archivedModel->setHeaderData(4, Qt::Horizontal, QObject::tr("Poste"));
    archivedModel->setHeaderData(5, Qt::Horizontal, QObject::tr("Expérience"));
    archivedModel->setHeaderData(6, Qt::Horizontal, QObject::tr("Mail"));
    archivedModel->setHeaderData(7, Qt::Horizontal, QObject::tr("Date de Naissance"));

    // Set the model to the table view for archived candidates
    ui->table->setModel(archivedModel);
}



void GesCond::on_tableView_doubleClicked(const QModelIndex &index)
{
    // Récupérer l'ID du candidat à partir de la cellule double-cliquée
    QSqlQueryModel* model = qobject_cast<QSqlQueryModel*>(ui->tableView->model());
    if (!model) {
        qWarning() << "Le modèle de la vue n'est pas un QSqlQueryModel.";
        return;
    }

    // On suppose que l'ID est dans la première colonne (index 0)
    int id = model->data(model->index(index.row(), 0)).toInt();

    // Appeler la méthode pour rechercher le candidat par ID
    rechercherCandidatParId(id);

    // Rendre le champ ID en lecture seule
    ui->lineEdit_ID->setReadOnly(true);
}


void GesCond::rechercherCandidatParId(int id) {
    qDebug() << "Querying candidate with ID:" << id;  // Débogage pour vérifier l'ID utilisé dans la requête

    QSqlQuery qry;
    qry.prepare("SELECT * FROM CANDIDATS WHERE idc = :idc");
    qry.bindValue(":idc", id);

    if (qry.exec()) {
        if (qry.next()) {
            // Remplir les champs avec les données récupérées
            ui->lineEdit_ID->setText(qry.value(0).toString());
            ui->lineEdit_nom->setText(qry.value(1).toString());
            ui->lineEdit_prenom->setText(qry.value(2).toString());
            ui->lineEdit_telephone->setText(qry.value(3).toString());
            ui->lineEdit_poste->setText(qry.value(4).toString());
            ui->lineEdit_experience->setText(qry.value(5).toString());
            ui->lineEdit_mail->setText(qry.value(6).toString());
            ui->dateEdit->setDate(qry.value(7).toDate());
        } else {
            QMessageBox::warning(this, "Erreur", "Aucune information trouvée pour cet ID.");

        }
    } else {
        qDebug() << "SQL Error:" << qry.lastError().text();  // Afficher l'erreur SQL
        QMessageBox::warning(this, "Erreur", "Erreur lors de la récupération des informations.");
    }
}

void GesCond::rechercherCandidatParDate(const QDate &dateN) {
    QSqlQuery query;
    QString dateString = dateN.toString("dd-MM-yyyy");  // Format de date DD-MM-YYYY
    query.prepare("SELECT * FROM CANDIDATS WHERE daten = TO_DATE(:date, 'DD-MM-YYYY')");
    query.bindValue(":date", dateString);

    if (!query.exec()) {
        qDebug() << "Erreur SQL:" << query.lastError().text();
        QMessageBox::warning(this, "Erreur", "Erreur lors de la récupération des informations.");
    } else {
        // Créer un modèle pour afficher les résultats
        QStandardItemModel *model = new QStandardItemModel();
        model->setHorizontalHeaderLabels({"ID", "Nom", "Prénom", "Téléphone", "Poste", "Expérience", "Mail", "Date de Naissance"});

        // Remplir le modèle avec les résultats de la requête
        while (query.next()) {
            QList<QStandardItem *> rowItems;
            for (int column = 0; column < query.record().count(); ++column) {
                rowItems.append(new QStandardItem(query.value(column).toString()));
            }
            model->appendRow(rowItems);
        }

        // Afficher le modèle dans le QTableView
        ui->tableView->setModel(model);
    }
}



void GesCond::on_pushButton_modifierC_clicked()
{

        // Récupérez les valeurs des champs du formulaire
        QString nom = ui->lineEdit_nom->text();
        QString prenom = ui->lineEdit_prenom->text();
        int telephone = ui->lineEdit_telephone->text().toInt();
        QString poste = ui->lineEdit_poste->text();
        int experience = ui->lineEdit_experience->text().toInt();
        QString mail = ui->lineEdit_mail->text();
        QDate dateN = ui->dateEdit->date();

        // Obtenez la valeur de l'ID
        int id = ui->lineEdit_ID->text().toInt();

        // Créez un objet Candidat avec les nouvelles données
        candidat cad(id, nom, prenom, telephone, poste, experience, mail, dateN);

        // Appelez la méthode de modification
        bool test = cad.modifier();

        if (test) {
            // Actualisez le modèle affiché dans la vue
            ui->tableView->setModel(cad.afficher());
            // Affichez un message de succès
            QMessageBox::information(this, "Succès", "Candidat mis à jour avec succès.");
        } else {
            // Affichez un message d'erreur
            QMessageBox::warning(this, "Erreur", "Échec de la mise à jour du candidat dans la base de données.");
        }




        // Nettoyez les champs après la modification
        ui->lineEdit_ID->clear();
        ui->lineEdit_nom->clear();
        ui->lineEdit_prenom->clear();
        ui->lineEdit_telephone->clear();
        ui->lineEdit_poste->clear();
        ui->lineEdit_experience->clear();
        ui->lineEdit_mail->clear();
        ui->dateEdit->setDate(QDate::currentDate());
        ui->lineEdit_ID->setReadOnly(false);


}

void GesCond::on_pushButton_rechercheC_clicked()
 {

        QDate selectedDate = ui->dateEdit->date();  // Récupérez la date sélectionnée

        // Appel à la fonction de recherche
        rechercherCandidatParDate(selectedDate);
    }




void GesCond::on_pushButton_triC_clicked()
{
    QString tri = ui->comboBox_triC->currentText();
    candidat can;
    QSqlQueryModel* model = nullptr;
    if (tri == "NOM A..Z") { // trier par Id dans l'ordre croissant
        model = can.afficherTriNomA();
    } else if (tri == "NOM Z..A") { // trier par Id dans l'ordre décroissant
        model = can.afficherTriNomD();
    } else if (tri == "PRENOM A..Z") { // trier par Salaire dans l'ordre décroissant
        model = can.afficherTriPrenomA();
    } else if (tri == "PRENOM Z..A") { // trier par Salaire dans l'ordre croissant
        model = can.afficherTriPrenomD();
    } else if (tri == "DATEN A") { // trier par Type dans l'ordre croissant
        model = can.afficherTriDateNA();
    } else if (tri == "DATEN D") { // trier par Type dans l'ordre décroissant
        model = can.afficherTriDateND();
}
    if (model) {
        ui->tableView->setModel(model);
    }
}

void GesCond::on_pushButton_stats_clicked()
{
    QSqlQuery qTotal, qCount;
    qreal tot = 0;

    // Champ pour la statistique
    QString field = "poste";  // Basé sur le poste des candidats
    QString fieldLabel = "Poste";  // Label pour le titre du graphique

    // Requête pour obtenir le nombre total de candidats
    qTotal.prepare("SELECT COUNT(*) FROM CANDIDATS");
    if (qTotal.exec() && qTotal.next()) {
        tot = qTotal.value(0).toDouble();
    } else {
        QMessageBox::warning(this, "Erreur", "Erreur lors de la récupération du total des candidats.");
        return;
    }

    // Requête pour obtenir les valeurs distinctes pour le champ sélectionné
    QSqlQuery qDistinct;
    qDistinct.prepare("SELECT DISTINCT " + field + " FROM CANDIDATS");
    if (!qDistinct.exec()) {
        QMessageBox::warning(this, "Erreur", "Erreur lors de la récupération des valeurs distinctes.");
        return;
    }

    // Créer une série de camembert
    QPieSeries *series = new QPieSeries();

    // Itérer sur chaque valeur distincte et calculer son pourcentage
    while (qDistinct.next()) {
        QString value = qDistinct.value(0).toString();

        // Compter combien de fois chaque valeur distincte apparaît
        qCount.prepare("SELECT COUNT(*) FROM CANDIDATS WHERE " + field + " = :value");
        qCount.bindValue(":value", value);
        if (qCount.exec() && qCount.next()) {
            qreal count = qCount.value(0).toDouble();
            qreal percentage = (count * 100) / tot;

            // Ajouter le résultat à la série de camembert
            series->append(value + " = " + QString::number(percentage, 'f', 2) + "%", percentage);
        }
    }

    // Créer le graphique
    QChart *chart = new QChart();
    chart->setTitle("Répartition des candidats par " + fieldLabel + ":");
    chart->addSeries(series);
    chart->legend()->show();
    chart->setBackgroundBrush(QColor(252, 236, 218, 0));

    // Afficher le graphique
    QChartView *chartView = new QChartView(chart, ui->graphicsView);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setMinimumSize(400, 400);
    chartView->show();
}


void GesCond::on_pushButton_refresh_clicked()
{
    candidat can; // Assuming you have a class `offre` that handles database operations
      ui->tableView->setModel(can.afficher());
}

void GesCond::on_pushButton_excel_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Excel File"), qApp->applicationDirPath(),
                                                    tr("Excel Files (*.xls)"));
    if (fileName.isEmpty())
        return;

    ExportExcelObject obj(fileName, "myData", ui->tableView);

    // Define columns to export
    obj.addField(0, "ID", "char(20)");
    obj.addField(1, "Nom", "char(20)");
    obj.addField(2, "Prénom", "char(20)");
    obj.addField(3, "Téléphone", "char(20)");
    obj.addField(4, "Poste", "char(20)");
    obj.addField(5, "Expérience", "char(20)");
    obj.addField(6, "Mail", "char(20)");
    obj.addField(7, "Date de Naissance", "char(20)");

    int retVal = obj.export2Excel();
    if (retVal > 0) {
        QMessageBox::information(this, tr("Success"),
                                 QString(tr("%1 records exported successfully!")).arg(retVal));
    } else {
        QMessageBox::warning(this, tr("Error"),
                             tr("."));
    }
}


void GesCond::on_pushButton_ArchiverC_clicked() {
    if (selected == 0) {
        QMessageBox::warning(this, "Archivage", "Aucun candidat sélectionné pour archivage.");
        return;
    }

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Archivage", "Êtes-vous sûr de vouloir archiver ce candidat?",
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        QString id = QString::number(selected); // Convert int to QString

        candidat can;
        if (can.archiver(id)) { // Pass the id directly to archiver
             // Refresh to show updated candidate status
            QMessageBox::information(this, "Archivage", "Le candidat a été archivé avec succès.");
            ui->tableView->setModel(can.afficher());
            ui->table->setModel(cad.afficherCandidatsArchives());
        } else {
            QMessageBox::warning(this, "Erreur", "L'archivage a échoué.");
        }

        selected = 0; // Reset selected candidate ID
    }
}







void GesCond::on_table_clicked(const QModelIndex &index)
{
   // Store the selected candidate's ID as an integer
   selected = ui->table->model()->data(ui->table->model()->index(index.row(), 0)).toInt();
}

void GesCond::on_pushButtonDesarchiver_clicked()
{
    if (selected == 0)
    {
        QMessageBox::warning(this, "Désarchivage", "Aucun candidat sélectionné pour désarchivage.");
        return;
    }

    // Confirm the unarchiving action
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Désarchivage",
                                  QString("Vous voulez désarchiver le candidat dont l'ID est : %1 ?").arg(selected),
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes)
    {
        candidat can;
        can.setId(selected); // Set the candidate ID directly

        // Attempt to unarchive the candidate using the selected ID
        if (can.desarchiver())
        {
            // Update the views to reflect the changes
            ui->tableView->setModel(can.afficher());
            ui->table->setModel(can.afficherCandidatsArchives());
            QMessageBox::information(this, "Succès",
                                     "Désarchivé avec succès.\nCliquez sur Annuler pour quitter.",
                                     QMessageBox::Cancel);
        }
        else
        {
            // Show an error message if the unarchiving fails
            QMessageBox::warning(this, "Erreur",
                                 "Échec du désarchivage !\nCliquez sur Annuler pour quitter.",
                                 QMessageBox::Cancel);
        }

        // Reset the selected ID after the operation
        selected = 0;
    }
}

void GesCond::on_pushButton_camera_clicked()
{
    c= new camera();
    c->show();
}
