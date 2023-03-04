#ifndef PRODUCTOSCRUD_H
#define PRODUCTOSCRUD_H

#include <iostream>
#include "products.h"

using namespace std;

#include <QDialog>

namespace Ui {
class productosCRUD;
}

class productosCRUD : public QDialog
{
    Q_OBJECT

public:
    explicit productosCRUD(QWidget *parent = nullptr);
    ~productosCRUD();

private:
    Ui::productosCRUD *ui;
};

#endif // PRODUCTOSCRUD_H
