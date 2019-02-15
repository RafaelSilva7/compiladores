#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "posfixa.h"
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QImage>
#include <console.h>

// Test
#include "afne.h"

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QImage logo("/home/rafael/Documents/homeworks/compiladores/images/brasãoUFT.png");

    QGraphicsScene* scene = new QGraphicsScene();
    ui->logoUFT->setScene(scene);
    QGraphicsPixmapItem* item = new QGraphicsPixmapItem(QPixmap::fromImage(logo));
    item->setScale(0.043);
    scene->addItem(item);
    ui->logoUFT->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_clearButton_clicked()
{
    ui->inputLineEdit->setText("");
    ui->outputLineEdit->setText("");
    ui->consoleTextEdit->setPlainText("");
}

void MainWindow::on_converterButton_clicked()
{
    Console* console = new Console(ui->consoleTextEdit);
//    Afn a1 = Afn::base("b", console);
//    Afn a2 = Afn::base("\\b", console);
//    Afn a3 = Afn::base("x", console);
//    Afn b1 = Afn::AfnUnion(a1,a2);
//    Afn b2 = Afn::AfnUnion(b1,a3);
//    b2 = Afn::klenneClasp(b2);
//    b2.pf();

    QString text = ui->inputLineEdit->text();

    Posfixa pos(text.toStdString(), console);

    if (pos.algorithm1()){
        QString posfixa = QString::fromStdString(pos.get_posfixa());
        ui->outputLineEdit->setText(posfixa);
        if (pos.algorithm2()){
            QString posfixa = QString::fromStdString(pos.get_posfixa());
            ui->outputLineEdit->setText(posfixa);
            return;
        }
    }
}
