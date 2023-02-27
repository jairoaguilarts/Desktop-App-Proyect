#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <iostream>

using namespace std;

void asignaciones_constantes_fechas();
void generacion_idOrden();
void buscar_productos_por_nombre( QString& , QTableWidget* );


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    //Conexion a PSQL
    database = QSqlDatabase::addDatabase("QPSQL");
    database.setHostName(HOST_NAME);
    database.setUserName(USER_NAME);
    database.setPassword(PASSWORD);
    database.setDatabaseName(DATABASE_NAME);
    database.open();
    //Prueba para ver si se conecto la base de datos
    /*if(database.open()){
        QMessageBox::information(this, "CONEXION A POSTGRESQL", "Conexion establecida correctamente");
    }
    else{
         QMessageBox::information(this, "CONEXION A POSTGRESQL", "No se ha establecido conexion");
    }*/
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    database.close();
    delete ui;
}


void MainWindow::on_PB_CrearOrden_clicked()
{
    //Obtiene los datos de los Line Edits
    int ordenID = this->generarIDOrden();
    QString idCliente = ui->LE_IDCliente->text();
    QString idEmpleado = ui->LE_IDEmpleado->text();
    QDate fechaOrden = QDate::currentDate();
    QDate fechaRequerida = fechaOrden.addDays(2);
    QDate fechaEnviada; //null
    QString idAgencia = ui->LE_IDAgencia->text();
    QString peso = ui->LE_Peso->text();
    QString nombreBarco = ui->LE_NombreBarco->text();
    QString direccionEnvio = ui->LE_DireccionEnvio->text();
    QString ciudad = ui->LE_CiudadEnvio->text();
    QString region = ui->LE_RegionEnvio->text();
    QString codigoPostal = ui->LE_CodigoPostal->text();
    QString pais = ui->LE_Pais->text();

    //Agrega los datos a la tabla de Orders (revisen los campos de los atributos de la tabla orders en sus bases de datos)
    QString queryString = "INSERT INTO orders (order_id, customer_id, employee_id, order_date, required_date, ship_via, freight, ship_name, ship_address, ship_city, ship_region, ship_postal_code, ship_country) VALUES (:orderID,:customerID,:employeeID,:orderDate,:requiredDate,:shipVia,:freight,:shipName,:shipAddress,:shipCity,:shipRegion,:shipPostalCode,:shipCountry)";
    QSqlQuery query;
    query.prepare(queryString);
    query.bindValue(":orderID", ordenID);
    query.bindValue(":customerID", idCliente);
    query.bindValue(":employeeID", idEmpleado);
    query.bindValue(":orderDate", fechaOrden.toString());
    query.bindValue(":requiredDate", fechaRequerida.toString());
    query.bindValue(":shipVia", idAgencia.toInt());
    query.bindValue(":freight", peso.toDouble());
    query.bindValue(":shipName", nombreBarco);
    query.bindValue(":shipAddress", direccionEnvio);
    query.bindValue("shipCity", ciudad);
    query.bindValue(":shipRegion", region);
    query.bindValue(":shipPostalCode", codigoPostal);
    query.bindValue(":shipCountry", pais);
    if(!query.exec()) {
        QMessageBox::information(this, "INFO ORDEN", "La orden no pudo ser procesada correctamente");
        qDebug() << "Error: " << query.lastError().text();
    } else {
        orderID_flag = ordenID;

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

}

int MainWindow::generarIDOrden()
{
    QSqlQuery query(this->database);
    query.prepare("SELECT MAX(order_id) FROM orders");
    query.exec();
    query.first();
    int orderID = query.value(0).toInt();
    return orderID + 1;
}


void MainWindow::on_PB_agregardetalles_clicked()
{

    QString idProducto = ui->LE_producto->text();
    QString unitPrice;
    QSqlQuery query1;
    if (query1.exec(QString("SELECT unit_price FROM products WHERE product_id = '%1'").arg(idProducto)) && query1.next()) {
        unitPrice = query1.value(0).toString();
    } else {
        unitPrice = "";
    }

    //Obtiene los datos de los Line Edits
    QString precio = unitPrice;
    QString cantidad = ui->LE_cantidad->text();
    QString descuento = ui->LE_descuento->text();

    // Verifica que la orden exista en la tabla orders
    QSqlQuery query2;
    if (query2.exec(QString("SELECT order_id FROM orders WHERE order_id = '%1'").arg(orderID_flag)) && query2.next()) {
        //Inserta los datos a order_details
        QString queryString = "INSERT INTO order_details (order_id, product_id, unit_price, quantity, discount) VALUES (:orderID,:productID,:unitPrice,:quantity,:discount)";
        QSqlQuery query3;
        query3.prepare(queryString);
        query3.bindValue(":orderID", orderID_flag);
        query3.bindValue(":productID", idProducto.toInt());
        query3.bindValue(":unitPrice", precio.toDouble());
        query3.bindValue(":quantity", cantidad.toInt());
        query3.bindValue(":discount", descuento.toDouble());

        if(!query3.exec()){
            QMessageBox::information(this, "INFO ORDEN", "La orden no pudo ser procesada correctamente");
            qDebug() << "Error: " << query3.lastError().text();
        }
        else{
            //Limpia los campos de los Line Edits
            ui->LE_producto->clear();
            ui->LE_cantidad->clear();
            ui->LE_descuento->clear();
        }
    } else {
        QMessageBox::information(this, "INFO ORDEN", "La orden no existe");
        qDebug() << "Error: " << query2.lastError().text();
    }
}


void MainWindow::on_PB_Buscarproducto_clicked()
{
        QString IDproducto2 = ui->LE_productoDB->text();
        QTableWidget* tabla_resultados2 = ui->tabla_resultados;
        buscar_productos_por_nombre(IDproducto2, tabla_resultados2);
}


void buscar_productos_por_nombre( QString& nombre_producto, QTableWidget* tabla_resultados)
{
    // Realizar consulta SQL
    QSqlQuery query;
    query.prepare("SELECT * FROM products WHERE product_name = ?");
    query.addBindValue(nombre_producto);
    if (!query.exec()) {
        qDebug() << query.lastError().text();
        return;
    }
    tabla_resultados->clear();
       tabla_resultados->setRowCount(0);

    // Mostrar resultados en la tabla
    tabla_resultados->clear();
    tabla_resultados->setColumnCount(3);
    tabla_resultados->setHorizontalHeaderLabels({"ID Producto", "Nombre", "ID Proveedor"});
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
}


void MainWindow::on_LE_IDCliente_editingFinished()
{
    QString idCliente = ui->LE_IDCliente->text();
    QString nombre;
    QSqlQuery query;
    if (query.exec(QString("SELECT contact_name FROM customers WHERE customer_id = '%1'").arg(idCliente)) && query.next()) {
        nombre = query.value(0).toString();
    } else {
        nombre = "";
    }
}

