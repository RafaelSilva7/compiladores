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
    int pos_last_token = 0;
    while (pos_last_token < ribbon.size()){
        //bool flag = true;
        int counter = 0;//
        int pos_init = pos_last_token;
        string last_token = "";
        int last_end_state = VOID;

        for (int i = 0; i < rules.size(); i++){
            counter = 0;
            ribbon_position = pos_init;
            int current_end_state = VOID;
            Afd *afd = rules[i].second;
            int current_state = afd->init_state;

            while (ribbon_position < ribbon.size()) {
                //ribbon_position = pos_init;
                string symbol = nextSymbol(afd->alphabet);
                int next_state = afd->transitionFunction(current_state, symbol);

                // The next symbol it is not of automaton's alphabet
                if (next_state != VOID){
                    current_state = next_state;
                    if (afd->isEndState(current_state)){
                        current_end_state = current_state;
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

            if (current_end_state != VOID){
                last_end_state = current_end_state;
                goBackRibbon(counter);
                if (pos_last_token < ribbon_position || pos_last_token == 0){
                    pos_last_token = ribbon_position;
                    last_token = rules[i].first;
                }
            }
        }

        if (last_end_state == VOID){
            //goBackRibbon(counter);
            //pos_init = ribbon_position;
            recognized.push_back("erro");
        } else {
            pos_init = pos_last_token;
            recognized.push_back(last_token);
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
