#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "posfixa.h"
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QImage>
#include <console.h>
#include <QSaveFile>

// Test
#include "afne.h"
#include "afd.h"
#include "lexicalanalyzer.h"

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->syntax_analisys = NULL;

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

    QString text = ui->inputLineEdit->text();

    Posfixa pos(text.toStdString(), console);

    if (pos.algorithm1()){
        QString posfixa = QString::fromStdString(pos.get_posfixa());
        ui->outputLineEdit->setText(posfixa);
        Afn* afn = pos.algorithm2();
        if (afn != NULL){
            console->myCout("AFN-&\n");
            afn->pf();
            console->myCout("\nFECHO-&\n");
            afn->pf_closure();
            console->myCout("\nAFD\n");
            Afd* afd = afn->toAfd();
            afd->pf();
            console->myCout("\nTabela de Minimização\n");
            Afd *afd_minimum = afd->toAfdMinimun();
            afd->pf_table();
            console->myCout("\nAFD Mínimo\n");
            afd_minimum->pf();
            return;
        }
    }
}

void MainWindow::on_configAnalysisButton_clicked()
{
    Console* console = new Console(ui->configOutput);
    QString text = ui->configInput->toPlainText();
    syntax_analisys = new LexicalAnalyzer(text.toStdString(), console);
    syntax_analisys->printConfig();
}

void MainWindow::on_clearConfigButton_clicked()
{
    ui->configInput->setText("");
    ui->configOutput->setPlainText("");
}

void MainWindow::on_runAnalisysButton_clicked()
{
    Console *console = new Console(ui->analisysOutput);
    if (syntax_analisys != NULL){
        syntax_analisys->switchConsole(console);
        QString input = ui->analisysInput->toPlainText();
        syntax_analisys->toRecognize(input.toStdString());
        syntax_analisys->printRecognized();
    }
}

void MainWindow::on_analisysClearButton_clicked()
{
    ui->analisysInput->setText("");
    ui->analisysOutput->setPlainText("");
}

// #################
//void MainWindow::saveFileAs(const QString &outputFileName)
//{
//    if (!outputFileName.isNull())
//    {
//        QSaveFile file(outputFileName);
//        file.open(QIODevice::WriteOnly);

//        QByteArray outputByteArray;
//        outputByteArray.append(textArea->toPlainText().toUtf8());
//        file.write(outputByteArray);
//        file.commit();

//        fileName = outputFileName;
//    }
//}

//void MainWindow::saveFileAs()
//{
//    saveFileAs(QFileDialog::getSaveFileName(this, i18n("Save File As")));
//}
