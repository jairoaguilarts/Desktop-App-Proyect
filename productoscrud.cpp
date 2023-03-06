#include "productoscrud.h"
#include "ui_productoscrud.h"

int id_Producto();

productosCRUD::productosCRUD(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::productosCRUD)
{
    database = QSqlDatabase::addDatabase("QPSQL");
    database.setHostName(HOST_NAME);
    database.setUserName(USER_NAME);
    database.setPassword(PASSWORD);
    database.setDatabaseName(DATABASE_NAME);
    database.open();
    ui->setupUi(this);
}

productosCRUD::~productosCRUD()
{
    delete ui;
}

void productosCRUD::on_PB_CrearProd_clicked()
{
    int id = id_Producto();
    QString nombre = ui->LE_C_Nom->text();
    QString supp_id = ui->LE_C_IDSupp->text();
    QString cat_id = ui->LE_C_IDCat->text();
    QString cantXund = ui->LE_C_CantxUnd->text();
    QString preXund = ui->LE_C_PrexUnd->text();
    QString und_Disp = ui->LE_C_UndDis->text();
    QString und_Ord = ui->LE_C_UndOrdenadas->text();
    QString niv_Reorden = ui->LE_C_NivRe->text();
    QString descontinuado;
    if(ui->CB_C_Desc->currentIndex() == 0)
        descontinuado = "1";
    else
        descontinuado = "0";

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
        Products *producto = new Products(id, nombre.toStdString(), supp_id.toInt(),
                                          cat_id.toInt(), cantXund.toStdString(), preXund.toDouble(),und_Disp.toInt(),
                                          und_Ord.toInt(),niv_Reorden.toInt(), descontinuado.toInt());
        ui->LE_C_Nom->clear();
        ui->LE_C_IDSupp->clear();
        ui->LE_C_IDCat->clear();
        ui->LE_C_CantxUnd->clear();
        ui->LE_C_PrexUnd->clear();
        ui->LE_C_UndDis->clear();
        ui->LE_C_UndOrdenadas->clear();
        ui->LE_C_NivRe->clear();
    }
}


void productosCRUD::on_A_PB_Actualizar_clicked()
{
    QString id = ui->A_LE_ID->text();
    QString precXund = ui->A_LE_PrexUnd->text();
    QString undDisp = ui->A_LE_UndDis->text();

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
        ui->A_LE_ID->clear();
        ui->A_LE_PrexUnd->clear();
        ui->A_LE_UndDis->clear();
    }
}

void mostrar_productos(QTableWidget *resultados){
    QSqlQuery query;
    query.prepare("SELECT product_id, product_name, unit_price, units_in_stock, discontinued FROM products WHERE discontinued = 0 order by product_id");
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

void desc_producto(QTableWidget* resultados, QString& id_producto){
    QSqlQuery query;
    query.prepare("UPDATE products SET discontinued = 1 WHERE product_id = ?");
    query.addBindValue(id_producto);
    if (!query.exec()) {
        qDebug() << query.lastError().text();
        return;
    }
    mostrar_productos(resultados);
}
void productosCRUD::on_PB_MostrarProd_clicked()
{
    QTableWidget * resultados = ui->tabla_productos;
    mostrar_productos(resultados);
}


void productosCRUD::on_PB_DescProd_clicked()
{
    QString id_producto = ui->LE_E_ID->text();
    QTableWidget* resultados = ui->tabla_productos;
    desc_producto(resultados, id_producto);
}

int id_Producto(){
    QSqlQuery query;
    query.prepare("SELECT MAX(product_id) FROM products");
    query.exec();
    query.first();
    int prod_ID = query.value(0).toInt();
    return prod_ID + 1;
}

