#include "orderdetails.h"

OrderDetails::OrderDetails(int order_id, int product_id, double unit_price, int quantity, double discount)
        : order_id(order_id), product_id(product_id), unit_price(unit_price), quantity(quantity),
          discount(discount) {}

int OrderDetails::getOrderId() const {
    return order_id;
}
void OrderDetails::setOrderId(int order_id) {
    this->order_id = order_id;
}

int OrderDetails::getProductId() const {
    return product_id;
}
void OrderDetails::setProductId(int product_id) {
    this->product_id = product_id;
}

double OrderDetails::getUnitPrice() const {
    return unit_price;
}
void OrderDetails::setUnitPrice(double unit_price) {
    this->unit_price = unit_price;
}

int OrderDetails::getQuantity() const {
    return quantity;
}
void OrderDetails::setQuantity(int quantity) {
    this->quantity = quantity;
}

double OrderDetails::getDiscount() const {
    return discount;
}
void OrderDetails::setDiscount(double discount) {
    this->discount = discount;
}
