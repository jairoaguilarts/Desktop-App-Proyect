#include "productoscrud.h"
#include "ui_productoscrud.h"

productosCRUD::productosCRUD(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::productosCRUD)
{
    ui->setupUi(this);
}

productosCRUD::~productosCRUD()
{
    delete ui;
}
