#include "posfixa.h"

Posfixa::Posfixa(std::string regex, Console* console)
{
    this->regex = regex;
    this->console = console;
}

bool Posfixa::algorithm1()
{
    for (auto it = this->regex.begin(); it != this->regex.end(); it++) {
        if (*it == '\\')
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

bool Posfixa::algorithm2()
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
//            this->stack.push(*it);
//            this->stack.push(*(++it));
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

//                // Remover!!!, depois
//                if (!stack_afne.empty() && stack_afne.top() == '\\')
//                    stack_afne.pop();

                if (this->unitiOperator(*it))
                {
                    //valor = resultado de aplicar o operador op2;
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
                        return false;
                    }
                }
            } else {
                console->myCout("Erro:\nFalta de Operandos.2");
                return false;
            }
        }
    }

    if (!stack_afne.empty()){

        op1 = stack_afne.top();
        stack_afne.pop();

        if (stack_afne.empty())
        {
            console->myCout("Expressão válida\n");
            op1.pf();
            console->myCout("\n");
            op1.pf_clasp();

            console->myCout("\n -------------- Test AFD ------------\n");
            Afd afd = op1.toAfd();
            afd.pf();

            return true;
        }
    }
    console->myCout("Erro:\nFalta de Operandos.3");
    return false;
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
