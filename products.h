#ifndef PRODUCTS_H
#define PRODUCTS_H

#include <iostream>
using namespace std;


class Products
{
private:
    int id;
    string nom_producto;
    int supp_id;
    int cat_id;
    string cant_x_unidad;
    double precio_unidad;
    int unidad_disp;
    int nivel_reorden;
    bool descontinuado;

public:
    Products(int, string, int, int, string, double, int, int, bool);

    int getId();
    void setId(int);
    string getNom_producto();
    void setNom_producto(string);
    int getSupp_id();
    void setSupp_id(int);
    int getCat_id();
    void setCat_id(int);
    string getCant_x_unidad();
    void setCant_x_unidad(string);
    double getPrecio_unidad();
    void setPrecio_unidad(double);
    int getUnidad_disp();
    void setUnidad_disp(int);
    int getNivel_reorden();
    void setNivel_reorden(int);
    bool getDiscontinuado();
    void setDiscontinuado(bool);
};

#endif // PRODUCTS_H
