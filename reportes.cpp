#include "reportes.h"
#include "ui_reportes.h"

Reportes::Reportes(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Reportes)
{
    ui->setupUi(this);
}

Reportes::~Reportes()
{
    delete ui;
}

void Reportes::openNewWindow()
{
    Reportes *newWindow = new Reportes();
    newWindow->exec();
}

