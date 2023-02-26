#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <iostream>
#include <time.h>

using namespace std;

void asignaciones_constantes_fechas();
void generacion_idOrden();
void buscar_productos_por_nombre( QString& , QTableWidget* );


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

void MainWindow::on_PB_agregardetalles_clicked()
{
    //Obtiene los datos de los Line Edits
            QString idProducto = ui->LE_producto->text();
            QString precio = ui->LE_precio->text();
            QString descuento = ui->LE_descuento->text();

            QString query_texto = "INSERT INTO public.ordersdetails ( \"Idproductos\", \"Price\", \"discount\")";
            query_texto += "VALUES ("+idProducto+"\', "+precio+"\', "+precio+", "+descuento+"\')";
            QSqlQuery query_array;
            if(query_array.exec(query_texto)){
                QMessageBox::information(this, "INFO ORDEN", "Orden procesada exitosamente");
            }
            else{
                QMessageBox::information(this, "INFO ORDEN", "La orden no pudo ser procesada correctamente");
            }

            //Limpia los campos de los Line Edits
            ui->LE_producto->clear();
            ui->LE_precio->clear();
            ui->LE_descuento->clear();
}




void MainWindow::on_PB_Buscarproducto_clicked()
{
        QString IDproducto2 = ui->LE_productoDB->text();
        QTableWidget* tabla_resultados2 = ui->tabla_resultados;
        buscar_productos_por_nombre(IDproducto2, tabla_resultados2);
}



void buscar_productos_por_nombre( QString& nombre_producto, QTableWidget* tabla_resultados)
{
    // Configurar conexión a la base de datos
    QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL");
    db.setHostName(HOST_NAME);
    db.setUserName(USER_NAME);
    db.setPassword(PASSWORD);
    db.setDatabaseName(DATABASE_NAME);


    // Conectar a la base de datos
    if (!db.open()) {
        std::cerr << "Error al conectar a la base de datos" << std::endl;
        return;
    }

    // Realizar consulta SQL
    QSqlQuery query;
    query.prepare("SELECT * FROM public.productos WHERE nombre = ?");
    query.addBindValue(nombre_producto);
    if (!query.exec()) {
        std::cerr << "Error al ejecutar la consulta SQL" << std::endl;
        db.close();
        return;
    }

    // Mostrar resultados en la tabla
    tabla_resultados->clear();
    tabla_resultados->setColumnCount(3);
    tabla_resultados->setHorizontalHeaderLabels({"ID Producto", "Precio", "Descuento"});
    int row = 0;
    while (query.next()) {
        tabla_resultados->insertRow(row);
        tabla_resultados->setItem(row, 0, new QTableWidgetItem(QString::number(query.value(0).toInt())));
        tabla_resultados->setItem(row, 1, new QTableWidgetItem(query.value(1).toString()));
        tabla_resultados->setItem(row, 2, new QTableWidgetItem(QString::number(query.value(2).toFloat())));
        row++;
    }

    if (row == 0) {
        tabla_resultados->setRowCount(1);
        tabla_resultados->setItem(0, 0, new QTableWidgetItem("No se encontraron productos con el nombre " + nombre_producto));
    }

    // Cerrar conexión a la base de datos
    db.close();
}




