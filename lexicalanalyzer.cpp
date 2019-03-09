#include "lexicalanalyzer.h"

LexicalAnalyzer::LexicalAnalyzer(string input, Console* console)
{
    this->console = console;
    parseInput(input);

}


void LexicalAnalyzer::switchConsole(Console *console)
{
    this->console = console;
}


void LexicalAnalyzer::parseInput(const string input)
{
    istringstream tokenStream(input);
    string token, regex;

    while (getline(tokenStream, token, '=')) {
        token = removeSpace(token);
        getline(tokenStream,regex);

        Afd *afd = Posfixa::toAfn(regex, this->console)->toAfd()->toAfdMinimun();
        pair<string, Afd*> new_rule(token,afd);
        rules.push_back(new_rule);
    }
}

string LexicalAnalyzer::removeSpace(string str)
{
    string new_str = "";
    for (auto i:str){
        if (i != ' ' && i != '\n')
            new_str += i;
    }
    return new_str;
}

void LexicalAnalyzer::printConfig()
{
    for(auto it:rules){
        console->myCout("token = <");
        console->myCout(it.first);
//        console->myCout(">\n\n-------- AFN-& --------\n");
//        it.second->pf();
        console->myCout(">\nAFD  =\n");
        it.second->pf();
        console->myCout("\n------------------\n");
    }
}


void LexicalAnalyzer::toRecognize(string input)
{
    recognized.clear();
    ribbon = removeSpace(input);
    ribbon_position = 0;
    while (ribbon_position < ribbon.size()){
        bool flag = true;
        int counter = 0;
        for (int i = 0; i < rules.size(); i++){
            Afd *afd = rules[i].second;
            int current_state = afd->init_state;
            int last_end_state = VOID;
            counter = 0;

            while (ribbon_position < ribbon.size()) {
                string symbol = nextSymbol(afd->alphabet);
                int next_state = afd->transitionFunction(current_state, symbol);

                // The next symbol it is not of automaton's alphabet
                if (next_state != VOID){
                    current_state = next_state;
                    if (afd->isEndState(current_state)){
                        last_end_state = current_state;
                        counter = 0;
                    }
                    else if (afd->isDeadState(current_state)){
                        counter++;
                        break;
                    } else
                        counter++;
                }
                else {
                    counter++;
                    break;
                }
            }

            goBackRibbon(counter);
            if (last_end_state != VOID){
                flag = false;
                recognized.push_back(rules[i].first);
                break;
            }
        }
        if (flag) {
            goForwadRobbon(counter);
            recognized.push_back("erro");
        }
    }
}


string LexicalAnalyzer::nextSymbol(vector<string> alphabet)
{
    for (auto symbol:alphabet) {
        int j = ribbon_position;
        bool flag = true;
        for (auto i:symbol){
            if (i == '\\') continue;
            if (j >= ribbon.size() || i != ribbon[j]) {
                flag = false;
                break;
            }
            j++;
        }

        if (flag){
            ribbon_position = j;
            return symbol;
        }
    }
    ribbon_position++;
    return "";
}


void LexicalAnalyzer::goBackRibbon(int quant)
{
    ribbon_position -= quant;
}

void LexicalAnalyzer::goForwadRobbon(int quant)
{
    ribbon_position += quant;
}

void LexicalAnalyzer::printRecognized()
{
    for (auto it:recognized){
        console->myCout("<");
        console->myCout(it);
        console->myCout(">");
    }
}
