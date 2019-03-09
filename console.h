#ifndef CONSOLE_H
#define CONSOLE_H

#include <sstream>
#include "ui_mainwindow.h"
#include <string>
#include <QString>

class Console
{
private:
    QPlainTextEdit* console;

public:
    std::string cout;

    Console(QPlainTextEdit* console);

    void myCout(std::string str);
    void myCout(int str);
};

#endif // CONSOLE_H
