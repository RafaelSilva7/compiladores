#ifndef AUTOMATON_H
#define AUTOMATON_H


class automaton
{
private:
    vector<char> alphabet;
    vector<int> states;
    int initialState;
    vector<int> finalStates;

public:
    automaton();
    bool isSymbol(char a);
    bool isState(char q);
    bool isFinalState(char q);

    vector<int> transitionFunction(char q, char a);
    vector<int> extendTransitionFunction(char q, int w);
    bool renameState(char q, char name);
    bool alphabetUnion(vector<char> aphabet1, vector<char> aphabet2);
    bool uneStates(vector<int> state1, vector<int> state2);

    int klenneClasp(q);
    void afd();
    void minimizesStates();
};

#endif // AUTOMATON_H
