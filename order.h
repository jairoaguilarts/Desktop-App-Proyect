#ifndef ORDER_H
#define ORDER_H

#include <iostream>
#include <string>
using namespace std;

class Order
{
private:
    int order_id;
    string customer_id;
    string employee_id;
    string order_date;
    string required_date;
    int ship_via;
    double freight;
    string ship_name;
    string ship_address;
    string ship_city;
    string ship_region;
    string ship_postal_code;
    string ship_country;

public:
    Order(int, string, string, string, string, int, double, string, string, string, string, string, string);


    int getOrderId() const;
    void setOrderId(int);

    string getCustomerId() const;
    void setCustomerId(string);

    string getEmployeeId() const;
    void setEmployeeId(string);

    string getOrderDate() const;
    void setOrderDate(string);

    string getRequiredDate() const;
    void setRequiredDate(string);

    int getShipVia() const;
    void setShipVia(int);

    double getFreight() const;
    void setFreight(double);

    string getShipName() const;
    void setShipName(string);

    string getShipAddress() const;
    void setShipAddress(string);

    string getShipCity() const;
    void setShipCity(string);

    string getShipRegion() const;
    void setShipRegion(string);

    string getShipPostalCode() const;
    void setShipPostalCode(string);

    string getShipCountry() const;
    void setShipCountry(string);

};

#endif // ORDER_H

