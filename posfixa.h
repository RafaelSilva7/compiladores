#ifndef POSFIXA_H
#define POSFIXA_H

#include <string>
#include <stack>
#include <iostream>

#include "afne.h"
#include "console.h"
#include "afd.h"

class Posfixa
{
private:
    std::string regex;
    std::stack<char> stack;
    std::string posfixa;

    bool isOperand(std::string::iterator a, bool flag);
    bool isOperator(char a);
    bool implicityUnion(std::string::iterator it);
    bool kleneClasp();
    bool concatenation();
    bool orOp();
    bool unitiOperator(char op);
public:
    Posfixa(std::string regex, Console* console);
    Console* console;

    bool algorithm1();
    Afn *algorithm2();
    std::string get_posfixa();

    static Afn *toAfn(string input, Console* console);
};

#endif // POSFIXA_H
