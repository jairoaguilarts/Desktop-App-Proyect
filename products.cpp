#include "products.h"


Products::Products(int id, string nom, int suppID, int catID, string cantUnd,
                   double precioUnd, int undDisp, int nivRe, bool desc):
    id(id), nom_producto(nom), supp_id(suppID), cat_id(catID), cant_x_unidad(cantUnd),
    precio_unidad(precioUnd), unidad_disp(undDisp), nivel_reorden(nivRe), descontinuado(desc){}

int Products::getId() {
    return id;
}

void Products::setId(int newId){
    this->id = newId;
}

string Products::getNom_producto(){
    return nom_producto;
}

void Products::setNom_producto(string newNom_producto) {
    this->nom_producto = newNom_producto;
}

int Products::getSupp_id(){
    return supp_id;
}

void Products::setSupp_id(int newSupp_id) {
    this->supp_id = newSupp_id;
}

int Products::getCat_id() {
    return cat_id;
}

void Products::setCat_id(int newCat_id)
{
    this->cat_id = newCat_id;
}

string Products::getCant_x_unidad() {
    return cant_x_unidad;
}

void Products::setCant_x_unidad(string newCant_x_unidad) {
    this->cant_x_unidad = newCant_x_unidad;
}

double Products::getPrecio_unidad() {
    return precio_unidad;
}

void Products::setPrecio_unidad(double newPrecio_unidad) {
    this->precio_unidad = newPrecio_unidad;
}

int Products::getUnidad_disp() {
    return unidad_disp;
}

void Products::setUnidad_disp(int newUnidad_disp)
{
    this->unidad_disp = newUnidad_disp;
}

int Products::getNivel_reorden() {
    return nivel_reorden;
}

void Products::setNivel_reorden(int newNivel_reorden)
{
    this->nivel_reorden = newNivel_reorden;
}

bool Products::getDiscontinuado() {
    return descontinuado;
}

void Products::setDiscontinuado(bool newDiscontinuado)
{
    this->descontinuado = newDiscontinuado;
}

