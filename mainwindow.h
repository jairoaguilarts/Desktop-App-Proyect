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
#include <QInputDialog>

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
    //void on_PB_agregardetalles_clicked();
    void on_emitirorden_clicked();
    void on_PB_CrearProducto_clicked();
    void on_PB_ActualizarProducto_clicked();
    void on_PB_Descontinuar_clicked();
    void on_tabWidget_currentChanged(int index);
    void on_PB_CrearEmpleado_clicked();
    void on_pushButton_clicked();
    void on_EMP_PB_ActualizarEstaShit_clicked();
    void on_PB_EliminarDetalle_clicked();
    void on_LE_BuscarProducto_textChanged(const QString &arg1);
    void on_CL_PB_CrearCliente_clicked();
    void on_CL_CB_CamposAModificar_currentIndexChanged(int);
    void on_CL_PB_Actualizat_clicked();
    void on_PB_AgregarDetalle_clicked();
    void on_EMP_CB_SeleccionDato_currentIndexChanged(int index);


    void on_EMP_RB_SeleccionDeElemento_toggled(bool checked);

    void on_CL_RB_SeleccionadorDeElementos_toggled(bool checked);

    void on_CB_Ordenes_currentIndexChanged(int index);

    void on_PB_Devolver_clicked();

    void on_PB_devolver_clicked();

private:
    Ui::MainWindow *ui;
    QSqlDatabase database;

    QString imagePath = "";

    QString campo = "";

    QString dato = "";

    QString id = "";

    bool seleccion_empleado = false;

    bool seleccion_cliente = false;

    int orderID_flag;
    int generarIDOrden();
    int generarIDProducto();
    int generarIDEmpleado();
    QString generarIDCliente(QString);
    void minorSettings(int);
    void cargarContactos();
    void cargarClientes();
    void cargarEmpleados();
    void cargarTitulos();
    void cargarAgencias();
    //void cargarTituloEmpleado();
    void cargarTituloCorEMP();
    void cargarProductos();
    void cargarProveedores();
    void cargarCategorias();
    void cargarOrdenes();
    void actualizarTabla();
    void mostrarProductos(QTableWidget*);
    void mostrarEmpleados();
    void buscarProducto(const QString &);
    void descontinuarProducto(QTableWidget*, QString&);
    void variable_busqueda(QString& searchText);
    void mostrarClientes();
    int mostrarDetallesOrden(QString);
    QString getIDFromContactName(QString);
    QString shortcutParaManager(QString);
    void defaultConfig();

};
#endif // MAINWINDOW_H
