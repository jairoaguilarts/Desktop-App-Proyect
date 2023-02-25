#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QFileDialog>
#include <QStringList>
#include <QClipboard>
#include <QTime>
#include <QUrl>
#include <QMessageBox>
#include <iostream>
#include <vector>
#include <QSqlDatabase>
#include <QSqlQuery>

#define HOST_NAME "localhost"
#define USER_NAME "postgres"
#define PASSWORD "12345"
#define DATABASE_NAME "Northwind"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_PB_CrearOrden_clicked();

private:
    Ui::MainWindow *ui;
    QSqlDatabase database;

    int connectPSQL();
};
#endif // MAINWINDOW_H
