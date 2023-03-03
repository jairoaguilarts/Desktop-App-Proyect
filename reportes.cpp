#include "reportes.h"
#include "ui_reportes.h"

Reportes::Reportes(QWidget *parent) :
    QDialog(parent),
    uiReporte(new Ui::Reportes)
{

    uiReporte->setupUi(this);
}

Reportes::~Reportes()
{
    delete uiReporte;
}

void Reportes::on_PB_Ok_clicked()
{
    this->hide();
}

void Reportes::setCliente(bool flag)
{
    this->cliente = flag;
}

void Reportes::setProveedor(bool flag)
{
    this->proveedor = flag;
}

void Reportes::setCategoria(bool flag)
{
    this->categoria = flag;
}

void Reportes::on_pushButton_clicked()
{
    QString id = uiReporte->LE_ID->text();
    /*
     *     QSqlQuery query2;
    query2.prepare("SELECT order_details.product_id, products.product_name, order_details.unit_price, order_details.quantity, order_details.discount "
                  "FROM order_details "
                  "JOIN products ON order_details.product_id = products.product_id "
                  "WHERE order_details.order_id = :order_id");
    query2.bindValue(":order_id", orderID_flag);
     */
    if(cliente){
        QSqlQueryModel *queryModel = new QSqlQueryModel();
        queryModel->setQuery(QString("SELECT extract(year from orders.order_date) AS Year, "
                                     "extract(month from orders.order_date) AS Month, "
                                     "SUM(order_details.quantity * order_details.unit_price) AS TotalPaid "
                                     "FROM customers JOIN orders ON customers.customer_id = orders.customer_id "
                                     "JOIN order_details ON orders.order_id = order_details.order_id "
                                     "WHERE customers.customer_id = '%1' "
                                     "GROUP BY extract(year from orders.order_date), extract(month from orders.order_date) "
                                     "ORDER BY extract(year from orders.order_date), extract(month from orders.order_date)").arg(id));
        uiReporte->TV_Reporte->setModel(queryModel);
    } else if(proveedor){

    } else if(categoria){

    }
}
