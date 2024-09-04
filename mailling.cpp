#include "mailling.h"
#include "ui_mailling.h"

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

#include "smtp.h"

Mailling::Mailling(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Mailling)
{
    ui->setupUi(this);
    connect(ui->sendBtn_2, &QPushButton::clicked, this, &Mailling::sendMail);
    connect(ui->browseBtn_2, &QPushButton::clicked, this, &Mailling::browse);
}

Mailling::~Mailling()
{
    delete ui;
}
void Mailling::browse()
{
    files.clear();

    QFileDialog dialog(this);
    dialog.setDirectory(QDir::homePath());
    dialog.setFileMode(QFileDialog::ExistingFiles);

    if (dialog.exec())
        files = dialog.selectedFiles();

    QString fileListString;
    for (const QString &file : files)
    {
        fileListString.append("\"" + QFileInfo(file).fileName() + "\" ");
    }

    ui->file_2->setText(fileListString);
}

void Mailling::sendMail()
{
    qDebug() << "Attempting to send email...";
    Smtp* smtp = new Smtp("rebai.youssef@esprit.tn", ui->paswd_2->text(), "smtp.gmail.com");

    connect(smtp, &Smtp::status, this, &Mailling::mailSent);

    if (!files.isEmpty()) {
        qDebug() << "Sending with attachments...";
        smtp->sendMail("rebai.youssef@esprit.tn", ui->rcpt_2->text(), ui->subject_2->text(), ui->msg_2->toPlainText(), files);
    } else {
        qDebug() << "Sending without attachments...";
        smtp->sendMail("rebai.youssef@esprit.tn", ui->rcpt_2->text(), ui->subject_2->text(), ui->msg_2->toPlainText());
    }

    qDebug() << "Mail sent method called.";
}


void Mailling::mailSent(QString status)
{
    if (status == "Message sent")
    {
        QMessageBox::information(nullptr, tr("Qt Simple SMTP client"), tr("Message sent!\n\n"));
    }
    else
    {
        QMessageBox::warning(nullptr, tr("Qt Simple SMTP client"), tr("Failed to send message.\n\n"));
    }

    // Clear the UI fields after sending
    ui->rcpt_2->clear();
    ui->subject_2->clear();
    ui->file_2->clear();
    ui->msg_2->clear();
    ui->paswd_2->clear();
}

