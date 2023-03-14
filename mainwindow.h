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
#include <cctype>
#include <cstdlib>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlError>
#include <QTableWidget>
#include <QRandomGenerator>
#include <QPrinter>
#include <QPainter>

//Cambiar
#define HOST_NAME "localhost"
#define USER_NAME "postgres"
#define PASSWORD "123"
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

    void on_emitirorden_clicked();

    void on_PB_CrearProducto_clicked();

    void on_PB_ActualizarProducto_clicked();

    void on_PB_Descontinuar_clicked();

    void on_tabWidget_currentChanged(int index);

    void on_PB_CrearEmpleado_clicked();

    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_2_clicked();

    void on_CL_PB_CrearCliente_clicked();

    void on_CL_CB_CamposAModificar_currentIndexChanged(int);

    void on_CL_PB_Actualizat_clicked();

private:
    Ui::MainWindow *ui;
    QSqlDatabase database;

    QString imagePath = "";

    QString campo = "";

    QString dato = "";

    int orderID_flag;

    int generarIDOrden();

    int generarIDProducto();

    int generarIDEmpleado();

    QString generarIDCliente(QString);

    void cargarClientes();

    void cargarEmpleados();

    void cargarTitulos();

    void cargarAgencias();

    void cargarProductos();

    void cargarProveedores();

    void cargarCategorias();

    void actualizarTabla();

    void mostrarProductos(QTableWidget*);

    void mostrarEmpleados();

    void descontinuarProducto(QTableWidget*, QString&);

    void mostrarClientes();

};
#endif // MAINWINDOW_H
