#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <iostream>
#include <time.h>

using namespace std;

void asignaciones_constantes_fechas();
void generacion_idOrden();

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_PB_CrearOrden_clicked()
{
    //Obtiene los datos de los Line Edits
    QString idCliente = ui->LE_IDCliente->text();
    QString idEmpleado = ui->LE_IDEmpleado->text();
    QString idAgencia = ui->LE_IDAgencia->text();
    QString peso = ui->LE_Peso->text();
    QString nombreBarco = ui->LE_NombreBarco->text();
    QString direccionEnvio = ui->LE_DireccionEnvio->text();
    QString ciudad = ui->LE_CiudadEnvio->text();
    QString region = ui->LE_RegionEnvio->text();
    QString codigoPostal = ui->LE_CodigoPostal->text();
    QString pais = ui->LE_Pais->text();
    connectPSQL();
    //Limpia los campos de los Line Edits
    ui->LE_IDCliente->clear();
    ui->LE_IDEmpleado->clear();
    ui->LE_IDAgencia->clear();
    ui->LE_Peso->clear();
    ui->LE_NombreBarco->clear();
    ui->LE_DireccionEnvio->clear();
    ui->LE_CiudadEnvio->clear();
    ui->LE_RegionEnvio->clear();
    ui->LE_CodigoPostal->clear();
    ui->LE_Pais->clear();

}

int MainWindow::connectPSQL()
{

    return 0;
}

void asignaciones_constantes_fechas(){
    time_t tp = time(nullptr);
    tm* fo = localtime(&tp);
    string fecha_orden, fecha_requerida, fecha_enviada = NULL;  //Fechas requeridas del inciso b

    int dia = fo->tm_mday;
    int mes = fo->tm_mon + 1;
    int anno = fo->tm_year + 1900;

    fecha_orden = to_string(dia) + "/" + to_string(mes) + "/" + to_string(anno);

    tp += 172800;
    fo = localtime(&tp);
    mes = fo->tm_mon + 1;
    dia = fo->tm_mday;
    anno = fo->tm_year + 1900;

    fecha_requerida = to_string(dia) + "/" + to_string(mes) + "/" + to_string(anno);

    cout << fecha_orden << endl;
    cout << fecha_requerida;
}

void generacion_idOrden(){
    srand(time(NULL));

    int id_orden = 10000 + rand() % (100000 - 10000);       //Generacion Aleatoria del OrderID
    cout << id_orden << endl;
}
