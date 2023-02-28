#ifndef PRODUCTOSCRUD_H
#define PRODUCTOSCRUD_H

#include <QFrame>
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

#define HOST_NAME "localhost"
#define USER_NAME "postgres"
#define PASSWORD "123"
#define DATABASE_NAME "northwind"

namespace Ui {
class ProductosCRUD;
};

class ProductosCRUD : public QFrame
{
    Q_OBJECT

public:
    explicit ProductosCRUD(QWidget *parent = nullptr);
    ~ProductosCRUD();

private slots:
    void on_PB_CRUD_Regresar_clicked();

private:
    Ui::ProductosCRUD *ui;
    QSqlDatabase database;
};

#endif // PRODUCTOSCRUD_H
