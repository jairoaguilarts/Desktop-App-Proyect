#include "productoscrud.h"
#include "./ui_productoscrud.h"
#include <iostream>

ProductosCRUD::ProductosCRUD(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::ProductosCRUD)
{
    database = QSqlDatabase::addDatabase("QPSQL");
    database.setHostName(HOST_NAME);
    database.setUserName(USER_NAME);
    database.setPassword(PASSWORD);
    database.setDatabaseName(DATABASE_NAME);
    database.open();
    ui->setupUi(this);
}

ProductosCRUD::~ProductosCRUD()
{
    delete ui;
}

void ProductosCRUD::on_PB_CRUD_Regresar_clicked()
{

}

