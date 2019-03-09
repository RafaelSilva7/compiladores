#ifndef SYNTAXANALISYS_H
#define SYNTAXANALISYS_H

#include <string>
#include <vector>
#include <utility>

#include "console.h"
#include "afne.h"
#include "posfixa.h"

using namespace std;

class LexicalAnalyzer
{
public:
    Console* console;
    vector<pair<string, Afd*>> rules;
    vector<string> recognized;
    string ribbon;
    int ribbon_position;

    LexicalAnalyzer(string input, Console* console);
    void parseInput(string input);
    void printConfig();

    void switchConsole(Console *console);

    void toRecognize(string input);
    void printRecognized();

private:
    string removeSpace(string str);
    string nextSymbol(vector<string> alphabet);
    bool hasSymbol();
    void goBackRibbon(int quant);
    void goForwadRobbon(int quant);

signals:

public slots:
};

#endif // SYNTAXANALISYS_H
