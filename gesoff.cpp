#include "gesoff.h"
#include "ui_gesoff.h"
#include "options.h"

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
#include "mailling.h"
#include "notification.h"

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
    ui->lineEdit_nomP->setValidator(new QRegExpValidator(letterRegExp, this));
    ui->lineEdit_experience->setValidator(new QRegExpValidator(letterRegExp, this));

    connect(ui->pushButton_modifierO, &QPushButton::clicked, this, &GesOff::on_pushButton_modifierO_clicked);
     ComboBox_RnomP();
     marquerJoursExpiration(ui->calendarWidget);




}

GesOff::~GesOff()
{
    delete ui;
}

void GesOff::on_pushButton_ajoutrO_clicked()
{
    // Step 1: Check if any fields are empty
    if (ui->lineEdit_ID->text().isEmpty() ||
        ui->lineEdit_type->text().isEmpty() ||
        ui->lineEdit_nomE->text().isEmpty() ||
        ui->lineEdit_nomP->text().isEmpty() ||
        ui->lineEdit_posteV->text().isEmpty() ||
        ui->lineEdit_salaire->text().isEmpty() ||
        ui->lineEdit_experience->text().isEmpty())
    {
        // Show a warning message if any fields are empty
        QMessageBox::warning(this, "Champs vides", "Veuillez remplir tous les champs.");
        return; // Stop the process here if any field is empty
    }

    // Step 2: Check if the ID already exists in the database
    int id = ui->lineEdit_ID->text().toInt(); // Convert the ID to an integer

    QSqlQuery query;
    query.prepare("SELECT * FROM OFFRES WHERE ido = :id");
    query.bindValue(":id", id);
    query.exec();

    if (query.next()) {
        // Show a warning message if the ID already exists
        QMessageBox::warning(this, "ID Existe", "Cet ID existe déjà dans la base de données.");
        ui->lineEdit_ID->clear(); // Clear the ID field
        return; // Stop the process if the ID exists
    }

    // Step 3: Extract data from the input fields
    QString type = ui->lineEdit_type->text();
    QString nomE = ui->lineEdit_nomE->text();
    QString nomP = ui->lineEdit_nomP->text();
    int posteV = ui->lineEdit_posteV->text().toInt(); // Convert posteV to an integer
    float salaire = ui->lineEdit_salaire->text().toFloat(); // Convert salaire to a float
    QString experience = ui->lineEdit_experience->text();
    QDate dateE = ui->dateEdit->date(); // Get the selected date

    // Step 4: Create an instance of the 'offre' class and attempt to add it to the database
    offre of(id, type, nomE, nomP, posteV, salaire, experience, dateE);
    bool test = of.ajouter(); // Attempt to add the offer to the database

    // Step 5: Handle the result of the insertion
    if (test) {
        // Refresh the table view with the new data
        ui->tableView->setModel(of.afficher());


        // Show a success message
        QMessageBox::information(this, "Succès", "Offre ajoutée avec succès.");


        // Clear the input fields
        ui->lineEdit_ID->clear();
        ui->lineEdit_type->clear();
        ui->lineEdit_nomE->clear();
        ui->lineEdit_nomP->clear();
        ui->lineEdit_posteV->clear();
        ui->lineEdit_salaire->clear();
        ui->lineEdit_experience->clear();
        ui->dateEdit->setDate(QDate::currentDate());

        // Refresh the ComboBox with the new nomP
        ComboBox_RnomP(); // Assuming this method refreshes the ComboBox with the new nomP
    } else {
        // Show an error message if the insertion fails
        QMessageBox::warning(this, "Erreur", "Échec de l'ajout de l'offre à la base de données.");
    }
}







void GesOff::on_pushButton_supprimerO_clicked()
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
        offre o;
        QString idString = QString::number(selected);

          // Appeler la méthode 'supprimer' avec la QString convertie

        if (o.supprimer(idString))
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
void GesOff::on_tableView_doubleClicked(const QModelIndex &index)
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
    rechercherOffreParId(id);

    // Rendre le champ ID en lecture seule
    ui->lineEdit_ID->setReadOnly(true);
}




void GesOff::on_pushButton_modifierO_clicked() {



    // Récupérez les valeurs des autres champs
    QString type = ui->lineEdit_type->text();
    QString nomE = ui->lineEdit_nomE->text();
    QString nomP = ui->lineEdit_nomP->text();
    float posteV = ui->lineEdit_posteV->text().toFloat();
    float salaire = ui->lineEdit_salaire->text().toFloat();
    QString experience = ui->lineEdit_experience->text();
    QDate dateE = ui->dateEdit->date();

    // Obtenez la valeur de l'ID
    int id = ui->lineEdit_ID->text().toInt();

    // Créez un objet Offre avec les nouvelles données
    offre o(id, type, nomE, nomP, posteV, salaire, experience, dateE);

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



    // Nettoyez les champs après la modification
    ui->lineEdit_ID->clear();
    ui->lineEdit_type->clear();
    ui->lineEdit_nomE->clear();
    ui->lineEdit_nomP->clear();
    ui->lineEdit_posteV->clear();
    ui->lineEdit_salaire->clear();
    ui->lineEdit_experience->clear();
    ui->dateEdit->setDate(QDate::currentDate());
    ui->lineEdit_ID->setReadOnly(false);
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
            ui->lineEdit_nomP->setText(qry.value(3).toString());
            ui->lineEdit_posteV->setText(qry.value(4).toString());
            ui->lineEdit_salaire->setText(qry.value(5).toString());
            ui->lineEdit_experience->setText(qry.value(6).toString());
            ui->dateEdit->setWhatsThis(qry.value(7).toString());
        } else {
            QMessageBox::warning(this, "Erreur", "Aucune information trouvée pour ce code.");
            ui->lineEdit_ID->clear();
        }
    } else {
        qDebug() << "SQL Error:" << qry.lastError().text();  // Afficher l'erreur SQL
        QMessageBox::warning(this, "Erreur", "Erreur lors de la récupération des informations.");
    }
}







void GesOff::on_pushButton_rechercheO_clicked() {
    QString selectedNomP = ui->comboBox_RnomP->currentText();
    offre o;

    if (o.recherche(selectedNomP)) {
        QSqlQuery query;
        query.prepare("SELECT * FROM OFFRES WHERE nomP = :nomP");
        query.bindValue(":nomP", selectedNomP);

        if (query.exec()) {
            QStandardItemModel *model = new QStandardItemModel();
            model->setHorizontalHeaderLabels({"ID", "Type", "NomE", "NomP", "PosteV", "Salaire", "Experience", "Date"});

            while (query.next()) {
                QList<QStandardItem *> rowItems;
                for (int column = 0; column < query.record().count(); ++column) {
                    rowItems.append(new QStandardItem(query.value(column).toString()));
                }
                model->appendRow(rowItems);
            }

            ui->tableView->setModel(model);
        } else {
            QMessageBox::warning(this, "Erreur", "Erreur lors de l'exécution de la recherche.");
        }
    } else {
        QMessageBox::warning(this, "Avertissement", "Aucune offre trouvée pour le nomP sélectionné.");
    }
}



void GesOff::on_pushButton_triO_clicked()
{
    QString tri = ui->comboBox_triO->currentText();
    offre o;
    QSqlQueryModel* model = nullptr;

    if (tri == "IdA") { // trier par Id dans l'ordre croissant
        model = o.afficherTriId();
    } else if (tri == "IdD") { // trier par Id dans l'ordre décroissant
        model = o.afficherTriIdD();
    } else if (tri == "salaireH") { // trier par Salaire dans l'ordre décroissant
        model = o.afficherTriSalaireH();
    } else if (tri == "salaireB") { // trier par Salaire dans l'ordre croissant
        model = o.afficherTriSalaireB();
    } else if (tri == "type A..Z") { // trier par Type dans l'ordre croissant
        model = o.afficherTriType();
    } else if (tri == "type Z..A") { // trier par Type dans l'ordre décroissant
        model = o.afficherTriTypeD();
    } else if (tri == "nomE A..Z") { // trier par NomE dans l'ordre croissant
        model = o.afficherTriNomE();
    } else if (tri == "nomE Z..A") { // trier par NomE dans l'ordre décroissant
        model = o.afficherTriNomED();
    } else if (tri == "postesVA") { // trier par PostesV dans l'ordre croissant
        model = o.afficherTriPostesV();
    } else if (tri == "postesVD") { // trier par PostesV dans l'ordre décroissant
        model = o.afficherTriPostesVD();
    } else if (tri == "nomP A..Z") { // trier par DateE dans l'ordre croissant
        model = o.afficherTriNomP();
    } else if (tri == "nomP Z..A") { // trier par DateE dans l'ordre décroissant
        model = o.afficherTriNomPD();
    }

    if (model) {
        ui->tableView->setModel(model);
    }
}


void GesOff::on_pushButton_PDFF_clicked()
{
    QSqlQuery query;
       query.prepare("SELECT nomP, type, nomE, salaire, experience, dateE FROM OFFRES");
       if (!query.exec()) {
           QMessageBox::warning(this, "Database Error", query.lastError().text());
           return;
       }

       QString html = "<html><head><style>"
                      "body { font-family: Arial, sans-serif; }"
                      "h1 { color: #007bff; }"
                      ".header { background-color: #f8f9fa; padding: 10px; text-align: center; border: 1px solid #ddd; }"
                      ".job-title { font-size: 24px; font-weight: bold; }"
                      ".company-info { font-size: 18px; margin-top: 10px; }"
                      ".details { border: 1px solid #ddd; margin-top: 20px; padding: 10px; }"
                      "table { width: 100%; border-collapse: collapse; margin-top: 20px; }"
                      "th, td { border: 1px solid #ddd; padding: 8px; text-align: left; }"
                      "th { background-color: #f2f2f2; }"
                      ".footer { margin-top: 30px; text-align: center; font-size: 12px; color: #888; }"
                      "</style></head><body>";

       html += "<div class='header'><h1>Offres d'Emploi</h1></div>";

       while (query.next()) {
           QString nomP = query.value(0).toString();
           QString type = query.value(1).toString();
           QString nomE = query.value(2).toString();
           float salaire = query.value(3).toFloat();
           QString experience = query.value(4).toString();
           QDate dateE = query.value(5).toDate();

           html += "<div class='details'>";
           html += "<div class='job-title'>" + nomP + "</div>";
           html += "<div class='company-info'>Entreprise: " + nomE + "</div>";
           html += "<p><strong>Type d'emploi:</strong> " + type + "<br>";
           html += "<strong>Salaire:</strong> " + QString::number(salaire) + " DT<br>";
           html += "<strong>Expérience demandé:</strong> " + experience + "<br>";
           html += "<strong>Date d'embauche:</strong> " + dateE.toString("dd/MM/yyyy") + "</p>";
           html += "</div>";
       }

       html += "<div class='footer'>Généré par l'application de gestion des offres d'emploi</div>";
       html += "</body></html>";

       QString pdfFilePath = "Styled_Offres_Emploi.pdf";
       QPrinter printer(QPrinter::PrinterResolution);
       printer.setOutputFormat(QPrinter::PdfFormat);
       printer.setOutputFileName(pdfFilePath);

       QTextDocument doc;
       doc.setHtml(html);
       doc.print(&printer);

       QMessageBox::information(this, "PDF Created", "Le fichier PDF a été créé avec succès.");

       // Open the PDF file after creating it
       QDesktopServices::openUrl(QUrl::fromLocalFile(pdfFilePath));

}

void GesOff::on_pushButton_stats_clicked()
{
    QSqlQuery qTotal, qDistinct, qCount;
    qreal tot = 0;

    QString field;  // This will hold either "NOME" or "NOMP" based on the checkbox selection

    // Determine which field to use based on the checkbox state
    if (ui->checkBox_nomE->isChecked()) {
        field = "NOME";
    } else if (ui->checkBox_nomP->isChecked()) {
        field = "NOMP";
    } else {
        // Handle the case where neither checkbox is selected
        QMessageBox::warning(this, "Erreur", "Veuillez sélectionner un critère (NomE ou NomP).");
        return;
    }

    // Query to get the total number of offers
    qTotal.prepare("SELECT COUNT(*) FROM OFFRES");
    qTotal.exec();
    if (qTotal.next()) {
        tot = qTotal.value(0).toDouble();
    }

    // Query to get distinct values for the selected field (either NOME or NOMP)
    qDistinct.prepare("SELECT DISTINCT " + field + " FROM OFFRES");
    qDistinct.exec();

    // Create a pie series
    QPieSeries *series = new QPieSeries();

    // Iterate over each distinct value and calculate its percentage
    while (qDistinct.next()) {
        QString value = qDistinct.value(0).toString();

        // Count how many records have this distinct value
        qCount.prepare("SELECT COUNT(*) FROM OFFRES WHERE " + field + " = :value");
        qCount.bindValue(":value", value);
        qCount.exec();

        if (qCount.next()) {
            qreal count = qCount.value(0).toDouble();
            qreal percentage = (count * 100) / tot;

            // Append the result to the pie chart series
            series->append(value + " = " + QString::number(percentage, 'f', 2) + "%", percentage);
        }
    }

    // Create the chart
    QChart *chart = new QChart();
    QString chartTitle = "Répartition des offres par " + (field == "NOME" ? QString("entreprise (nomE)") : QString("poste (nomP)")) + ":";
    chart->setTitle(chartTitle);
    chart->addSeries(series);
    chart->legend()->show();
    chart->setBackgroundBrush(QColor(252, 236, 218, 0));

    // Display the chart
    QChartView *chartView = new QChartView(chart, ui->graphicsView);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setMinimumSize(400, 400);
    chartView->show();
}

void GesOff::ComboBox_RnomP()
{
    QSqlQuery query;

    // Query to get distinct nomP values from the offres table
    query.prepare("SELECT DISTINCT nomP FROM offres");

    if (query.exec()) {
        // Clear the combo box before populating it
        ui->comboBox_RnomP->clear();

        // Loop through the query results and add each nomP to the combo box
        while (query.next()) {
            QString nomP = query.value(0).toString();  // Get the first column (nomP)
            ui->comboBox_RnomP->addItem(nomP);
        }
    } else {
        // Handle any errors that occurred during the query execution
        QMessageBox::critical(this, "Erreur", "Impossible de récupérer les données: " + query.lastError().text());
    }
}

void GesOff::on_pushButton_refresh_clicked()
{
    offre of; // Assuming you have a class `offre` that handles database operations
      ui->tableView->setModel(of.afficher());
}



// Slot to handle delete button click
void GesOff::onDeleteButtonClicked(const QModelIndex &index)
{
    int row = index.row();
    QString id = ui->tableView->model()->index(row, 0).data().toString(); // Assuming the ID is in the first column

    // Delete the offer from the database
    offre o;
    if (o.supprimer(id)) {
        // Refresh the table view after deletion
        ui->tableView->setModel(o.afficher());
        QMessageBox::information(this, "Succès", "Offre supprimée avec succès.");
    } else {
        QMessageBox::warning(this, "Erreur", "Échec de la suppression de l'offre.");
    }
}



// Assuming the Mailling window class is named 'Mailling'
void GesOff::on_pushButton_Mailling_clicked()
{
    // Create an instance of the Mailling window
    Mailling *mailWindow = new Mailling(this);

    // Show the Mailling window
    mailWindow->show();
}

void GesOff::show_tables(){
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

void GesOff::on_tableView_clicked(const QModelIndex &index)
{
    // Obtenir la valeur du premier champ (ID) de la ligne sélectionnée.
    selected = ui->tableView->model()->data(ui->tableView->model()->index(index.row(), 0)).toInt();
}


void GesOff::marquerJoursExpiration(QCalendarWidget *calendrier) {
    // Fetch expiration dates, job titles, and company names from the database
    QSqlQuery query;
    QMap<QDate, QPair<QString, QString>> joursExpiration; // Use QPair to store both nomP and nomE

    // Query the database to get expiration dates and corresponding job titles and company names
    if (query.exec("SELECT dateE, nomP, nomE FROM OFFRES")) {
        while (query.next()) {
            QDate dateE = query.value(0).toDate();
            QString nomP = query.value(1).toString();
            QString nomE = query.value(2).toString();
            joursExpiration.insert(dateE, qMakePair(nomP, nomE));  // Store dateE with nomP and nomE in the map
            qDebug() << "DateE fetched:" << dateE.toString() << "NomP:" << nomP << "NomE:" << nomE; // Debug: Log the date, job title, and company name
        }
    } else {
        qDebug() << "Database query failed: " << query.lastError().text();
        return;
    }

    // Apply the style (red background) to the expiration dates in the calendar
    QTextCharFormat format;
    format.setBackground(QBrush(QColor("red")));  // Set the background color to red

    // Debug: Ensure the calendar widget is valid
    if (!calendrier) {
        qDebug() << "Calendar widget is null";
        return;
    }

    // Highlight dates
    for (auto it = joursExpiration.begin(); it != joursExpiration.end(); ++it) {
        QDate jourExpiration = it.key();
        qDebug() << "Highlighting Date:" << jourExpiration.toString(); // Debug: Log the highlighted date
        calendrier->setDateTextFormat(jourExpiration, format);
    }

    // Force the calendar to update and repaint
    calendrier->update();

    // Ensure the calendar is set to single selection mode
    calendrier->setSelectionMode(QCalendarWidget::SingleSelection);

    // Connect the selectionChanged signal to display the job title and company name in a QMessageBox when an expiration date is clicked
    connect(calendrier, &QCalendarWidget::selectionChanged, [=]() {
        QDate selectedDate = calendrier->selectedDate();
        if (joursExpiration.contains(selectedDate)) {
            QString nomP = joursExpiration.value(selectedDate).first;  // Get job title
            QString nomE = joursExpiration.value(selectedDate).second; // Get company name
            QMessageBox::information(this, "Job Information", "Job Title: " + nomP + "\nCompany Name: " + nomE);
            qDebug() << "Selected Date:" << selectedDate.toString() << "NomP:" << nomP << "NomE:" << nomE; // Debug: Log the selected date, job title, and company name
        } else {
            QMessageBox::information(this, "Job Information", "No expiration date selected.");
            qDebug() << "Selected Date (no expiration):" << selectedDate.toString(); // Debug: Log non-expiration date selections
        }
    });
}
void GesOff::on_pb_back1_clicked()
 {
     options *E = new options;
     E ->show();
     hide();

 }
