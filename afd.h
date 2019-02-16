#ifndef AFD_H
#define AFD_H

#include <vector>
#include <string>
#include <console.h>
#include <map>
#include <set>

using namespace std;

#define VOID -1

typedef vector<string> Alphabet;
typedef vector<int> AfdStates;
typedef vector<vector<int>> AfdTransitions;


class Afd
{
private:
    AfdStates empty;

public:
    Console* console;
    map<string, int> m;

    Alphabet alphabet;
    vector<int> states;
    AfdTransitions transitions;
    int init_state;
    vector<int> ends_state;

    // Constructor with console (widget)
    Afd(Console* console);

    // Getter of states and symbols
    int getSymbol(string symbol);
    int getState(int state);
    int getState(set<int> states);
    void setAlphatet(Alphabet a);

    // Check ends states and initial state
    bool isEndState(int state);
    bool isInitState(int state);

    // Transition Function and Minimized Automaton
    int transitionFunction(int state, string symbol);
    Afd minimizedAutomaton();

    // Functions for minimization
    void mapping(AfdStates states);

    // Print automaton
    void pf();

    // WORK AROUND
    string hash(set<int> s);
};

#endif // AFD_H
