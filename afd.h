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
    Afd *afdMinimun;
    vector<vector<int>> table; // Minimization table
    map<int,int> equalStates;

    void _deadStates();

    // Functions for minimization
    bool _isNotEqual(int i, int j);
    int _equalStates(); // return number of states not equal

public:
    Console* console;
    map<set<int>, int> m;
    vector<int> deadStates;
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
    bool isDeadState(int state);

    // Transition Function and Minimized Automaton
    int transitionFunction(int state, string symbol);
    Afd * toAfdMinimun();

    // Print automaton
    void pf();
    void pf_table();

    // WORK AROUND
    set<int> getKey(int i);
};

#endif // AFD_H
