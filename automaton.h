#ifndef AUTOMATON_H
#define AUTOMATON_H

#include <vector>
#include <string>

#define VOID -1

typedef int State;
typedef std::vector<int> States;
typedef struct
{
    States states;
    int numStates;
} Transition;
typedef std::vector<std::vector<Transition>> TransitionMatrix;
typedef std::string Symbol;
typedef std::vector<Symbol> Alphabet;
typedef std::string Word;

class Automaton
{
private:

public:
    Alphabet alphabet;
    States states;
    TransitionMatrix transitonMatrix;
    State initialState;
    States finalStates;
    int numStates;
    int numFinalStates;

    Automaton();
    int isSymbol(Symbol a);
    int isState(State q);
    bool isFinalState(State q);

    States transitionFunction(State q, Symbol a);
    States extendTransitionFunction(State q, Word w);

    //bool renameState(State q, int name);
    Alphabet alphabetUnion(Alphabet aphabet1, Alphabet aphabet2);
    States uneStates(States states1, States states2);

    static Automaton base(Symbol symbol);
    static Automaton concatenation(Automaton a, Automaton b);
    static Automaton automatonUnion(Automaton a, Automaton b);
    static Automaton klenneClasp(State q);

    void afd();
    void minimizesStates();

    // Print this automaton
    void pf();
};

#endif // AUTOMATON_H
