#include "automaton.h"


bool my_comp(string a, string b) { return (a.compare(b) < 0); }

//Automaton::Automaton(Automaton a)
//{
//    alphabet = a.alphabet;
//    states = a.states;
//    transitions = a.transitions;
//    init_state = a.init_state;
//    end_states = a.end_states;
//    num_end_states = a.num_end_states;
//    num_states = a.num_states;
//}

Automaton::Automaton(){}


Automaton Automaton::base(string symbol)
{
    // Create new base automaton
    vector<int> x1(1,0), x2(1,1);

    // Initialize the base automaton
    Automaton base;
    base.alphabet.push_back(symbol);
    base.num_states = 2;
    base.states[x1] = 0;
    base.states[x2] = 1;

    // Create transitions of state x1
    vector<vector<int>> temp1;
    temp1.push_back(x2);

    // Create transitions of state x2
    vector<vector<int>> temp2;
    temp2.push_back(base.empty);

    // Add transitions in 'transitions'
    base.transitions.push_back(temp1);
    base.transitions.push_back(temp2);

//    Set initial state
    base.init_state = x1;

    // Set final state and number of final states
    base.num_end_states = 1;
    base.end_state = x2;

    return base;
}

Automaton Automaton::concatenation(Automaton a, Automaton b)
{
    Automaton automaton;
    automaton.alphabet = Automaton::alphabetUnion(a.alphabet, b.alphabet);
    automaton.num_states = a.num_states + b.num_states;

//    Fill states of new automaton with states of 'a' and 'b'
    for (auto i:a.states){
        automaton.states[i.first] = automaton.states.size();
    }
    for (auto i:b.states){
        vector<int> aux;
        for(auto it:i.first)
            aux.push_back(it+a.num_states);
        automaton.states[aux] = automaton.states.size();
    }

//    Initialize the transitions matrix
    for (int i=0;i<automaton.num_states;i++){
        vector<vector<int>> temp;
        for(int j=0;j<automaton.alphabet.size();j++)
            temp.push_back(automaton.empty);
        automaton.transitions.push_back(temp);
    }

//    Fill transition of new automaton with transitions of 'a' and 'b'
    for(auto i:a.states){
        for(auto j:a.alphabet){
            int x = automaton.getState(i.first);
            int y = automaton.getSymbol(j);

            vector<int> transition = a.transitionFunction(i.first, j);
            automaton.transitions[x][y] = transition;
        }
    }
    for(auto i:b.states){
        for(auto j:b.alphabet){
            vector<int> aux;
            for(auto it:i.first)
                aux.push_back(it+a.num_states);
            int x = automaton.getState(aux);
            int y = automaton.getSymbol(j);

            vector<int> transition = b.transitionFunction(i.first, j);
            aux.clear();
            for(auto it:transition)
                aux.push_back(it+a.num_states);
            automaton.transitions[x][y] = aux;
        }
    }

    int x = automaton.getState(a.end_state);
    int y = automaton.getSymbol("&");

    vector<int> aux;
    for(auto it:b.init_state)
        aux.push_back(it+a.num_states);
    automaton.transitions[x][y] = aux;

    aux.clear();
    for(auto it:b.end_state)
        aux.push_back(it+a.num_states);
    automaton.end_state = aux;

    automaton.init_state = a.init_state;

    return automaton;
}

vector<int> Automaton::transitionFunction(vector<int> state, string symbol)
{
    int i = this->getState(state);
    int j = this->getSymbol(symbol);

    if (i == VOID || j == VOID){
        return empty;
    }

    return this->transitions[i][j];
}

int Automaton::getSymbol(string symbol)
{
    for (int i=0;i<alphabet.size();i++)
        if(symbol.compare(alphabet[i]) == 0)
            return i;

    return VOID;
}

int Automaton::getState(vector<int> state)
{
    if (this->states.find(state) != this->states.end())
        return this->states[state];

    return VOID;
}

bool Automaton::isFinalState(vector<int> state)
{
    return (init_state == state) ? true : false;
}

Alphabet Automaton::alphabetUnion(Alphabet alphabet1, Alphabet alphabet2)
{
    Alphabet alphabet = alphabet1;

    for(auto it:alphabet2)
        if (!Automaton::isSymbol(alphabet, it))
            alphabet.push_back(it);

    if (!Automaton::isSymbol(alphabet, "&"))
        alphabet.push_back("&");

    return alphabet;
}

bool Automaton::isSymbol(Alphabet alphabet, string symbol)
{
    for (auto it:alphabet)
        if (symbol.compare(it) == 0)
            return true;
    return false;
}

void Automaton::pf()
{
    cout << "  |";
    for (auto it:this->alphabet)
        cout << it << "\t|";
    cout << endl;

    for (int i=0; i < num_states; i++){
        cout << i << " |";
        for (int j=0; j < alphabet.size(); j++){
            if (transitions[i][j].size() == 0)
                cout << "-";
            else {
                for (auto it:transitions[i][j]){
                    cout << it << ",";
                }
            }
            cout << "\t|";
        }
        cout << endl;
    }
}
