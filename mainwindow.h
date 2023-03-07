#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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
#include <QTableWidget>
#include <QRandomGenerator>
#include "order.h"
#include "productoscrud.h"

//Cambiar
#define HOST_NAME "localhost"
#define USER_NAME "postgres"
#define PASSWORD "12345"
#define DATABASE_NAME "northwind"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_PB_CrearOrden_clicked();

    void on_PB_agregardetalles_clicked();

    //void on_PB_Buscarproducto_clicked();

    void on_emitirorden_clicked();

    //void on_PB_CrearProd_clicked();

    void on_PB_CrearProducto_clicked();

    void on_PB_ActualizarProducto_clicked();

    void on_PB_MostrarProd_3_clicked();

    void on_PB_Descontinuar_clicked();

private:
    Ui::MainWindow *ui;
    QSqlDatabase database;

    int orderID_flag;

    int generarIDOrden();

    int generarIDProducto();

    void cargarClientes();

    void cargarEmpleados();

    void cargarAgencias();

    void cargarProductos();

    void cargarProveedores();

    void cargarCategorias();

    void actualizarTabla();

    void mostrarProductos(QTableWidget*);

    void descontinuarProducto(QTableWidget*, QString&);

};
#endif // MAINWINDOW_H
