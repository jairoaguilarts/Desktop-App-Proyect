#ifndef PRODUCTOSCRUD_H
#define PRODUCTOSCRUD_H

#include <QMainWindow>
#include <QDebug>
#include <QFileDialog>
#include <QStringList>
#include <QClipboard>
#include <QTime>
#include <QUrl>
#include <QMessageBox>
#include <iostream>
#include <vector>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QRandomGenerator>
#include <iostream>
#include "products.h"

using namespace std;

#define HOST_NAME "localhost"
#define USER_NAME "postgres"
#define PASSWORD "12345"
#define DATABASE_NAME "northwind"

#include <QDialog>

namespace Ui {
class productosCRUD;
}

class productosCRUD : public QDialog
{
    Q_OBJECT

public:
    explicit productosCRUD(QWidget *parent = nullptr);
    ~productosCRUD();

private slots:
    void on_PB_CrearProd_clicked();

    void on_A_PB_Actualizar_clicked();

    void on_PB_MostrarProd_clicked();

    void on_PB_DescProd_clicked();

private:
    Ui::productosCRUD *ui;
    QSqlDatabase database;
};

#endif // PRODUCTOSCRUD_H
