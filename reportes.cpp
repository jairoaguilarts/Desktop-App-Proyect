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
    QSqlQueryModel *queryModel = new QSqlQueryModel();
    if(cliente){
        queryModel->setQuery(QString("SELECT extract(year from orders.order_date) AS Year, "
                                     "extract(month from orders.order_date) AS Month, "
                                     "SUM(order_details.quantity * order_details.unit_price) AS TotalPaid "
                                     "FROM customers JOIN orders ON customers.customer_id = orders.customer_id "
                                     "JOIN order_details ON orders.order_id = order_details.order_id "
                                     "WHERE customers.customer_id = '%1' "
                                     "GROUP BY extract(year from orders.order_date), extract(month from orders.order_date) "
                                     "ORDER BY extract(year from orders.order_date), extract(month from orders.order_date)").arg(id));
    } else if(proveedor){
        queryModel->setQuery(QString("SELECT EXTRACT(YEAR FROM o.order_date) AS Year, SUM(od.quantity) AS TotalSold "
                             "FROM suppliers s "
                             "JOIN products p ON s.supplier_id = p.supplier_id "
                             "JOIN order_details od ON p.product_id = od.product_id "
                             "JOIN orders o ON od.order_id = o.order_id "
                             "WHERE s.supplier_id = %1 "
                             "GROUP BY EXTRACT(YEAR FROM o.order_date) "
                             "ORDER BY EXTRACT(YEAR FROM o.order_date)").arg(id.toInt()));
    } else if(categoria){

    }
    uiReporte->TV_Reporte->setModel(queryModel);
}
