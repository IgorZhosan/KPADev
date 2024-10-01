#include "mainwindow.h"
#include "./ui_mainwindow.h"

QCheckBox* kpaCheckBox = nullptr;
QCheckBox* priemCheckBox = nullptr;
QCheckBox* AD9M2 = nullptr;
QCheckBox* broadcast = nullptr;
QCheckBox* TM = nullptr;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    kpaCheckBox = ui->KPA;
    priemCheckBox = ui->reception;
    AD9M2 = ui ->AD9M2;
    broadcast = ui ->broadcast;
    TM = ui -> TM;
}

MainWindow::~MainWindow()
{
    delete ui;
}

