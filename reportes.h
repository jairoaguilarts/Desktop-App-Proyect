#ifndef REPORTES_H
#define REPORTES_H

#include <QDialog>
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
#include <QSqlQueryModel>
#include <QSqlError>

#define HOST_NAME "localhost"
#define USER_NAME "postgres"
#define PASSWORD "12345"
#define DATABASE_NAME "northwind"

namespace Ui {
class Reportes;
}

class Reportes : public QDialog
{
    Q_OBJECT

public:

    explicit Reportes(QWidget *parent = nullptr);
    ~Reportes();

    Ui::Reportes *uiReporte;

    void setCliente(bool);

    void setProveedor(bool);

    void setCategoria(bool);

private slots:
    void on_PB_Ok_clicked();

    void on_pushButton_clicked();

private:
    QSqlDatabase database;

    bool cliente = false;

    bool proveedor = false;

    bool categoria = false;
};

#endif // REPORTES_H
