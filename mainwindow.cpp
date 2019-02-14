#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "posfixa.h"
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QImage>

// Test
#include "automaton.h"

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

    Automaton a1 = Automaton::base("b");
    Automaton a2 = Automaton::base("\\b");
    Automaton a3 = Automaton::base("x");
    Automaton b1 = Automaton::automatonUnion(a1,a2);
    Automaton b2 = Automaton::automatonUnion(b1,a3);
    b2 = Automaton::klenneClasp(b2);
    b2.pf();

    QString text = ui->inputLineEdit->text();

    Posfixa pos(text.toStdString());

    if (pos.algorithm1()){
        if (pos.algorithm2()){
            QString posfixa = QString::fromStdString(pos.get_posfixa());
            ui->outputLineEdit->setText(posfixa);
            ui->consoleTextEdit->setPlainText("Expressão válida!");
            return;
        }
    }
    ui->consoleTextEdit->setPlainText("Expressão inválida!!");
}
