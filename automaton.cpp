#include "automaton.h"


Automaton::Automaton(){}

Automaton Automaton::base(string symbol)
{
    // Create new base automaton
    vector<int> x1(1,0), x2(1,1);

    // Initialize the base automaton
    Automaton base;
    base.alphabet.push_back(symbol);
    base.num_states = 2;
    base.states.push_back(0);
    base.states.push_back(1);

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
    base.init_state = 0;

    // Set final state and number of final states
    base.end_state = 1;

    return base;
}

Automaton Automaton::concatenation(Automaton a, Automaton b)
{
    Automaton automaton;
    automaton.alphabet = Automaton::alphabetUnion(a.alphabet, b.alphabet);
    automaton.num_states = a.num_states + b.num_states;

//    Fill states of new automaton with states of 'a' and 'b'
//    Get the 'a' automaton states
    for (auto i:a.states){
        automaton.states.push_back(i);
    }
//    Get the 'b' automaton states
    for (auto i:b.states){
//        convert the states for new automaton
        automaton.states.push_back(i+a.num_states);
    }

//    Initialize the transitions matrix
    for (int i=0;i<automaton.num_states;i++){
        vector<vector<int>> temp;
        for(int j=0;j<automaton.alphabet.size();j++)
            temp.push_back(automaton.empty);
        automaton.transitions.push_back(temp);
    }

//    Fill transition of new automaton with transitions of 'a' and 'b'
//    Get the transitions of 'a' automaton
    for(auto i:a.states){
        for(auto j:a.alphabet){
            int x = automaton.getState(i);
            int y = automaton.getSymbol(j);

            if (x == VOID || y == VOID)
                cout << "Error:\nInvalid state!" << endl;

            vector<int> transition = a.transitionFunction(i, j);
            automaton.transitions[x][y] = transition;
        }
    }
//    Get the transitions of 'b' automaton
    for(auto i:b.states){
        for(auto j:b.alphabet){
//            convert the states for new automaton
            int x = automaton.getState(i+a.num_states);
            int y = automaton.getSymbol(j);

            if (x == VOID || y == VOID)
                cout << "Error:\nInvalid state!" << endl;

            vector<int> aux, transition = b.transitionFunction(i, j);
            for(auto it:transition)
                aux.push_back(it+a.num_states);

            automaton.transitions[x][y] = aux;
        }
    }

//    Create the 'void movement' in between the automatons
    int x = automaton.getState(a.end_state);
    int y = automaton.getSymbol("&");

    vector<int> aux;
    aux.push_back(b.init_state+a.num_states);
    automaton.transitions[x][y] = aux;

//    Set the end state in new automaton
    automaton.end_state = automaton.num_states-1;

//    Set initial state in new automaton
    automaton.init_state = 0;

    return automaton;
}

Automaton Automaton::automatonUnion(Automaton a, Automaton b)
{

    Automaton automaton;
    automaton.alphabet = Automaton::alphabetUnion(a.alphabet, b.alphabet);
    automaton.num_states = a.num_states + b.num_states + 2;
    automaton.states.push_back(0);

//    Fill states of new automaton with states of 'a' and 'b'
//    Get the 'a' automaton states
    for (auto i:a.states){
        automaton.states.push_back(i+1);
    }
//    Get the 'b' automaton states
    for (auto i:b.states){
//        convert the states for new automaton
        automaton.states.push_back(i+a.num_states+1);
    }
    automaton.states.push_back(automaton.num_states-1);


//    Initialize the transitions matrix
    for (int i=0;i<automaton.num_states;i++){
        vector<vector<int>> temp;
        for(int j=0;j<automaton.alphabet.size();j++)
            temp.push_back(automaton.empty);
        automaton.transitions.push_back(temp);
    }


//    Fill transition of new automaton with transitions of 'a' and 'b'
//    Get the transitions of 'a' automaton
    for(auto i:a.states){
        for(auto j:a.alphabet){
//            convert the states for new automaton
            int x = automaton.getState(i+1);
            int y = automaton.getSymbol(j);

            if (x == VOID || y == VOID)
                cout << "Error:\nInvalid state!" << endl;

            vector<int> aux, transition = a.transitionFunction(i, j);
            for(auto it:transition)
                aux.push_back(it+1);
            automaton.transitions[x][y] = aux;
        }
    }
//    Get the transitions of 'b' automaton
    for(auto i:b.states){
        for(auto j:b.alphabet){
//            convert the states for new automaton
            int x = automaton.getState(i+a.num_states+1);
            int y = automaton.getSymbol(j);

            vector<int> aux, transition = b.transitionFunction(i, j);
            for(auto it:transition)
                aux.push_back(it+a.num_states+1);
            automaton.transitions[x][y] = aux;
        }
    }

//    Set the end state in new automaton
    automaton.end_state = automaton.num_states-1;

//    Set initial state in new automaton
    automaton.init_state = 0;

//    Create the 'void movement' in between the automatons
    int x = automaton.init_state;
    int y = automaton.getSymbol("&");
    vector<int> aux;
    aux.push_back(a.init_state+1);
    aux.push_back(b.init_state+a.num_states+1);
    automaton.transitions[x][y] = aux;

    aux.clear();
    aux.push_back(automaton.end_state);

    x = a.end_state+1;
    automaton.transitions[x][y] = aux;

    x = b.end_state+a.num_states+1;
    automaton.transitions[x][y] = aux;

    return automaton;
}

Automaton Automaton::klenneClasp(Automaton a)
{
    Automaton automaton;
    automaton.alphabet = a.alphabet;
    if (automaton.getSymbol("&") == VOID)
        automaton.alphabet.push_back("&");
    automaton.num_states = a.num_states + 2;

//    Set the states of new automaton
    for (int i=0; i<automaton.num_states;i++)
        automaton.states.push_back(i);

//    Initialize the transitions matrix
    for (int i=0;i<automaton.num_states;i++){
        vector<vector<int>> temp;
        for(int j=0;j<automaton.alphabet.size();j++)
            temp.push_back(automaton.empty);
        automaton.transitions.push_back(temp);
    }

//    Fill transition of new automaton with transitions of 'a' and 'b'
//    Get the transitions of 'a' automaton
    for(auto i:a.states){
        for(auto j:a.alphabet){
//            convert the states for new automaton
            int x = automaton.getState(i+1);
            int y = automaton.getSymbol(j);

            if (x == VOID || y == VOID)
                cout << "Error:\nInvalid state!" << endl;

            vector<int> aux, transition = a.transitionFunction(i, j);
            for(auto it:transition)
                aux.push_back(it+1);
            automaton.transitions[x][y] = aux;
        }
    }

//    Set the end state in new automaton
    automaton.end_state = automaton.num_states-1;

//    Set initial state in new automaton
    automaton.init_state = 0;

//    Create the 'void movement' in between the automatons
    int y = automaton.getSymbol("&");
    vector<int> aux;
    aux.push_back(a.init_state+1);
    aux.push_back(automaton.end_state);
    automaton.transitions[0][y] = aux;

    int x = a.end_state+1;
    automaton.transitions[x][y] = aux;

    return automaton;

}

vector<int> Automaton::transitionFunction(int state, string symbol)
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
//    Get the position of symbol in alphabet
    for (int i=0;i<alphabet.size();i++)
        if(symbol.compare(alphabet[i]) == 0)
            return i;

    return VOID;
}

int Automaton::getState(int state)
{
//    Get the position of state in transitions matrix
    for (int i=0;i<this->states.size(); i++)
        if (this->states[i] == state)
            return i;

    return VOID;
}

bool Automaton::isFinalState(int state)
{
//    Check if  state is a 'end state'
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
//    Walks by alphabet and check if the symbol is have in alphabet
    for (auto it:alphabet)
        if (symbol.compare(it) == 0)
            return true;
    return false;
}

void Automaton::pf()
{
//    Print the alphabet
    cout << "\t|";
    for (auto it:this->alphabet)
        cout << it << "\t|";
    cout << endl;

//    Print the transitions of automaton
    for (int i=0; i < num_states; i++){
        if (i == init_state)
            cout << "-> " << i << "\t|";
        else if (i == end_state)
            cout << "* " << i << "\t|";
        else
            cout << i << "\t|";

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
