#ifndef Afn_H
#define Afn_H

#include <qdebug.h>

#include <vector>
#include <string>
#include <console.h>
#include <afd.h>
#include <map>
#include <set>
#include <unordered_set>

#define VOID -1

using namespace std;

typedef vector<int> States;
typedef vector<vector<States>> Transitions;
typedef vector<string> Alphabet;
typedef vector<string> Word;


class Afn
{
private:
    vector<int> empty;
    Afd *afd;

    set<int> _closureE(int state, vector<int> visited);

public:
    Console* console;
    Alphabet alphabet;
    States states;
    Transitions transitions;
    int init_state;
    int end_state;
    int num_states;
    vector<set<int>> closuresE;

    // Constructo with console (widget)
    Afn(Console* console);

    // Getters symbol and state
    int getSymbol(string symbol);
    int getState(int state);

    // Check if state is 'end state' or symbol
    bool isFinalState(int state);
    bool isEndState(set<int> states);
    static bool isSymbol(Alphabet alphabet,string symbol);

    // Transition Function
    vector<int> transitionFunction(int state, string symbol);

    // Functions of operations with automaton
    static Alphabet alphabetUnion(Alphabet alphabet1, Alphabet alphabet2);  
    static Afn base(string symbol, Console* console);
    static Afn concatenation(Afn a, Afn b);
    static Afn AfnUnion(Afn a, Afn b);
    static Afn klenneClasp(Afn a);

    // Get claspE of automaton's states and convert AFN-& to AFD
    void closureE();
    bool isEqual(vector<set<int>> closures, int state);
    Afd* toAfd();

    // Print this Afn and Kleene's closure
    void pf();
    void pf_closure();

    // WORK AROUND
    string hash(set<int> s);
};

#endif // Afn_H
