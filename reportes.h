#ifndef REPORTES_H
#define REPORTES_H

#include <QDialog>

namespace Ui {
class Reportes;
}

class Reportes : public QDialog
{
    Q_OBJECT

public:
    explicit Reportes(QWidget *parent = nullptr);
    ~Reportes();

    void openNewWindow();

private:
    Ui::Reportes *ui;
};

#endif // REPORTES_H
