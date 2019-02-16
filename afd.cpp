#include "afd.h"


// ############################
// Constructor with console (widget)
// ############################
Afd::Afd(Console* console)
{
    this->console = console;
}


// ############################
// Getter of states and symbols
// ############################
int Afd::getSymbol(string symbol)
{
//    Get the position of symbol in alphabet
    for (int i=0;i<alphabet.size();i++)
        if(symbol.compare(alphabet[i]) == 0)
            return i;

    return VOID;
}

int Afd::getState(int state)
{
//    Get the position of state in transitions matrix
    for (int i=0;i<states.size(); i++)
        if (states[i] == state)
            return i;

    return VOID;
}

int Afd::getState(set<int> states){
    string str = hash(states);
    if (m.find(str) != m.end())
        return m[str];
    return VOID;
}

void Afd::setAlphatet(Alphabet a){
    Alphabet alphabet;
    for (auto it: a){
        if (it.compare("&"))
            alphabet.push_back(it);
    }
    this->alphabet = alphabet;
}


// ############################
// Check ends states and initial state
// ############################
bool Afd::isInitState(int state){
    return (state == init_state) ? true : false;
}

bool Afd::isEndState(int state){
    for(auto it:ends_state)
        if (it == state) return true;

    return false;
}


// ############################
// Transition Function and Minimized Automaton
// ############################
int Afd::transitionFunction(int state, string symbol)
{
    int i = getState(state);
    int j = getSymbol(symbol);

    if (i == VOID || j == VOID){
        return VOID;
    }

    return transitions[i][j];
}


// ############################
// Functions for minimization
// ############################
//void Afd::mapping(States states){
//    m[states] = m.size();
//}


// ############################
// Print automaton
// ############################
void Afd::pf()
{
//    Print the alphabet
    console->myCout("\t|");
    for (auto it:this->alphabet){
        console->myCout(it);
        console->myCout("\t|");
    }
    console->myCout("\n");

//    Print the transitions of Afn
    for (int i=0; i < states.size(); i++){
        if (i == init_state){
            console->myCout("-> ");
            console->myCout(i);
            console->myCout("\t|");
        }
        else if (isEndState(i)){
            console->myCout("* ");
            console->myCout(i);
            console->myCout("\t|");
        }
        else{
            console->myCout(i);
            console->myCout("\t|");
        }

        for (int j=0; j < alphabet.size(); j++){
            if (transitions[i][j] == VOID)
                console->myCout("-");
            else {
                console->myCout(transitions[i][j]);
                console->myCout(",");
            }
            console->myCout("\t|");
        }
        console->myCout("\n");
    }
}


// ############################
// WORK AROUND
// ############################
string Afd::hash(set<int> s){
    string str = "";
    for (auto it:s){
        str += std::to_string(it);
    }
    return str;
}
