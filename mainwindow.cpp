#include "mainwindow.h"
#include "./ui_mainwindow.h"


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
    QString query_texto = "INSERT INTO public.orders ( \"OrderID\", \"CustomerID\", \"EmployeeID\", \"OrderDate\", \"RequiredDate\", \"ShippedDate\", \"ShipVia\", \"Freight\", \"ShipName\", \"ShipAddress\", \"ShipCity\", \"ShipRegion\", \"ShipPostalCode\", \"ShipCountry\")";
    query_texto += "VALUES (11078, \'"+idCliente+"\', "+idEmpleado+", \'2023-02-25\', \'2023-02-28\', NULL, "+idAgencia+", "+peso+", \'"+nombreBarco+"\', \'"+direccionEnvio+"\', \'"+ciudad+"\', "+region+", \'"+codigoPostal+"\', \'"+pais+"\')";
    QSqlQuery query_array;
    if(query_array.exec(query_texto)){
        QMessageBox::information(this, "INFO ORDEN", "Orden procesada exitosamente");
    }
    else{
        QMessageBox::information(this, "INFO ORDEN", "La orden no pudo ser procesada correctamente");
    }
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
    database = QSqlDatabase::addDatabase("QPSQL");
    database.setHostName(HOST_NAME);
    database.setUserName(USER_NAME);
    database.setPassword(PASSWORD);
    database.setDatabaseName(DATABASE_NAME);

    if(database.open()){
        QMessageBox::information(this, "CONEXION A POSTGRESQL", "Conexion establecida correctamente");
    }
    else{
         QMessageBox::information(this, "CONEXION A POSTGRESQL", "No se ha establecido conexion");
    }
    return 0;
}
