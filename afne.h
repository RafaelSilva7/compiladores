#ifndef Afn_H
#define Afn_H

#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <iostream>
#include <console.h>

#define VOID -1

using namespace std;

typedef vector<int> States;
typedef vector<vector<vector<int>>> Transitions;
typedef vector<string> Alphabet;
typedef vector<string> Word;


class Afn
{
private:
    vector<int> empty;

public:
    Console* console;
    Alphabet alphabet;
    States states;
    Transitions transitions;
    int init_state;
    int end_state;
    int num_states;
//    int num_end_states;

    Afn(Console* console);
    int getSymbol(string symbol);
    int getState(int state); // send array of states
    bool isFinalState(int state); // send array of states
    static bool isSymbol(Alphabet alphabet,string symbol);

    vector<int> transitionFunction(int state, string symbol);
    //States extendTransitionFunction(int* state,Word word);

    //bool renameState(State q, int name); // send state position and new name (array)
    static Alphabet alphabetUnion(Alphabet alphabet1, Alphabet alphabet2);
//    States uneStates(States states1, States states2);

    static Afn base(string symbol, Console* console);
    static Afn concatenation(Afn a, Afn b);
    static Afn AfnUnion(Afn a, Afn b);
    static Afn klenneClasp(Afn a);

    vector<int> claspE(int state, vector<int>& visited);
    //void afd();
    //void minimizesStates();

    // Print this Afn
    void pf();
    void pf_clasp();
};

#endif // Afn_H
