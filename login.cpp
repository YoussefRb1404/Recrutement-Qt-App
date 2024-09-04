#include "login.h"
#include "ui_login.h"
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
#include "gesoff.h"
#include "options.h"


login::login(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::login)
{
    ui->setupUi(this);
}

login::~login()
{
    delete ui;
}



void login::on_pushButton_login_clicked()
{
    // Retrieve ID and password from the input fields
    int id = ui->lineEdit_ID->text().toInt();
    int mdp = ui->lineEdit_mdp->text().toInt();

    // Debug output to check inputs (optional)
    qDebug() << "ID:" << id << ", MDP:" << mdp;

    // Prepare the SQL query to check credentials
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM login WHERE id = :id AND mdp = :mdp");
    query.bindValue(":id", id);
    query.bindValue(":mdp", mdp);

    if (!query.exec()) {
        qDebug() << "Error executing query:" << query.lastError().text();
        QMessageBox::critical(this, tr("Database Error"), tr("Failed to execute query."));
        return;
    }

    // Check if credentials are valid
    if (query.next() && query.value(0).toInt() > 0) {
        QMessageBox::information(this, tr("Login Successful"), tr("Welcome!"));

        // Hide the login dialog
        this->hide();

        // Create and show the options main window
        options* mainWin = new options;
        mainWin->show();
    } else {
        QMessageBox::warning(this, tr("Login Failed"), tr("Incorrect ID or password."));

        // Optionally, clear the input fields
        ui->lineEdit_ID->clear();
        ui->lineEdit_mdp->clear();
    }
}

