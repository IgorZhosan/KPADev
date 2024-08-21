#include "mainwindow.h"
#include "./ui_mainwindow.h"

QCheckBox* kpaCheckBox = nullptr;
QCheckBox* priemCheckBox = nullptr;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    kpaCheckBox = ui->KPA;
    priemCheckBox = ui->reception;
}

MainWindow::~MainWindow()
{
    delete ui;
}


