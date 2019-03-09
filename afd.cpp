#include "afd.h"


// ############################
// Constructor with console (widget)
// ############################
Afd::Afd(Console* console)
{
    this->afdMinimun = NULL;
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
//    string str = hash(states);
    if (m.find(states) != m.end())
        return m[states];
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

bool Afd::isDeadState(int state)
{
    if (deadStates.empty())
        _deadStates();

    for(auto it:deadStates){
        if (it == state) return true;
    }
    return false;
}

void Afd::_deadStates(){
    deadStates.clear();
    for (auto it:states){
        bool flag = true;
        for (auto symbol:alphabet){
            if (it != transitionFunction(it, symbol)){
                flag = false;
                break;
            }
        }

        if (flag)
            deadStates.push_back(it);
    }
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
Afd* Afd::toAfdMinimun()
{
    // Crete minimization table
    for (int row=0; row < states.size(); row++){
        vector<int> aux;
        for (int col=0; col < row; col++){
            aux.push_back(VOID);
        }
        table.push_back(aux);
    }

    // Fill the minimization table
    bool flag = true;
    int count = 0;
    while (flag) {
        flag = false;
        for (int row=1; row < states.size(); row++){
            for (int col=0; col < row; col++){
                if (!count){
                    if (!isEndState(row) && isEndState(col) || isEndState(row) && !isEndState(col)){
                        table[row][col] = count;
                        flag = true;
                    }
                }
                else if (table[row][col] != VOID)
                    continue;
                else {
                    if (_isNotEqual(row, col)){
                        table[row][col] = count;
                        flag = true;
                    }
                }
            }
        }
        count++;
    }

    // Compute the new states and get number of states equals
    int num_states = _equalStates();

    // Make new transitions matrix
    vector<vector<int>> new_transitions;
    for (int i=0; i< num_states; i++){
        vector<int> temp(alphabet.size(), VOID);
        new_transitions.push_back(temp);
    }

    // Fill new transitions matrix
    for (int i=0; i< states.size(); i++){
        for (int j=0; j < alphabet.size(); j++){
            int state = transitions[i][j];
            new_transitions[equalStates[i]][j] = equalStates[state];
        }
    }

    // Make and fill new states
    vector<int> new_states;
    for (int i=0; i < num_states; i++)
        new_states.push_back(i);

    // Make and fill new end states
    vector<int> new_end_states;
    for (auto i:ends_state)
        new_end_states.push_back(equalStates[i]);

    // Creates the minimum AFD
    Afd *afd_minimum = new Afd(console);
    afd_minimum->transitions = new_transitions;
    afd_minimum->states = new_states;
    afd_minimum->ends_state = new_end_states;
    afd_minimum->init_state = 0;
    afd_minimum->alphabet = alphabet;

    return afd_minimum;
}

bool Afd::_isNotEqual(int row, int col)
{
    for (auto symbol:alphabet){
        int state1 = transitionFunction(row,symbol);
        int state2 = transitionFunction(col,symbol);

        if (state1 == state2) continue;

        if (table[state1][state2] != VOID ||
                (!isEndState(row) && isEndState(col) || isEndState(row) && !isEndState(col)))
            return true;
    }
    return false;
}

int Afd::_equalStates()
{
    equalStates.clear();
    int next = 1;

    for (int i=1; i < states.size(); i++){
        bool flag = true;
        for (int j=0; j < i; j++){
            if (table[i][j] == VOID){
                flag = false;
                equalStates[i] = equalStates[j];
            }
        }

        if (flag){
            equalStates[i] = next;
            next++;
        }
    }
    return next;
}


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
        } else
            console->myCout("   ");
        if (isEndState(i)){
            console->myCout("*");
        } else
            console->myCout(" ");

        console->myCout("q");
        console->myCout(i);
        console->myCout("\t|");

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

void Afd::pf_table()
{
    for (int row=1; row < states.size(); row++){
        console->myCout("q");
        console->myCout(row);
        console->myCout("\t");
        for (int col=0; col < row; col++){
            console->myCout("|");
            if (table[row][col] == VOID)
                console->myCout("  ");
            else
                console->myCout(table[row][col]);
            console->myCout("     ");
        }
        console->myCout("|\n");
    }
    console->myCout("\t");
    for (int i=0; i < states.size()-1; i++){
        console->myCout("q");
        console->myCout(i);
        console->myCout("    ");
    }
    console->myCout("\n");
}


// ############################
// WORK AROUND
// ############################
set<int> Afd::getKey(int i) {
    map<set<int>,int>::iterator it = this->m.begin();
    while (it != this->m.end()){
        if (it->second == i)
            return it->first;
        it++;
    }
    set<int> fail({-1});
    return fail;
}
