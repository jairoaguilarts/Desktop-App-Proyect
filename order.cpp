#include "order.h"

Order::Order(int order_id, string customer_id, string employee_id, string order_date, string required_date,
                 int ship_via, double freight, string ship_name, string ship_address, string ship_city,
                 string ship_region, string ship_postal_code, string ship_country)
               : order_id(order_id), customer_id(customer_id), employee_id(employee_id), order_date(order_date),
                 required_date(required_date), ship_via(ship_via), freight(freight), ship_name(ship_name),
                 ship_address(ship_address), ship_city(ship_city), ship_region(ship_region),
                 ship_postal_code(ship_postal_code), ship_country(ship_country) {}

int Order::getOrderId() const {
    return order_id;
}
void Order::setOrderId(int order_id) {
    this->order_id = order_id;
}

string Order::getCustomerId() const {
    return customer_id;
}
void Order::setCustomerId(string customer_id) {
    this->customer_id = customer_id;
}

string Order::getEmployeeId() const {
    return employee_id;
}
void Order::setEmployeeId(string employee_id) {
    this->employee_id = employee_id;
}

string Order::getOrderDate() const {
    return order_date;
}
void Order::setOrderDate(string order_date) {
    this->order_date = order_date;
}

string Order::getRequiredDate() const {
    return required_date;
}
void Order::setRequiredDate(string required_date) {
    this->required_date = required_date;
}

int Order::getShipVia() const {
    return ship_via;
}
void Order::setShipVia(int ship_via) {
    this->ship_via = ship_via;
}

double Order::getFreight() const {
    return freight;
}
void Order::setFreight(double freight) {
    this->freight = freight;
}

string Order::getShipName() const {
    return ship_name;
}
void Order::setShipName(string ship_name) {
    this->ship_name = ship_name;
}

string Order::getShipAddress() const {
    return ship_address;
}
void Order::setShipAddress(string ship_address) {
    this->ship_address = ship_address;
}

string Order::getShipCity() const {
    return ship_city;
}
void Order::setShipCity(string ship_city) {
    this->ship_city = ship_city;
}

string Order::getShipRegion() const {
    return ship_region;
}
void Order::setShipRegion(string ship_region) {
    this->ship_region = ship_region;
}

string Order::getShipPostalCode() const {
    return ship_postal_code;
}
void Order::setShipPostalCode(string ship_postal_code) {
    this->ship_postal_code = ship_postal_code;
}

string Order::getShipCountry() const {
    return ship_country;
}
void Order::setShipCountry(string ship_country) {
    this->ship_country = ship_country;
}
