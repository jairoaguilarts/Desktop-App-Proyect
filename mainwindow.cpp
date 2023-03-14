#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <iostream>

using namespace std;

void asignaciones_constantes_fechas();
void generacion_idOrden();
//void buscar_productos_por_nombre( QString& , QTableWidget* );


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
    cargarClientes();
    cargarEmpleados();
    cargarAgencias();
    cargarProductos();
    cargarProveedores();
    cargarCategorias();
    cargarTitulos();
    mostrarClientes();
    ui->CB_Descontinuado->addItem("Si");
    ui->CB_Descontinuado->addItem("No");
    ui->DE_fechaContratado->setDate(QDate::currentDate());
    //Cosas ocultas para fines esteticos
    ui->CL_CB_TitContacto2->setHidden(true);
    ui->CL_TL_ModTitulo->setHidden(true);
}

MainWindow::~MainWindow()
{
    database.close();
    delete ui;
}

void MainWindow::cargarClientes()
{
    QSqlQuery query;
    query.prepare("select contact_name from customers ORDER BY customer_id ASC");
    if(query.exec()){
        QStringList items;
        while (query.next()) {
            items.append(query.value(0).toString());
        }
        ui->CB_Clientes->addItems(items);
    }
}

void MainWindow::cargarEmpleados()
{
    QSqlQuery query;
    query.prepare("select first_name, last_name from employees ORDER BY employee_id ASC");
    if(query.exec()) {
        QStringList items;
        while (query.next()) {
            items.append(query.value(0).toString() + " " + query.value(1).toString());
        }
        ui->CB_Empleado->addItems(items);
        ui->CB_Empleado_2->addItems(items);
        ui->CB_Empleado_3->addItems(items);
        ui->CB_Empleado_4->addItems(items);
    }
}

void MainWindow::cargarAgencias()
{
    QSqlQuery query;
    query.prepare("select company_name from shippers");
    if(query.exec()) {
        QStringList items;
        while (query.next()) {
            items.append(query.value(0).toString());
        }
        ui->CB_Agencia->addItems(items);
    }
}

void MainWindow::cargarTitulos(){
    QSqlQuery query;
    ui->CL_CB_TituloContacto->clear();
    ui->CL_CB_TitContacto2->clear();
    query.prepare("select distinct contact_title from customers order by contact_title asc");
    if(query.exec()) {
        QStringList items;
        while (query.next()) {
            items.append(query.value(0).toString());
        }
        ui->CL_CB_TituloContacto->addItems(items);
        ui->CL_CB_TitContacto2->addItems(items);
    }
}

void MainWindow::cargarProductos()
{
    QSqlQuery query;
    query.prepare("select product_name from products ORDER BY product_id ASC");
    if(query.exec()) {
        QStringList items;
        while (query.next()) {
            items.append(query.value(0).toString());
        }
        ui->CB_Productos->addItems(items);
        ui->CB_Productos_2->addItems(items);
        ui->CB_Productos_3->addItems(items);
    }
}

void MainWindow::cargarProveedores()
{
    QSqlQuery query;
    query.prepare("select company_name from suppliers");
    if(query.exec()) {
        QStringList items;
        while (query.next()) {
            items.append(query.value(0).toString());
        }
        ui->CB_Proveedor->addItems(items);
    }
}

void MainWindow::cargarCategorias()
{
    QSqlQuery query;
    query.prepare("select category_name from categories ORDER BY category_id ASC");
    if(query.exec()) {
        QStringList items;
        while (query.next()) {
            items.append(query.value(0).toString());
        }
        ui->CB_Categoria->addItems(items);
    }
}

void MainWindow::on_PB_CrearOrden_clicked()
{
    int ordenID = this->generarIDOrden();

    QString nombreCliente = ui->CB_Clientes->currentText();
    QString idCliente;
    //Obtiende el ID del customer
    QSqlQuery queryCliente;
    queryCliente.prepare("SELECT customer_id FROM customers WHERE contact_name = ?");
    queryCliente.addBindValue(nombreCliente);
    if(queryCliente.exec() && queryCliente.next()){
        idCliente = queryCliente.value(0).toString();
    }

    QString nombreEmpleado = ui->CB_Empleado->currentText();
    QStringList nombreSeparado = nombreEmpleado.split(" ");
    QString idEmpleado;
    //Obtiene el ID del employee
    QSqlQuery queryEmpleado;
    queryEmpleado.prepare("SELECT employee_id FROM employees WHERE first_name = ? AND last_name = ?");
    queryEmpleado.addBindValue(nombreSeparado.first());
    queryEmpleado.addBindValue(nombreSeparado.last());
    if(queryEmpleado.exec() && queryEmpleado.next()){
        idEmpleado = queryEmpleado.value(0).toString();
    }

    QString nombreAgencia = ui->CB_Agencia->currentText();
    QString idAgencia;
    //Obtiene el shipper_id de shippers
    QSqlQuery queryAgencia;
    queryAgencia.prepare("SELECT shipper_id FROM shippers WHERE company_name = ?");
    queryAgencia.addBindValue(nombreAgencia);
    if(queryAgencia.exec() && queryAgencia.next()){
        idAgencia = queryAgencia.value(0).toString();
    }

    //Crea las fechas
    QDate fechaOrden = QDate::currentDate();
    QDate fechaRequerida = fechaOrden.addDays(2);

    //Obtiene los datos de los Line Edits
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
        /*Order *order = new Order(ordenID, idCliente.toStdString(), idEmpleado.toStdString(), (fechaOrden.toString()).toStdString(), (fechaRequerida.toString()).toStdString(),
                                 idAgencia.toInt(), peso.toDouble(),nombreBarco.toStdString(), direccionEnvio.toStdString(), ciudad.toStdString(), region.toStdString(),
                                 codigoPostal.toStdString(), pais.toStdString());*/
        //Limpia los campos de los Line Edits
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

void MainWindow::actualizarTabla()
{
    QSqlQueryModel *queryModel = new QSqlQueryModel();
    queryModel->setQuery(QString("SELECT product_name, products.unit_price, quantity, discount FROM order_details INNER JOIN products ON order_details.product_id = products.product_id WHERE order_details.order_id = %1").arg(orderID_flag));
    ui->TV_detalles->setModel(queryModel);
    ui->TV_detalles->resizeColumnsToContents();
    ui->TV_detalles->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

void MainWindow::on_PB_agregardetalles_clicked()
{
    QString nombreProducto = ui->CB_Productos->currentText();
    QString idProducto;
    //Obtiene el produc_id de products
    QSqlQuery queryProducto;
    queryProducto.prepare("SELECT product_id FROM products WHERE product_name = ?");
    queryProducto.addBindValue(nombreProducto);
    if(queryProducto.exec() && queryProducto.next()){
        idProducto = queryProducto.value(0).toString();
    }

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
    QSqlQuery querycant;
    int cant;
    if(querycant.exec(QString("SELECT units_in_stock FROM products WHERE product_id = '%1'").arg(idProducto)) && querycant.next()){
        cant = querycant.value(0).toInt();
    }
    else{
        cant = 0;
    }
    if(cantidad.toInt() <= cant){
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
                /*OrderDetails *orderDetail = new OrderDetails(orderID_flag, idProducto.toInt(), unitPrice.toDouble(), cantidad.toInt(),
                                                             descuento.toDouble());*/
                //ACTUALIZA CANT PRODUCTOS
                int newCant = cant - cantidad.toInt();
                QString str = "UPDATE products SET units_in_stock = :newCant WHERE product_id = :productID";
                QSqlQuery queryprod;
                queryprod.prepare(str);
                queryprod.bindValue(":newCant", newCant);
                queryprod.bindValue(":productID", idProducto);
                if(queryprod.exec()){
                    QMessageBox::information(this, "PRODUCTOS", "Se actualizo la cantidad de productos");
                }
                //Limpia los campos de los Line Edits
                ui->LE_cantidad->clear();
                ui->LE_descuento->clear();
            }
        } else {
            QMessageBox::information(this, "INFO ORDEN", "La orden no existe");
            qDebug() << "Error: " << query2.lastError().text();
        }
    }
    else{
        QMessageBox::information(this, "PRODUCTO", "No existe los suficientes productos");
    }
    actualizarTabla();
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
    float descuentoNum = 0;
    float total = 0;
    // Ejecutar la consulta y verificar si hay errores
    if (query2.exec()) {
        while(query2.next()){
            // Obtener los valores de la consulta
            nombreProducto = query2.value("product_name").toString();
            precioUnitario = query2.value("unit_price").toString();
            cantidad = query2.value("quantity").toString();
            descuento = query2.value("discount").toString();
            double precioUnitarioNum =precioUnitario.toDouble() ;

            float totalDetalle = (cantidad.toInt()) * precioUnitarioNum;

            sub_total += totalDetalle;
            descuentoNum = totalDetalle * (descuento.toDouble());
            total += totalDetalle - descuentoNum;

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
        painter.drawText(x, y, QString("Total: .......  $%1").arg(total));
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

int MainWindow::generarIDProducto(){
    QSqlQuery query;
    query.prepare("SELECT MAX(product_id) FROM products");
    query.exec();
    query.first();
    int prod_ID = query.value(0).toInt();
    return prod_ID + 1;
}

void MainWindow::on_PB_CrearProducto_clicked()
{
    int id = this->generarIDProducto();

    QString suppName = ui->CB_Proveedor->currentText();
    QString supp_id;
    //Obtiene el supplier_id de suppliers
    QSqlQuery querySupplier;
    querySupplier.prepare("SELECT supplier_id FROM suppliers WHERE company_name = ?");
    querySupplier.addBindValue(suppName);
    if(querySupplier.exec() && querySupplier.next()){
        supp_id = querySupplier.value(0).toString();
    }

    QString catName = ui->CB_Categoria->currentText();
    QString cat_id;
    //Obtiene el category_id de categories
    QSqlQuery queryCategory;
    queryCategory.prepare("SELECT category_id FROM categories WHERE category_name = ?");
    queryCategory.addBindValue(catName);
    if(queryCategory.exec() && queryCategory.next()){
        cat_id = queryCategory.value(0).toString();
    }

    QString descontinuado;
    if(ui->CB_Descontinuado->currentIndex() == 0)
        descontinuado = "1";
    else
        descontinuado = "0";

    //Obtiene los datos de los line edits
    QString nombre = ui->LE_P_Nom->text();
    QString cantXund = ui->LE_P_Cant->text();
    QString preXund = ui->LE_P_Precio->text();
    QString und_Disp = ui->LE_P_Disponibles->text();
    QString und_Ord = ui->LE_P_Ordenadas->text();
    QString niv_Reorden = ui->LE_P_Reorden->text();



    QString queryString = "INSERT INTO products (product_id, product_name, supplier_id, category_id, quantity_per_unit, unit_price, units_in_stock, units_on_order, reorder_level, discontinued) "
                          "VALUES (:ID, :nombre, :suppID, :catID, :cantUnd, :preUnd, :undDis, :undOrd, :nivReorden, :descontinuado)";
    QSqlQuery query;
    query.prepare(queryString);
    query.bindValue(":ID", id);
    query.bindValue(":nombre", nombre);
    query.bindValue(":suppID", supp_id);
    query.bindValue(":catID", cat_id);
    query.bindValue(":cantUnd", cantXund);
    query.bindValue(":preUnd", preXund);
    query.bindValue(":undDis", und_Disp);
    query.bindValue(":undOrd", und_Ord);
    query.bindValue(":nivReorden", niv_Reorden);
    query.bindValue(":descontinuado", descontinuado);

    if(!query.exec()) {
        QMessageBox::information(this, "INFO PRODUCTO", "El producto NO pudo ser creado exitosamente");
        qDebug() << "Error: " << query.lastError().text();
    } else {
        QMessageBox::information(this, "INFO PRODUCTO", "El producto ha sido creado exitosamente");
        /*Products *producto = new Products(id, nombre.toStdString(), supp_id.toInt(),
                                          cat_id.toInt(), cantXund.toStdString(), preXund.toDouble(),und_Disp.toInt(),
                                          und_Ord.toInt(),niv_Reorden.toInt(), descontinuado.toInt());*/
        ui->LE_P_Nom->clear();
        ui->LE_P_Cant->clear();
        ui->LE_P_Precio->clear();
        ui->LE_P_Disponibles->clear();
        ui->LE_P_Ordenadas->clear();
        ui->LE_P_Reorden->clear();
    }
    cargarProductos();
    mostrarProductos(ui->TV_Productos);
}


void MainWindow::on_PB_ActualizarProducto_clicked()
{
    QString nombreProducto = ui->CB_Productos_2->currentText();
    QString id;
    //Obtiene el product_id de products
    QSqlQuery queryProduct;
    queryProduct.prepare("SELECT product_id FROM products WHERE product_name = ?");
    queryProduct.addBindValue(nombreProducto);
    if(queryProduct.exec() && queryProduct.next()){
        id = queryProduct.value(0).toString();
    }

    QString precXund = ui->LE_A_Precio->text();
    QString undDisp = ui->LE_A_Disponibles->text();

    QSqlQuery query2;

    if(undDisp == "" && precXund == "")
        QMessageBox::information(this, "INFO PRODUCTO", "Informacion Incompleta!");
    else if(undDisp == ""){
        QString queryString = "UPDATE products "
                              "SET unit_price = :precUnd "
                              "WHERE product_id = :prodID";

        query2.prepare(queryString);
        query2.bindValue(":precUnd", precXund);
        query2.bindValue(":prodID", id);
    }
    else if(precXund == ""){
        QString queryString = "UPDATE products "
                              "SET units_in_stock = :undDisp "
                              "WHERE product_id = :prodID";

        query2.prepare(queryString);
        query2.bindValue(":undDisp", undDisp);
        query2.bindValue(":prodID", id);
    }
    else {
        QString queryString = "UPDATE products "
                              "SET units_in_stock = :undDisp, unit_price = :precUnd "
                              "WHERE product_id = :prodID";

        query2.prepare(queryString);
        query2.bindValue(":undDisp", undDisp);
        query2.bindValue(":precUnd", precXund);
        query2.bindValue(":prodID", id);
    }

    if(!query2.exec()) {
        QMessageBox::information(this, "INFO PRODUCTO", "El producto NO pudo ser actualizado exitosamente");
        qDebug() << "Error: " << query2.lastError().text();
    } else {
        QMessageBox::information(this, "INFO PRODUCTO", "El producto fue actualizado exitosamente");
        ui->LE_A_Precio->clear();
        ui->LE_A_Disponibles->clear();
    }
    QTableWidget* resultados = ui->TV_Productos;
    mostrarProductos(resultados);
}

void MainWindow::mostrarProductos(QTableWidget *resultados) {
    QSqlQuery query;
    query.prepare("SELECT product_id, product_name, unit_price, units_in_stock, discontinued FROM products WHERE discontinued = 0 order by product_id asc");
    if (!query.exec()) {
        qDebug() << query.lastError().text();
        return;
    }
    resultados->clear();
    resultados->setRowCount(0);
    resultados->setColumnCount(5);
    resultados->setHorizontalHeaderLabels({"ID Producto", "Nombre", "Precio", "Inventario", "Descontinuado"});
    int row = 0;
    while (query.next()) {
        resultados->insertRow(row);
        resultados->setItem(row, 0, new QTableWidgetItem(QString::number(query.value(0).toInt())));
        resultados->setItem(row, 1, new QTableWidgetItem(query.value(1).toString()));
        resultados->setItem(row, 2, new QTableWidgetItem(QString::number(query.value(2).toFloat())));
        resultados->setItem(row, 3, new QTableWidgetItem(QString::number(query.value(3).toInt())));
        resultados->setItem(row, 4, new QTableWidgetItem(QString::number(query.value(4).toInt())));
        row++;
    }
    resultados->resizeColumnsToContents();
    resultados->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

void MainWindow::descontinuarProducto(QTableWidget* resultados, QString& id_producto){
    QSqlQuery query;
    query.prepare("UPDATE products SET discontinued = 1 WHERE product_id = ?");
    query.addBindValue(id_producto);
    if (!query.exec()) {
        qDebug() << query.lastError().text();
        return;
    }
    mostrarProductos(resultados);
}

void MainWindow::on_PB_Descontinuar_clicked()
{
    QString nombreProducto = ui->CB_Productos_3->currentText();
    QString id;
    //Obtiene el product_id de products
    QSqlQuery queryProduct;
    queryProduct.prepare("SELECT product_id FROM products WHERE product_name = ?");
    queryProduct.addBindValue(nombreProducto);
    if(queryProduct.exec() && queryProduct.next()){
        id = queryProduct.value(0).toString();
    }

    QTableWidget* resultados = ui->TV_Productos;
    descontinuarProducto(resultados, id);
}

void MainWindow::on_tabWidget_currentChanged(int index)
{
    if(index == 1){
        QTableWidget * resultados = ui->TV_Productos;
        mostrarProductos(resultados);
    } else if (index == 2){
        QSqlQueryModel *queryModelCliente = new QSqlQueryModel();
        queryModelCliente->setQuery("SELECT customers.customer_id, extract(year from orders.order_date) AS Año, "
                                     "extract(month from orders.order_date) AS Mes, "
                                     "SUM(order_details.quantity * order_details.unit_price) AS Total "
                                     "FROM customers JOIN orders ON customers.customer_id = orders.customer_id "
                                     "JOIN order_details ON orders.order_id = order_details.order_id "
                                     "GROUP BY customers.customer_id, extract(year from orders.order_date), extract(month from orders.order_date) "
                                     "ORDER BY customers.customer_id, extract(year from orders.order_date), extract(month from orders.order_date)");
        ui->TB_ReporteClientes->setModel(queryModelCliente);
        ui->TB_ReporteClientes->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        QSqlQueryModel *queryModelProveedor = new QSqlQueryModel();
        queryModelProveedor->setQuery("SELECT s.company_name, EXTRACT(YEAR FROM o.order_date) AS Año, SUM(od.quantity) AS Productos_Vendidos "
                             "FROM suppliers s "
                             "JOIN products p ON s.supplier_id = p.supplier_id "
                             "JOIN order_details od ON p.product_id = od.product_id "
                             "JOIN orders o ON od.order_id = o.order_id "
                             "GROUP BY s.company_name, EXTRACT(YEAR FROM o.order_date) "
                             "ORDER BY s.company_name, EXTRACT(YEAR FROM o.order_date)");
        ui->TB_ReporteProveedores->setModel(queryModelProveedor);
        ui->TB_ReporteProveedores->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        QSqlQueryModel *queryModelCategoria = new QSqlQueryModel();
        queryModelCategoria->setQuery("SELECT c.category_name Nombre_Categoria, COUNT(p.product_id) AS Productos_Totales, SUM(od.quantity) AS Productos_Vendidos "
                                     "FROM categories c "
                                     "JOIN products p ON c.category_id = p.category_id "
                                     "JOIN order_details od ON p.product_id = od.product_id "
                                     "GROUP BY c.category_name "
                                     "ORDER BY c.category_name");
        ui->TB_ReeporteCategorias->setModel(queryModelCategoria);
        ui->TB_ReeporteCategorias->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    } else if (index == 3) {
        mostrarEmpleados();
    }
}

void MainWindow::mostrarEmpleados() {
    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery("SELECT e.employee_id, e.title_of_courtesy, concat(e.first_name, ' ', e.last_name) AS Nombre, concat(m.first_name, ' ', m.last_name) AS Manager "
                    "FROM Employees e "
                    "LEFT JOIN Employees m ON e.reports_to = m.employee_id"
                    "ORDER BY employee_id ASC");
    ui->TV_Empleados->setModel(model);
    ui->TV_Empleados->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

int MainWindow::generarIDEmpleado() {
    QSqlQuery query;
    query.prepare("SELECT MAX(employee_id) FROM employees");
    query.exec();
    query.first();
    int employee_id = query.value(0).toInt();
    return employee_id + 1;
}

void MainWindow::on_PB_CrearEmpleado_clicked()
{
    int employeeID = generarIDEmpleado();
    QString nombre = ui->LE_nombreEmpleado->text();
    QStringList nombreSeparado = nombre.split(' ');
    QString titulo = ui->LE_tituloEmpleado->text();
    QString tituloCortesia = ui->CB_Cortesia->currentText();
    QDate fechaNacimiento = ui->DE_fechaNacimiento->date();
    QDate fechaContratado = ui->DE_fechaContratado->date();
    QString direccion = ui->LE_Direccion->text();
    QString ciudad = ui->LE_Ciudad->text();
    QString region = ui->LE_Region->text();
    QString codigoPostal = ui->LE_codigoPostal->text();
    QString pais = ui->LE_PaisE->text();
    QString telefono = ui->LE_Telefono->text();
    QString extension = ui->LE_Extension->text();
    QString notas = ui->LE_Notas->text();

    //Obtiene los bytes de la imagen
    QFile imageFile(imagePath);
    if (!imageFile.open(QIODevice::ReadOnly)) {
        qDebug() << "Error al abrir el archivo:" << imageFile.errorString();
    }
    QByteArray imageData = imageFile.readAll();
    imageFile.close();

    QString nombreManager = ui->CB_Empleado_2->currentText();
    QStringList nombreManagerSeparado = nombreManager.split(' ');
    //Obtiene el manager del empleado
    QString reportsTo;
    QSqlQuery queryEmpleado;
    queryEmpleado.prepare("SELECT employee_id FROM employees WHERE first_name = ? AND last_name = ?");
    queryEmpleado.addBindValue(nombreManagerSeparado.first());
    queryEmpleado.addBindValue(nombreManagerSeparado.last());
    if(queryEmpleado.exec() && queryEmpleado.next()) {
        reportsTo = queryEmpleado.value(0).toString();
    }

    if(imagePath != "") {
        QSqlQuery query;
        query.prepare("INSERT INTO employees (employee_id, last_name, first_name, title, title_of_courtesy, birth_date, hire_date, address, city, region, postal_code, country, home_phone, extension, photo, notes, reports_to, photo_path) "
                      "VALUES (:employee_id, :last_name, :first_name, :title, :title_of_courtesy, :birth_date, :hire_date, :address, :city, :region, :postal_code, :country, :home_phone, :extension, :photo, :notes, :reports_to, :image_path)");
        query.bindValue(":employee_id", employeeID);
        query.bindValue(":last_name", nombreSeparado.last());
        query.bindValue(":first_name", nombreSeparado.first());
        query.bindValue(":title", titulo);
        query.bindValue(":title_of_courtesy", tituloCortesia);
        query.bindValue(":birth_date", fechaNacimiento.toString("yyyy-MM-dd"));
        query.bindValue(":hire_date", fechaContratado.toString("yyyy-MM-dd"));
        query.bindValue(":address", direccion);
        query.bindValue(":city", ciudad);
        query.bindValue(":region", region);
        query.bindValue(":postal_code", codigoPostal);
        query.bindValue(":country", pais);
        query.bindValue(":home_phone", telefono);
        query.bindValue(":extension", extension);
        query.bindValue(":photo", imageData);
        query.bindValue(":notes", notas);
        query.bindValue(":reports_to", reportsTo.toInt());
        query.bindValue(":image_path", imagePath);
        if (!query.exec()) {
            qDebug() << "Error al ejecutar la consulta:" << query.lastError().text();
        } else {
            ui->LE_nombreEmpleado->clear();
            ui->LE_tituloEmpleado->clear();
            ui->LE_Direccion->clear();
            ui->LE_Ciudad->clear();
            ui->LE_Region->clear();
            ui->LE_codigoPostal->clear();
            ui->LE_PaisE->clear();
            ui->LE_Telefono->clear();
            ui->LE_Extension->clear();
            ui->LE_Notas->clear();
            imagePath = "";
            cargarEmpleados();
            mostrarEmpleados();
        }
    }
}


void MainWindow::on_pushButton_clicked()
{
    imagePath = QFileDialog::getOpenFileName(this, "Seleccionar imagen", "/", "Archivos de imagen (*.png *.jpg *.bmp)");
    if (imagePath.isEmpty()) {
        imagePath = "";
    }
}


void MainWindow::on_pushButton_2_clicked()
{
    QString nombre = ui->CB_Empleado_3->currentText();
    QStringList nombreSeparado = nombre.split(' ');
    //Obtiene el id del empleado
    QString idEmpleado;
    QSqlQuery queryEmpleado;
    queryEmpleado.prepare("SELECT employee_id FROM employees WHERE first_name = ? AND last_name = ?");
    queryEmpleado.addBindValue(nombreSeparado.first());
    queryEmpleado.addBindValue(nombreSeparado.last());
    if(queryEmpleado.exec() && queryEmpleado.next()) {
        idEmpleado = queryEmpleado.value(0).toString();
    }

    QString photoPath;
    QSqlQuery query;
    query.prepare("SELECT photo_path FROM employees WHERE employee_id = ?");
    query.addBindValue(idEmpleado.toInt());
    if (query.exec() && query.next()) {
        photoPath = query.value(0).toString();
    }

    imagePath = QFileDialog::getOpenFileName(this, "Seleccionar imagen", "/", "Archivos de imagen (*.png *.jpg *.bmp)");
    if (imagePath.isEmpty()) {
        imagePath = photoPath;
    }
}


void MainWindow::on_pushButton_3_clicked()
{
    QString nombre = ui->CB_Empleado_3->currentText();
    QStringList nombreSeparado = nombre.split(' ');
    //Obtiene el id del empleado
    QString idEmpleado;
    QSqlQuery queryEmpleado;
    queryEmpleado.prepare("SELECT employee_id FROM employees WHERE first_name = ? AND last_name = ?");
    queryEmpleado.addBindValue(nombreSeparado.first());
    queryEmpleado.addBindValue(nombreSeparado.last());
    if(queryEmpleado.exec() && queryEmpleado.next()) {
        idEmpleado = queryEmpleado.value(0).toString();
    }

    QString direccion = ui->LE_A_Direccion->text();
    QString ciudad = ui->LE_A_Ciudad->text();
    QString region = ui->LE_A_Region->text();
    QString codigoPostal = ui->LE_A_codigoPostal->text();
    QString pais = ui->LE_A_PaisE->text();
    QString telefono = ui->LE_A_Telefono->text();
    QString extension = ui->LE_A_Extension->text();

    //Obtiene los bytes de la imagen
    QFile imageFile(imagePath);
    if (!imageFile.open(QIODevice::ReadOnly)) {
        qDebug() << "Error al abrir el archivo:" << imageFile.errorString();
    }
    QByteArray imageData = imageFile.readAll();
    imageFile.close();

    QString nombreManager = ui->CB_Empleado_2->currentText();
    QStringList nombreManagerSeparado = nombreManager.split(' ');
    //Obtiene el manager del empleado
    QString reportsTo;
    QSqlQuery queryManager;
    queryManager.prepare("SELECT employee_id FROM employees WHERE first_name = ? AND last_name = ?");
    queryManager.addBindValue(nombreManagerSeparado.first());
    queryManager.addBindValue(nombreManagerSeparado.last());
    if(queryManager.exec() && queryManager.next()) {
        reportsTo = queryManager.value(0).toString();
    }

    QSqlQuery query;
    query.prepare("UPDATE employees SET address = ?, city = ?, region = ?, postal_code = ?, country = ?, home_phone = ?, extension = ?, photo = ?, reports_to = ?, photo_path = ? WHERE employee_id = ?");
    query.addBindValue(direccion);
    query.addBindValue(ciudad);
    query.addBindValue(region);
    query.addBindValue(codigoPostal);
    query.addBindValue(pais);
    query.addBindValue(telefono);
    query.addBindValue(extension);
    query.addBindValue(imageData);
    query.addBindValue(reportsTo.toInt());
    query.addBindValue(imagePath);
    query.addBindValue(idEmpleado);
    if (!query.exec()) {
        qDebug() << "Error al ejecutar la consulta:" << query.lastError().text();
    } else {
        ui->LE_A_Direccion->clear();
        ui->LE_A_Ciudad->clear();
        ui->LE_A_Region->clear();
        ui->LE_A_codigoPostal->clear();
        ui->LE_A_PaisE->clear();
        ui->LE_A_Telefono->clear();
        ui->LE_A_Extension->clear();
    }
}

void MainWindow::mostrarClientes(){
    QSqlQueryModel *modelo = new QSqlQueryModel();
    modelo->setQuery("select customer_id as ID_Cliente, company_name as Compañia, contact_name as Contacto, contact_title as Titulo_Contacto, "
                     "address as Direccion, city as Ciudad, region as Region, postal_code as Codigo_Postal, phone as Telefono, fax as Fax "
                     "from customers "
                     "order by customer_id asc");
    ui->TV_Clientes->setModel(modelo);
    ui->TV_Clientes->resizeColumnsToContents();
    ui->TV_Clientes->columnViewportPosition(0);
    //ui->TV_Clientes->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

QString MainWindow::generarIDCliente(QString nomComp){
    QString res;
    int j = 0, i = 0;
    while(j < 5 && i < nomComp.length()){
        if(!nomComp[j].isSpace()){
            res += nomComp[j];
            j++;
        }
        i++;
    }

    while(res.length() < 5){
        int charAscii = rand() % 26 + 65;
        res += (char)charAscii;
    }
    return res;
}

void MainWindow::on_CL_PB_CrearCliente_clicked()
{
    QString nomCompañia = ui->CL_LE_NomEmpresa->text();
    QString nomContacto = ui->CL_LE_NomContacto->text();
    QString titContacto = ui->CL_CB_TituloContacto->currentText();
    QString direccion = ui->CL_LE_Direccion->text();
    QString idCliente = generarIDCliente(nomCompañia);
    QString codPostal = ui->CL_LE_CodPostal->text();
    QString telefono = ui->CL_LE_Telefono->text();
    QString ciudad = ui->CL_LE_Ciudad->text();
    QString region = ui->CL_LE_Region->text();
    QString pais = ui->CL_LE_Pais->text();
    QString fax = ui->CL_LE_Fax->text();

    QSqlQuery query;
    QString queryString = "INSERT INTO customers (customer_id, company_name, contact_name, contact_title, address, city, region, postal_code, country, phone, fax) "
                          "VALUES (:id, :comNam, :contNam, :contTitle, :addss, :city, :region, :posCode, :country, :phone, :fax)";
    query.prepare(queryString);
    query.bindValue(":id", idCliente);
    query.bindValue(":comNam", nomCompañia);
    query.bindValue(":contNam", nomContacto);
    query.bindValue(":contTitle", titContacto);
    query.bindValue(":addss", direccion);
    query.bindValue(":city", ciudad);
    query.bindValue(":region", region);
    query.bindValue(":posCode", codPostal);
    query.bindValue(":country", pais);
    query.bindValue(":phone", telefono);
    query.bindValue(":fax", fax);

    if(!query.exec()){
        QMessageBox::information(this, "ERROR CLIENTE", "El cliente no pudo ser creado :(");
        qDebug() << "Error: " << query.lastError().text();
    } else {
        mostrarClientes();
        ui->CL_LE_NomEmpresa->clear();
        ui->CL_LE_NomContacto->clear();
        ui->CL_LE_Direccion->clear();
        ui->CL_LE_CodPostal->clear();
        ui->CL_LE_Telefono->clear();
        ui->CL_LE_Ciudad->clear();
        ui->CL_LE_Region->clear();
        ui->CL_LE_Pais->clear();
        ui->CL_LE_Fax->clear();
        QMessageBox::information(this, "INFO CLIENTE", "El cliente fue creado exitosamente :D");
    }
}


void MainWindow::on_CL_CB_CamposAModificar_currentIndexChanged(int index)
{

    if(index == 3){
        ui->CL_TL_Dato_a_Modificar->setHidden(true);
        ui->CL_LE_Modificacion->setHidden(true);
        ui->CL_CB_TitContacto2->setHidden(false);
        ui->CL_TL_ModTitulo->setHidden(false);
    } else{
        ui->CL_TL_Dato_a_Modificar->setHidden(false);
        ui->CL_LE_Modificacion->setHidden(false);
        ui->CL_CB_TitContacto2->setHidden(true);
        ui->CL_TL_ModTitulo->setHidden(true);
    }

    switch(index){
    case 0:
        ui->CL_TL_Dato_a_Modificar->setText("Ingrese el ID");
        this->campo = "customer_id";
        break;
    case 1:
        ui->CL_TL_Dato_a_Modificar->setText("Ingrese Nombre de Compañia");
        this->campo = "company_name";
        break;
    case 2:
        ui->CL_TL_Dato_a_Modificar->setText("Ingrese Nombre de Contacto");
        this->campo = "contact_name";
        break;
    case 4:
        ui->CL_TL_Dato_a_Modificar->setText("Ingrese la Direccion");
        this->campo = "address";
        break;
    case 5:
        ui->CL_TL_Dato_a_Modificar->setText("Ingrese la Ciudad");
        this->campo = "city";
        break;
    case 6:
        ui->CL_TL_Dato_a_Modificar->setText("Ingrese la Region");
        this->campo = "region";
        break;
    case 7:
        ui->CL_TL_Dato_a_Modificar->setText("Ingrese el Codigo Postal");
        this->campo = "postal_code";
        break;
    case 8:
        ui->CL_TL_Dato_a_Modificar->setText("Ingrese el Telefono");
        this->campo = "phone";
        break;
    case 9:
        ui->CL_TL_Dato_a_Modificar->setText("Ingrese el Fax");
        this->campo = "fax";
        break;
    }

}


void MainWindow::on_CL_PB_Actualizat_clicked()
{
    QSqlQuery query;
    QString dato = ui->CL_LE_Modificacion->text();
    QString id = ui->CL_LE_IDCliente->text();
    QString queryString = "UPDATE customers "
                          "SET :campo = ' :dato ' "
                          "WHERE customer_id = ' :id '";

    query.prepare(queryString);
    query.bindValue(":campo", this->campo);
    query.bindValue(":dato", dato);
    query.bindValue(":id", id);

    if (!query.exec()) {
        QMessageBox::information(this, "ERROR CLIENTE", "El cliente no pudo ser Actualizado :(");
        qDebug() << "Error al ejecutar la consulta:" << query.lastError().text();
    } else{
        QMessageBox::information(this, "INFO CLIENTE", "El cliente fue Actualizado Exitosamente :D");
        ui->CL_LE_IDCliente->clear();
        ui->CL_LE_Modificacion->clear();
    }

    cout << endl << campo.toStdString() << endl;
    cout << endl << dato.toStdString() << endl;
    cout << endl << id.toStdString() << endl;
}

