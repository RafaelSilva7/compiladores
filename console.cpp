#include "console.h"

Console::Console(QPlainTextEdit* console){
    this->console = console;
    cout = "";
}

void Console::myCout(std::string str){
    cout += str;
    QString qstr(cout.c_str());
    this->console->setPlainText(qstr);
//    console->setPlainText(this->cout);
}

void Console::myCout(int str){
    cout += std::to_string(str);
    QString qstr(cout.c_str());
    this->console->setPlainText(qstr);
}
