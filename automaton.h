#ifndef AUTOMATON_H
#define AUTOMATON_H

#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <iostream>

#define VOID -1

using namespace std;

typedef map<vector<int>,int> States;
typedef vector<vector<vector<int>>> Transitions;
typedef vector<string> Alphabet;
typedef vector<string> Word;


class Automaton
{
private:
    vector<int> empty;

public:
    Alphabet alphabet;
    States states;
    Transitions transitions;
    vector<int> init_state;
    vector<int> end_state;
    int num_states;
    int num_end_states;

    Automaton();
    int getSymbol(string symbol);
    int getState(vector<int> state); // send array of states
    bool isFinalState(vector<int> state); // send array of states
    static bool isSymbol(Alphabet alphabet,string symbol);

    vector<int> transitionFunction(vector<int> state, string symbol);
    //States extendTransitionFunction(int* state,Word word);

    //bool renameState(State q, int name); // send state position and new name (array)
    static Alphabet alphabetUnion(Alphabet alphabet1, Alphabet alphabet2);
    //States uneStates(States states1, States states2);

    static Automaton base(string symbol);
    static Automaton concatenation(Automaton a, Automaton b);
    //static Automaton automatonUnion(Automaton a, Automaton b);
    //static Automaton klenneClasp(State q);

    //void afd();
    //void minimizesStates();

    // Print this automaton
    void pf();
};

#endif // AUTOMATON_H
