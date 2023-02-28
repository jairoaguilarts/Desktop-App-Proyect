#ifndef ORDERDETAILS_H
#define ORDERDETAILS_H

#include<iostream>
using namespace std;

class OrderDetails
{
private:
    int order_id;
    int product_id;
    double unit_price;
    int quantity;
    double discount;

public:
    OrderDetails(int, int, double, int, double);

    int getOrderId() const;
    void setOrderId(int);

    int getProductId() const;
    void setProductId(int);

    double getUnitPrice() const;
    void setUnitPrice(double);

    int getQuantity() const;
    void setQuantity(int);

    double getDiscount() const;
    void setDiscount(double);
};

#endif // ORDERDETAILS_H

