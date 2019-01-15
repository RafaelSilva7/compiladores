#include "automaton.h"

Automaton::Automaton()
{
    alphabet.clear();
    states.clear();
    transitonMatrix.clear();
    initialState = -1;
    finalStates.clear();
    numStates = numFinalStates = 0;
}

Automaton Automaton::base(Symbol symbol)
{
   // Create new base automaton
   Automaton base;

   // Set alphabet and number of states
   base.alphabet.push_back(symbol);
   base.numStates = 2;

   // Create initial states
   for (int i=0; i<base.numStates; i++) base.states.push_back(i);

   // Create transitions and temps vector<Transition>
   Transition transition1, transition2;
   std::vector<Transition> temp1, temp2;

   // Create transition1
   transition1.states.push_back(1);
   transition1.numStates = 1;
   temp1.push_back(transition1);
   base.transitonMatrix.push_back(temp1);

   // Create transition2
   transition2.states.clear();
   transition2.numStates = 0;
   temp2.push_back(transition2);
   base.transitonMatrix.push_back(temp2);

   // Set initial and final states
   base.initialState = 0;
   base.numFinalStates = 1;
   base.finalStates.push_back(1);

   return base;
}

int Automaton::isSymbol(Symbol a)
{
    for (int i=0;  i < this->alphabet.size(); i++)
        if (this->alphabet[i].compare(a) == 0) return i;
    return -1;
}

int Automaton::isState(State q)
{
    for (int i=0;  i < this->states.size(); i++)
        if (this->states[i] == q) return i;
    return -1;
}

bool Automaton::isFinalState(State q)
{
    for (auto it : this->finalStates)
        if (it == q) return true;
    return false;
}

void Automaton::pf()
{
    for (int i=0; i < this->transitonMatrix.size(); i++)
    {
        printf("%d -> ", i);
        for (int j=0; j < this->transitonMatrix[i].size(); j++){
            printf("|");
            for (auto it : this->transitonMatrix[i][j].states) printf("%d,", it);
            printf("| ");
        }
        printf("\n");
    }
}

