#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "orderdetails.h"
#include "reportes.h"
#include "ui_reportes.h"
#include <iostream>
#include <QPrinter>
#include <QPainter>

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
    query.bindValue(":shipCity", ciudad);
    query.bindValue(":shipRegion", region);
    query.bindValue(":shipPostalCode", codigoPostal);
    query.bindValue(":shipCountry", pais);
    if(!query.exec()) {
        QMessageBox::information(this, "INFO ORDEN", "La orden no pudo ser procesada correctamente");
        qDebug() << "Error: " << query.lastError().text();
    } else {
        orderID_flag = ordenID;
        //Instancia de una orden
        Order *order = new Order(ordenID, idCliente.toStdString(), idEmpleado.toStdString(), (fechaOrden.toString()).toStdString(), (fechaRequerida.toString()).toStdString(),
                                 idAgencia.toInt(), peso.toDouble(),nombreBarco.toStdString(), direccionEnvio.toStdString(), ciudad.toStdString(), region.toStdString(),
                                 codigoPostal.toStdString(), pais.toStdString());
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
        ui->nombreCliente->setText("");
        ui->nombreEmpleado->setText("");
        ui->nombreAgencia->setText("");
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
        query3.bindValue(":unitPrice", unitPrice.toDouble());
        query3.bindValue(":quantity", cantidad.toInt());
        query3.bindValue(":discount", descuento.toDouble());

        if(!query3.exec()){
            QMessageBox::information(this, "INFO ORDEN", "La orden no pudo ser procesada correctamente");
            qDebug() << "Error: " << query3.lastError().text();
        }
        else{
            //Instancia de un detalle de orden
            OrderDetails *orderDetail = new OrderDetails(orderID_flag, idProducto.toInt(), unitPrice.toDouble(), cantidad.toInt(),
                                                         descuento.toDouble());
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
        nombre = "Cliente no encontrado";
    }
    ui->nombreCliente->setText(nombre);
}


void MainWindow::on_LE_IDEmpleado_editingFinished()
{
    QString idEmpleado = ui->LE_IDEmpleado->text();
    QString nombre;
    QSqlQuery query;
    if (query.exec(QString("SELECT first_name, last_name FROM employees WHERE employee_id = '%1'").arg(idEmpleado)) && query.next()) {
        nombre = query.value(0).toString() + " " + query.value(1).toString();
    } else {
        nombre = "Empleado no encontrado";
    }
    ui->nombreEmpleado->setText(nombre);
}


void MainWindow::on_LE_IDAgencia_editingFinished()
{
    QString idAgencia = ui->LE_IDAgencia->text();
    QString nombre;
    QSqlQuery query;
    if (query.exec(QString("SELECT company_name FROM shippers WHERE shipper_id = '%1'").arg(idAgencia)) && query.next()) {
        nombre = query.value(0).toString();
    } else {
        nombre = "Agencia no encontrada";
    }
    ui->nombreAgencia->setText(nombre);
}

void MainWindow::on_emitirorden_clicked()
{
    // Mostrar un diálogo de selección de archivo para que el usuario pueda seleccionar la ubicación y el nombre del archivo
    QString filename = QFileDialog::getSaveFileName(this, tr("Guardar archivo PDF"), QString(), tr("PDF files (*.pdf)"));
    if (filename.isEmpty()) {
        return;
    }

    // Crear un objeto QPrinter y establecer sus propiedades
    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(filename);
    printer.setPageSize(QPageSize(QPageSize::EnvelopeC65));

    // Crear un objeto QPainter para dibujar en el QPrinter
    QPainter painter;
    if (!painter.begin(&printer)) {
        qWarning() << "No se puede iniciar la impresión del documento PDF.";
        return;
    }

    // Establecer la fuente y el tamaño del texto
    QFont font("Times New Roman", 14);
    painter.setFont(font);

    // Establecer el espaciado de línea
    painter.setPen(Qt::blue);
    painter.drawText(0, 50, " ");
    painter.drawText(0, 200, " ");
    painter.drawText(0, 400, " ");
    painter.drawText(1900, 600, "FACTURA ");

    painter.setPen(Qt::black);

    // Realizar una consulta para obtener información de la orden
    QSqlQuery query;
    query.prepare("SELECT customers.contact_name AS customer_name, "
                      "employees.last_name || ', ' || employees.first_name AS employee_name, "
                      "shippers.company_name AS shipper_name, "
                      "orders.freight, "
                      "orders.ship_name, "
                      "orders.ship_address, "
                      "orders.ship_city, "
                      "orders.ship_region, "
                      "orders.ship_postal_code, "
                      "orders.ship_country "
                      "FROM orders "
                      "JOIN customers ON orders.customer_id = customers.customer_id "
                      "JOIN employees ON orders.employee_id = employees.employee_id "
                      "JOIN shippers ON orders.ship_via = shippers.shipper_id "
                      "WHERE orders.order_id = :order_id");


    query.bindValue(":order_id", orderID_flag);

    // Ejecutar la consulta y verificar si hay errores
    if (!query.exec()) {
        qWarning() << "Error al ejecutar la consulta: " << query.lastError().text();
        return;
    }

    // Comprobar si se encontró la orden
    if (!query.first()) {
        qWarning() << "No se encontró la orden con ID" << orderID_flag;
        return;
    }

    // Obtener los valores de la consulta
    QString nombreCliente = query.value("customer_name").toString();
    QString nombreEmpleado = query.value("employee_name").toString();
    QString nombreAgenciaEnvio = query.value("shipper_name").toString();
    QString peso = query.value("freight").toString();
    QString nombreBarco = query.value("ship_name").toString();
    QString direccionEnvio = query.value("ship_address").toString();
    QString ciudadEnvio = query.value("ship_city").toString();
    QString regionEnvio = query.value("ship_region").toString();
    QString codigoPostalEnvio = query.value("ship_postal_code").toString();
    QString paisEnvio = query.value("ship_country").toString();

    //Coordenadas
    int x = 1200;
    int y = 1200;

    // Dibujar los textos con la información de la orden
    painter.setFont(QFont("Times New Roman", 12, QFont::Normal));
    painter.drawText(x, y, "Cliente:................." + nombreCliente);
    y += 200;
    painter.drawText(x, y, "Empleado:................" + nombreEmpleado);
    y += 200;
    painter.drawText(x, y, "Agencia de envío:......." + nombreAgenciaEnvio);
    y += 200;
    painter.drawText(x, y, "Peso:..................." + peso);
    y += 200;
    painter.drawText(x, y, "Nombre del barco:......." + nombreBarco);
    y += 200;
    painter.drawText(x, y, "Dirección de envío:....." + direccionEnvio);
    y += 200;
    painter.drawText(x, y, "Ciudad de envío:........" + ciudadEnvio);
    y += 200;
    painter.drawText(x, y, "Región de envío:........" + regionEnvio);
    y += 200;
    painter.drawText(x, y, "Código postal de envío: " + codigoPostalEnvio);
    y += 200;
    painter.drawText(x, y, "País de envío:.........." + paisEnvio);
    y += 600;
    painter.setFont(QFont("Times New Roman", 14, QFont::Normal));

    // Establecer el espaciado de línea
    painter.setPen(Qt::blue);
    painter.drawText(x, y, "DETALLES DE LA ORDEN ");
    painter.setPen(Qt::black);

    // Obtener los detalles de la orden
    QSqlQuery query2;
    query2.prepare("SELECT order_details.product_id, products.product_name, order_details.unit_price, order_details.quantity, order_details.discount "
                  "FROM order_details "
                  "JOIN products ON order_details.product_id = products.product_id "
                  "WHERE order_details.order_id = :order_id");
    query2.bindValue(":order_id", orderID_flag);

    QString nombreProducto;
    QString precioUnitario;
    QString cantidad;
    QString descuento;

    y += 400;
    float sub_total = 0;
    float total = 0;
    float total1 = 0;
    // Ejecutar la consulta y verificar si hay errores
    if (query2.exec()) {
        while(query2.next()){
            // Obtener los valores de la consulta
            nombreProducto = query2.value("product_name").toString();
            precioUnitario = query2.value("unit_price").toString();
            cantidad = query2.value("quantity").toString();
            descuento = query2.value("discount").toString();
            double precioUnitarioNum =precioUnitario.toDouble() ;
            int cantidadNum = cantidad.toInt();
            float descuento2 = descuento.toDouble();

            sub_total +=  cantidadNum * precioUnitarioNum;
            total += sub_total * descuento2;
            total1 += sub_total - total;

            // Dibujar los textos con la información de la orden
            painter.setFont(QFont("Times New Roman", 12, QFont::Normal));
            painter.drawText(x, y, "Nombre del producto:...." + nombreProducto);
            y += 200;
            painter.drawText(x, y, "Precio unitario:........" + precioUnitario);
            y += 200;
            painter.drawText(x, y, "Cantidad de producto:..." + cantidad);
            y += 200;
            painter.drawText(x, y, "Descuento de producto:.." + descuento);
            y += 400;
        }
        painter.drawText(x, y,QString("Subtotal: .....  $%1").arg(sub_total));
        y += 200;
        painter.drawText(x, y, QString("Total: .......  $%1").arg(total1));
        y += 500;
        x+=600;
        painter.drawText(x, y, "||||||||||||||||||||||||||||||||||" );
        y += 100;
        painter.drawText(x, y, "||||||||||||||||||||||||||||||||||" );
        x+=300;
        y += 200;
        painter.drawText(x, y, "90218328" );
    } else {
        qWarning() << "Error al ejecutar la consulta de detalles de la orden: " << query2.lastError().text();
        return;
    }

    // Comprobar si la impresión ha finalizado correctamente
    if (!painter.end()) {
        qWarning() << "No se puede finalizar la impresión del documento PDF.";
        return;
    }

    // Mostrar un mensaje de confirmación al usuario
    QMessageBox::information(this, "Documento PDF creado", "El documento PDF se ha creado correctamente.");
}

void MainWindow::on_PB_reporteCliente_clicked()
{
    Reportes *reporte = new Reportes();
    reporte->setCliente(true);
    reporte->uiReporte->LB_solicitarID->setText("Ingrese el ID del cliente");
    reporte->setModal(true);
    reporte->show();
}


void MainWindow::on_PB_reporteProveedor_clicked()
{
    Reportes *reporte = new Reportes();
    reporte->setProveedor(true);
    reporte->uiReporte->LB_solicitarID->setText("Ingrese el ID del proveedor");
    reporte->setModal(true);
    reporte->show();
}


void MainWindow::on_PB_reporteCategoria_clicked()
{
    Reportes *reporte = new Reportes();
    reporte->setCategoria(true);
    reporte->uiReporte->LB_solicitarID->setText("Ingrese el ID de la categoria");
    reporte->setModal(true);
    reporte->show();
}


void MainWindow::on_PB_CRUDProductos_clicked()
{
    productosCRUD *pWindow = new productosCRUD();
    pWindow->setModal(true);
    pWindow->show();
}

