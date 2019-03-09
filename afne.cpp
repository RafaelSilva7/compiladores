#include "afne.h"


// ############################
// Constructo with console (widget)
// ############################
Afn::Afn(Console* console){
    this->console = console;
    this->afd = NULL;
}


// ############################
// Getters symbol and state
// ############################
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


// ############################
// Check if state is 'end state' or symbol
// ############################
bool Afn::isSymbol(Alphabet alphabet, string symbol)
{
//    Walks by alphabet and check if the symbol is have in alphabet
    for (auto it:alphabet)
        if (symbol.compare(it) == 0)
            return true;
    return false;
}

bool Afn::isFinalState(int state)
{
//    Check if  state is a 'end state'
    return (init_state == state) ? true : false;
}

bool Afn::isEndState(set<int> states){
    for (auto it:states)
        if (it == end_state) return true;
    return false;
}


// ############################
// Transition Function
// ############################
vector<int> Afn::transitionFunction(int state, string symbol)
{
    int i = this->getState(state);
    int j = this->getSymbol(symbol);

    if (i == VOID || j == VOID){
        return empty;
    }

    return this->transitions[i][j];
}


// ############################
// Functions of operations with automaton
// ############################
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
    Afn afn(a.console);
    afn.alphabet = Afn::alphabetUnion(a.alphabet, b.alphabet);
    afn.num_states = a.num_states + b.num_states;

//    Fill states of new afn with states of 'a' and 'b'
//    Get the 'a' afn states
    for (auto i:a.states){
        afn.states.push_back(i);
    }
//    Get the 'b' afn states
    for (auto i:b.states){
//        convert the states for new afn
        afn.states.push_back(i+a.num_states);
    }

//    Initialize the transitions matrix
    for (int i=0;i<afn.num_states;i++){
        vector<vector<int>> temp;
        for(int j=0;j<afn.alphabet.size();j++)
            temp.push_back(afn.empty);
        afn.transitions.push_back(temp);
    }

//    Fill transition of new afn with transitions of 'a' and 'b'
//    Get the transitions of 'a' afn
    for(auto i:a.states){
        for(auto j:a.alphabet){
            int x = afn.getState(i);
            int y = afn.getSymbol(j);

            if (x == VOID || y == VOID)
                a.console->myCout("Error:\nInvalid state!1");

            vector<int> transition = a.transitionFunction(i, j);
            afn.transitions[x][y] = transition;
        }
    }
//    Get the transitions of 'b' afn
    for(auto i:b.states){
        for(auto j:b.alphabet){
//            convert the states for new afn
            int x = afn.getState(i+a.num_states);
            int y = afn.getSymbol(j);

            if (x == VOID || y == VOID)
                a.console->myCout("Error:\nInvalid state!2");

            vector<int> aux, transition = b.transitionFunction(i, j);
            for(auto it:transition)
                aux.push_back(it+a.num_states);

            afn.transitions[x][y] = aux;
        }
    }

//    Create the 'void movement' in between the afns
    int x = afn.getState(a.end_state);
    int y = afn.getSymbol("&");

    vector<int> aux;
    aux.push_back(b.init_state+a.num_states);
    afn.transitions[x][y] = aux;

//    Set the end state in new afn
    afn.end_state = afn.num_states-1;

//    Set initial state in new afn
    afn.init_state = 0;

    return afn;
}

Afn Afn::AfnUnion(Afn a, Afn b)
{

    Afn afn(a.console);
    afn.alphabet = Afn::alphabetUnion(a.alphabet, b.alphabet);
    afn.num_states = a.num_states + b.num_states + 2;
    afn.states.push_back(0);

//    Fill states of new afn with states of 'a' and 'b'
//    Get the 'a' afn states
    for (auto i:a.states){
        afn.states.push_back(i+1);
    }
//    Get the 'b' afn states
    for (auto i:b.states){
//        convert the states for new afn
        afn.states.push_back(i+a.num_states+1);
    }
    afn.states.push_back(afn.num_states-1);


//    Initialize the transitions matrix
    for (int i=0;i<afn.num_states;i++){
        vector<vector<int>> temp;
        for(int j=0;j<afn.alphabet.size();j++)
            temp.push_back(afn.empty);
        afn.transitions.push_back(temp);
    }


//    Fill transition of new afn with transitions of 'a' and 'b'
//    Get the transitions of 'a' afn
    for(auto i:a.states){
        for(auto j:a.alphabet){
//            convert the states for new afn
            int x = afn.getState(i+1);
            int y = afn.getSymbol(j);

            if (x == VOID || y == VOID)
                a.console->myCout("Error:\nInvalid state!3");

            vector<int> aux, transition = a.transitionFunction(i, j);
            for(auto it:transition)
                aux.push_back(it+1);
            afn.transitions[x][y] = aux;
        }
    }
//    Get the transitions of 'b' afn
    for(auto i:b.states){
        for(auto j:b.alphabet){
//            convert the states for new afn
            int x = afn.getState(i+a.num_states+1);
            int y = afn.getSymbol(j);

            vector<int> aux, transition = b.transitionFunction(i, j);
            for(auto it:transition)
                aux.push_back(it+a.num_states+1);
            afn.transitions[x][y] = aux;
        }
    }

//    Set the end state in new afn
    afn.end_state = afn.num_states-1;

//    Set initial state in new afn
    afn.init_state = 0;

//    Create the 'void movement' in between the afns
    int x = afn.init_state;
    int y = afn.getSymbol("&");
    vector<int> aux;
    aux.push_back(a.init_state+1);
    aux.push_back(b.init_state+a.num_states+1);
    afn.transitions[x][y] = aux;

    aux.clear();
    aux.push_back(afn.end_state);

    x = a.end_state+1;
    afn.transitions[x][y] = aux;

    x = b.end_state+a.num_states+1;
    afn.transitions[x][y] = aux;

    return afn;
}

Afn Afn::klenneClasp(Afn a)
{
    Afn afn(a.console);
    afn.alphabet = a.alphabet;
    if (afn.getSymbol("&") == VOID)
        afn.alphabet.push_back("&");
    afn.num_states = a.num_states + 2;

//    Set the states of new afn
    for (int i=0; i<afn.num_states;i++)
        afn.states.push_back(i);

//    Initialize the transitions matrix
    for (int i=0;i<afn.num_states;i++){
        vector<vector<int>> temp;
        for(int j=0;j<afn.alphabet.size();j++)
            temp.push_back(afn.empty);
        afn.transitions.push_back(temp);
    }

//    Fill transition of new afn with transitions of 'a' and 'b'
//    Get the transitions of 'a' afn
    for(auto i:a.states){
        for(auto j:a.alphabet){
//            convert the states for new afn
            int x = afn.getState(i+1);
            int y = afn.getSymbol(j);

            if (x == VOID || y == VOID)
                a.console->myCout("Error:\nInvalid state!4");

            vector<int> aux, transition = a.transitionFunction(i, j);
            for(auto it:transition)
                aux.push_back(it+1);
            afn.transitions[x][y] = aux;
        }
    }

//    Set the end state in new afn
    afn.end_state = afn.num_states-1;

//    Set initial state in new afn
    afn.init_state = 0;

//    Create the 'void movement' in between the afns
    int y = afn.getSymbol("&");
    vector<int> aux;
    aux.push_back(a.init_state+1);
    aux.push_back(afn.end_state);
    afn.transitions[0][y] = aux;

    int x = a.end_state+1;
    afn.transitions[x][y] = aux;

    return afn;

}


// ############################
// Get closureE of automaton's states and convert AFN-& to AFD
// ############################
set<int> Afn::_closureE(int state, States visited){

    // Check if courrent state is visited
    for (auto it: visited){
        if (state == it) {
            set<int> empty;
            return empty;
        }
    }

    // Add state to the visited
    visited.push_back(state);

    // Initialize exit vector
    set<int> exit;
    exit.insert(state);

    // Go through automaton's transition
    States aux = transitionFunction(state,"&");
    if (!aux.empty()){
        for(auto it:aux){
            set<int> clasp = _closureE(it,visited);
            exit.insert(clasp.begin(), clasp.end());
        }
    }

    return exit;
}


void Afn::closureE(){

    for (auto it:states){
        States aux;
        set<int> s = _closureE(it,aux);
        closuresE.push_back(s);
    }
}


Afd *Afn::toAfd(){
    if (this->afd != NULL) return this->afd;

    // Create to AFD
    Afd* afd = new Afd(console);
    afd->setAlphatet(alphabet);

    // Get Kleene's closures to AFN
    if (closuresE.empty()) {
        closureE();
    }

    // Mapping Kleene's closures to AFD and create this states
    afd->m[closuresE[0]] = 0;
    afd->states.push_back(0);
    if (isEndState(closuresE[0]))
        afd->ends_state.push_back(0);

    // Allocate transitions
    vector<int> aux1(afd->alphabet.size(), VOID);
    afd->transitions.push_back(aux1);

    // Convert transitions of AFN-E by AFD
    for (int i = 0; i < afd->states.size(); i++) {
        set<int> current = afd->getKey(i);
        for (int j=0;j<afd->alphabet.size();j++){
            // Get set of transitions of state
            set<int> new_state;
            for (auto it:current){
                States aux = transitionFunction(it,afd->alphabet[j]);

                // Union Kleene's closures
                if (!aux.empty()){
                    int x = aux[0];
                    new_state.insert(closuresE[x].begin(), closuresE[x].end());
                }
            }

            // Check if is new state
            if (new_state.empty())
                new_state = {-1};

//            str = hash(new_state);
            if (afd->m.find(new_state) == afd->m.end()){
                afd->m[new_state] = afd->m.size();
                vector<int> n(afd->alphabet.size(), VOID);
                afd->transitions.push_back(n);
                afd->states.push_back(afd->m.size()-1);
            }

            // Check if is an end state
            if (isEndState(new_state))
                afd->ends_state.push_back(afd->m[new_state]);

            afd->transitions[i][j] = afd->m[new_state];
        }
    };

    // Set the initial state
    afd->init_state = 0;

    return afd;
}


// ############################
// Print this Afn and Kleene's closure
// ############################
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
        }
        if (i == end_state){
            console->myCout("* ");
        }

        console->myCout("q");
        console->myCout(i);
        console->myCout("\t|");

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

void Afn::pf_closure(){
    if (closuresE.empty())
        closureE();

    int i=0;
    for (auto it:closuresE){

        console->myCout("Fecho-& - q");
        console->myCout(i);
        console->myCout(" -> {");
        for (auto s:it){
            console->myCout(s);
            console->myCout(",");
        }
        console->myCout("}\n");
        i++;
    }
}



