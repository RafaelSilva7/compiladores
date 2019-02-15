#include "afne.h"


Afn::Afn(Console* console){
    this->console = console;
}

Afn Afn::base(string symbol, Console* console)
{
    // Create new base Afn
    vector<int> x1(1,0), x2(1,1);

    // Initialize the base Afn
    Afn base(console);
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

Afn Afn::concatenation(Afn a, Afn b)
{
    Afn Afn(a.console);
    Afn.alphabet = Afn::alphabetUnion(a.alphabet, b.alphabet);
    Afn.num_states = a.num_states + b.num_states;

//    Fill states of new Afn with states of 'a' and 'b'
//    Get the 'a' Afn states
    for (auto i:a.states){
        Afn.states.push_back(i);
    }
//    Get the 'b' Afn states
    for (auto i:b.states){
//        convert the states for new Afn
        Afn.states.push_back(i+a.num_states);
    }

//    Initialize the transitions matrix
    for (int i=0;i<Afn.num_states;i++){
        vector<vector<int>> temp;
        for(int j=0;j<Afn.alphabet.size();j++)
            temp.push_back(Afn.empty);
        Afn.transitions.push_back(temp);
    }

//    Fill transition of new Afn with transitions of 'a' and 'b'
//    Get the transitions of 'a' Afn
    for(auto i:a.states){
        for(auto j:a.alphabet){
            int x = Afn.getState(i);
            int y = Afn.getSymbol(j);

            if (x == VOID || y == VOID)
                cout << "Error:\nInvalid state!" << endl;

            vector<int> transition = a.transitionFunction(i, j);
            Afn.transitions[x][y] = transition;
        }
    }
//    Get the transitions of 'b' Afn
    for(auto i:b.states){
        for(auto j:b.alphabet){
//            convert the states for new Afn
            int x = Afn.getState(i+a.num_states);
            int y = Afn.getSymbol(j);

            if (x == VOID || y == VOID)
                cout << "Error:\nInvalid state!" << endl;

            vector<int> aux, transition = b.transitionFunction(i, j);
            for(auto it:transition)
                aux.push_back(it+a.num_states);

            Afn.transitions[x][y] = aux;
        }
    }

//    Create the 'void movement' in between the Afns
    int x = Afn.getState(a.end_state);
    int y = Afn.getSymbol("&");

    vector<int> aux;
    aux.push_back(b.init_state+a.num_states);
    Afn.transitions[x][y] = aux;

//    Set the end state in new Afn
    Afn.end_state = Afn.num_states-1;

//    Set initial state in new Afn
    Afn.init_state = 0;

    return Afn;
}

Afn Afn::AfnUnion(Afn a, Afn b)
{

    Afn Afn(a.console);
    Afn.alphabet = Afn::alphabetUnion(a.alphabet, b.alphabet);
    Afn.num_states = a.num_states + b.num_states + 2;
    Afn.states.push_back(0);

//    Fill states of new Afn with states of 'a' and 'b'
//    Get the 'a' Afn states
    for (auto i:a.states){
        Afn.states.push_back(i+1);
    }
//    Get the 'b' Afn states
    for (auto i:b.states){
//        convert the states for new Afn
        Afn.states.push_back(i+a.num_states+1);
    }
    Afn.states.push_back(Afn.num_states-1);


//    Initialize the transitions matrix
    for (int i=0;i<Afn.num_states;i++){
        vector<vector<int>> temp;
        for(int j=0;j<Afn.alphabet.size();j++)
            temp.push_back(Afn.empty);
        Afn.transitions.push_back(temp);
    }


//    Fill transition of new Afn with transitions of 'a' and 'b'
//    Get the transitions of 'a' Afn
    for(auto i:a.states){
        for(auto j:a.alphabet){
//            convert the states for new Afn
            int x = Afn.getState(i+1);
            int y = Afn.getSymbol(j);

            if (x == VOID || y == VOID)
                cout << "Error:\nInvalid state!" << endl;

            vector<int> aux, transition = a.transitionFunction(i, j);
            for(auto it:transition)
                aux.push_back(it+1);
            Afn.transitions[x][y] = aux;
        }
    }
//    Get the transitions of 'b' Afn
    for(auto i:b.states){
        for(auto j:b.alphabet){
//            convert the states for new Afn
            int x = Afn.getState(i+a.num_states+1);
            int y = Afn.getSymbol(j);

            vector<int> aux, transition = b.transitionFunction(i, j);
            for(auto it:transition)
                aux.push_back(it+a.num_states+1);
            Afn.transitions[x][y] = aux;
        }
    }

//    Set the end state in new Afn
    Afn.end_state = Afn.num_states-1;

//    Set initial state in new Afn
    Afn.init_state = 0;

//    Create the 'void movement' in between the Afns
    int x = Afn.init_state;
    int y = Afn.getSymbol("&");
    vector<int> aux;
    aux.push_back(a.init_state+1);
    aux.push_back(b.init_state+a.num_states+1);
    Afn.transitions[x][y] = aux;

    aux.clear();
    aux.push_back(Afn.end_state);

    x = a.end_state+1;
    Afn.transitions[x][y] = aux;

    x = b.end_state+a.num_states+1;
    Afn.transitions[x][y] = aux;

    return Afn;
}

Afn Afn::klenneClasp(Afn a)
{
    Afn Afn(a.console);
    Afn.alphabet = a.alphabet;
    if (Afn.getSymbol("&") == VOID)
        Afn.alphabet.push_back("&");
    Afn.num_states = a.num_states + 2;

//    Set the states of new Afn
    for (int i=0; i<Afn.num_states;i++)
        Afn.states.push_back(i);

//    Initialize the transitions matrix
    for (int i=0;i<Afn.num_states;i++){
        vector<vector<int>> temp;
        for(int j=0;j<Afn.alphabet.size();j++)
            temp.push_back(Afn.empty);
        Afn.transitions.push_back(temp);
    }

//    Fill transition of new Afn with transitions of 'a' and 'b'
//    Get the transitions of 'a' Afn
    for(auto i:a.states){
        for(auto j:a.alphabet){
//            convert the states for new Afn
            int x = Afn.getState(i+1);
            int y = Afn.getSymbol(j);

            if (x == VOID || y == VOID)
                cout << "Error:\nInvalid state!" << endl;

            vector<int> aux, transition = a.transitionFunction(i, j);
            for(auto it:transition)
                aux.push_back(it+1);
            Afn.transitions[x][y] = aux;
        }
    }

//    Set the end state in new Afn
    Afn.end_state = Afn.num_states-1;

//    Set initial state in new Afn
    Afn.init_state = 0;

//    Create the 'void movement' in between the Afns
    int y = Afn.getSymbol("&");
    vector<int> aux;
    aux.push_back(a.init_state+1);
    aux.push_back(Afn.end_state);
    Afn.transitions[0][y] = aux;

    int x = a.end_state+1;
    Afn.transitions[x][y] = aux;

    return Afn;

}

vector<int> Afn::transitionFunction(int state, string symbol)
{
    int i = this->getState(state);
    int j = this->getSymbol(symbol);

    if (i == VOID || j == VOID){
        return empty;
    }

    return this->transitions[i][j];
}

vector<int> Afn::claspE(int state, States& visited){

    // Check if courrent state is visited
    for (auto it: visited){
        if (state == it) {
            return empty;
        }
    }

    // Add state to the visited
    visited.push_back(state);

    // Initialize exit vector
    States exit;
    exit.push_back(state);

    // Go through automaton's transition
    States aux = transitionFunction(state,"&");
    if (!aux.empty()){
        for(auto it:aux){
            States clasp = claspE(it,visited);
            exit.insert(exit.end(),clasp.begin(),clasp.end());
        }
    }

    // clear visited vector
    visited.clear();

    // Return the claspE
    return exit;
}

int Afn::getSymbol(string symbol)
{
//    Get the position of symbol in alphabet
    for (int i=0;i<alphabet.size();i++)
        if(symbol.compare(alphabet[i]) == 0)
            return i;

    return VOID;
}

int Afn::getState(int state)
{
//    Get the position of state in transitions matrix
    for (int i=0;i<this->states.size(); i++)
        if (this->states[i] == state)
            return i;

    return VOID;
}

bool Afn::isFinalState(int state)
{
//    Check if  state is a 'end state'
    return (init_state == state) ? true : false;
}

Alphabet Afn::alphabetUnion(Alphabet alphabet1, Alphabet alphabet2)
{
    Alphabet alphabet = alphabet1;

    for(auto it:alphabet2)
        if (!Afn::isSymbol(alphabet, it))
            alphabet.push_back(it);

    if (!Afn::isSymbol(alphabet, "&"))
        alphabet.push_back("&");

    return alphabet;
}

bool Afn::isSymbol(Alphabet alphabet, string symbol)
{
//    Walks by alphabet and check if the symbol is have in alphabet
    for (auto it:alphabet)
        if (symbol.compare(it) == 0)
            return true;
    return false;
}

void Afn::pf()
{
//    Print the alphabet
    console->myCout("\t|");
    for (auto it:this->alphabet){
        console->myCout(it);
        console->myCout("\t|");
    }
    console->myCout("\n");

//    Print the transitions of Afn
    for (int i=0; i < num_states; i++){
        if (i == init_state){
            console->myCout("-> ");
            console->myCout(i);
            console->myCout("\t|");
        }
        else if (i == end_state){
            console->myCout("* ");
            console->myCout(i);
            console->myCout("\t|");
        }
        else{
            console->myCout(i);
            console->myCout("\t|");
        }

        for (int j=0; j < alphabet.size(); j++){
            if (transitions[i][j].size() == 0)
                console->myCout("-");
            else {
                for (auto it:transitions[i][j]){
                    console->myCout(it);
                    console->myCout(",");
                }
            }
            console->myCout("\t|");
        }
        console->myCout("\n");
    }
}

void Afn::pf_clasp(){
    States visited;
    for (auto it:states){
        States exit = claspE(it, visited);

        // Test
        console->myCout("Fecho-& - ");
        console->myCout(it);
        console->myCout(" -> {");
        for (auto i:exit){
            console->myCout(i);
            console->myCout(",");
        }
        console->myCout("}\n");
        // Test
    }
}
