#include "mailling.h"
#include "ui_mailling.h"

Mailling::Mailling(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Mailling)
{
    ui->setupUi(this);
}

Mailling::~Mailling()
{
    delete ui;
}
