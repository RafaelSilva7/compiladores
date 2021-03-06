#include "posfixa.h"

Posfixa::Posfixa(std::string regex, Console* console)
{
    this->regex = regex;
    this->console = console;
}

bool Posfixa::algorithm1()
{
    for (auto it = this->regex.begin(); it != this->regex.end(); it++) {
        if (*it == ' ' || *it == '\t') continue;
        else if (*it == '\\')
        {
            this->posfixa.push_back(*it);
            this->posfixa.push_back(*(++it));
            this->implicityUnion(it);
        } else if (*it == '(')
        {
            this->stack.push('(');
        } else if (*it == ')')
        {
            if (this->stack.empty())
            {
                console->myCout("Error:\nFalta de abertura de parenteses.");
//                std::cout <<  << std::endl;
                return false;
            }

            while (this->stack.top() != '(')
            {
                this->posfixa.push_back(this->stack.top());
                this->stack.pop();

                if (this->stack.empty())
                {
                    console->myCout("Error:\nFalta de abertura de parenteses.");
                    std::cout << "Error:\nFalta de abertura de parenteses." << std::endl;
                    return false;
                }
            }
            this->stack.pop();
            this->implicityUnion(it);
        } else if (this->isOperand(it, false))
        {
            this->posfixa.push_back(*it);
            this->implicityUnion(it);
        } else if (this->isOperator(*it))
        {
            bool flag;
            do{
                if (*it == '*') {
                    flag = this->kleneClasp();
                    this->implicityUnion(it);
                } else if (*it == '.') flag = this->concatenation();
                else flag = this->orOp();
            } while (flag);
        } else
        {
            console->myCout("Error:\nSintaxe invalida.");
            return false;
        }
    }

    while (!this->stack.empty())
    {
        if (this->stack.top() == '(')
        {
            console->myCout("Erro:\nExcesso de parênteses de abertura.");
            return false;
        }
        this->posfixa.push_back(this->stack.top());
        this->stack.pop();
    }

    return true;
}

Afn* Posfixa::algorithm2()
{
    std::stack<Afn> stack_afne;
    Afn op1(console), op2(console);
    string s;

    for (auto it = this->posfixa.begin(); it < this->posfixa.end(); it++)
    {
        if (*it == '\\')
        {
            s = "";
            s += *it;
            s += *(++it);
            op1 = Afn::base(s, console);
            stack_afne.push(op1);
        } else if (this->isOperand(it, false))
        {
            s = "";
            s += *it;
            stack_afne.push(Afn::base(s, console));
//            this->stack.push(*it);
        } else {
            if (!stack_afne.empty())
            {
                op2 = stack_afne.top();
                stack_afne.pop();

                if (this->unitiOperator(*it))
                {
                    stack_afne.push(Afn::klenneClasp(op2));
                } else {
                    if (!stack_afne.empty())
                    {
                        op1 = stack_afne.top();
                        stack_afne.pop();

                        if (*it == '.')
                            stack_afne.push(Afn::concatenation(op1,op2));
                        else if (*it == '+')
                            stack_afne.push(Afn::AfnUnion(op1,op2));
                    } else {
                        console->myCout("Erro:\nFalta de Operandos.1");
                        return NULL;
                    }
                }
            } else {
                console->myCout("Erro:\nFalta de Operandos.2");
                return NULL;
            }
        }
    }

    if (!stack_afne.empty()){

        op1 = stack_afne.top();
        stack_afne.pop();

        if (stack_afne.empty())
        {
            Afn* afn = new Afn(console);

            afn->alphabet = op1.alphabet;
            afn->closuresE = op1.closuresE;
            afn->end_state = op1.end_state;
            afn->init_state = op1.init_state;
            afn->num_states = op1.num_states;
            afn->states = op1.states;
            afn->transitions = op1.transitions;

            return afn;
        }
    }
    console->myCout("Erro:\nFalta de Operandos.3");
    return NULL;
}

bool Posfixa::isOperator(char a){
    return (a == '*' || a == '+' || a == '.');
}

bool Posfixa::isOperand(std::string::iterator it, bool flag){
    if (flag == true && it != this->regex.begin() && *(it-1) == '\\') return true;
    return !(*it == '*' || *it == '+' || *it == '.' || *it == '(' || *it == ')');
}

bool Posfixa::implicityUnion(std::string::iterator it){
    if (it == this->regex.end()-1) return false;

    std::string::iterator next = it+1;
    if ((this->isOperand(it, true) || *it == '*' || *it == ')') && (this->isOperand(next, false) || *next == '('))
    {
        while (concatenation()) {
        }
        return true;
    }
    return false;
}

bool Posfixa::kleneClasp(){
    this->posfixa.push_back('*');
    return false;
}

bool Posfixa::concatenation(){
    if (this->stack.empty() || this->stack.top() == '(' || this->stack.top() == '+')
    {
        this->stack.push('.');
        return false;
    }

    this->posfixa.push_back(this->stack.top());
    this->stack.pop();
    return true;
}

bool Posfixa::orOp(){
    if (this->stack.empty() || this->stack.top() == '(')
    {
        this->stack.push('+');
        return false;
    }

    this->posfixa.push_back(this->stack.top());
    this->stack.pop();
    return true;
}

std::string Posfixa::get_posfixa(){
    return this->posfixa;
}

bool Posfixa::unitiOperator(char op)
{
    return (op == '*');
}


Afn* Posfixa::toAfn(string input, Console* console){
    Posfixa posfixa(input, console);
    posfixa.algorithm1();
    return posfixa.algorithm2();
}
