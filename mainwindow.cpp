#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "posfixa.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_convertButton_clicked()
{
    QString text = ui->inputLineEdit->text();
    //qInfo("%s", text.toStdString().c_str());

    Posfixa pos(text.toStdString());

    if (pos.algorithm1()){
        if (pos.algorithm2()){
            QString posfixa = QString::fromStdString(pos.get_posfixa());
            ui->outputLabel->setText(posfixa);
            return;
        }
    }

    ui->outputLabel->setText("Expressão invalida!!");
}

void MainWindow::on_clearButton_clicked()
{
    ui->inputLineEdit->setText("");

}
