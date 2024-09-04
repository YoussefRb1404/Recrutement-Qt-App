#include "options.h"
#include "ui_options.h"
#include "gesoff.h"
#include "gescond.h"

options::options(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::options)
{
    ui->setupUi(this);
}

options::~options()
{
    delete ui;
}

void options::on_pushButton_Offre_clicked()
{
    GesOff* mainWin = new GesOff;
    mainWin->show();
}

void options::on_pushButton_Candidat_clicked()
{
    GesCond* mainWin = new GesCond;
    mainWin->show();
}
