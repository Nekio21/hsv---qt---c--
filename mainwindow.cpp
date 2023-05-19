#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <QColorDialog>

using namespace std;


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    ui->widget->hsv.h = ui->horizontalSlider->value();
     ui->widget->updateHSV();
}


void MainWindow::on_horizontalSlider_2_valueChanged(int value)
{
    ui->widget->hsv.s = value/100.0;
    ui->widget->updateHSV();
}


void MainWindow::on_horizontalSlider_3_valueChanged(int value)
{
    ui->widget->hsv.v = value/100.0;
    ui->widget->updateHSV();
}


void MainWindow::on_horizontalSlider_sliderReleased()
{
    cout<<"fff"<<endl;
}

