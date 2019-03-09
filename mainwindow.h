#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "lexicalanalyzer.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_clearButton_clicked();

    void on_converterButton_clicked();

    void on_configAnalysisButton_clicked();

    void on_clearConfigButton_clicked();

    void on_runAnalisysButton_clicked();

    void on_analisysClearButton_clicked();

// #######################
//    void saveFileAs();
//    void saveFileAs(const QString &outputFileName);

private:
    Ui::MainWindow *ui;
    LexicalAnalyzer *syntax_analisys;
};

#endif // MAINWINDOW_H
