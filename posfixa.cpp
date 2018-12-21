#include "posfixa.h"

Posfixa::Posfixa(std::string regex)
{
    this->regex = regex;
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
                std::cout << "Error:\nFalta de abertura de parenteses." << std::endl;
                return false;
            }

            while (this->stack.top() != '(')
            {
                this->posfixa.push_back(this->stack.top());
                this->stack.pop();

                if (this->stack.empty())
                {
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
            std::cout << "Error:\nSintaxe invalida." << std::endl;
            return false;
        }
    }

    while (!this->stack.empty())
    {
        if (this->stack.top() == '(')
        {
            std::cout << "Erro:\nExcesso de parênteses de abertura." << std::endl;
            return false;
        }
        this->posfixa.push_back(this->stack.top());
        this->stack.pop();
    }

    return true;
}

bool Posfixa::algorithm2()
{
    char op1, op2;
    for (auto it = this->posfixa.begin(); it < this->posfixa.end(); it++)
    {
        if (*it == '\\')
        {
            this->stack.push(*it);
            this->stack.push(*(++it));
        } else if (this->isOperand(it, false))
        {
            this->stack.push(*it);
        } else {
            if (!this->stack.empty())
            {
                op2 = this->stack.top();
                this->stack.pop();

                // Remover!!!, depois
                if (!this->stack.empty() && this->stack.top() == '\\')
                    this->stack.pop();

                if (this->unitiOperator(*it))
                {
                    //valor = resultado de aplicar o operador op2;
                    this->stack.push(op2);
                } else {
                    if (!this->stack.empty())
                    {
                        op1 = this->stack.top();
                        this->stack.pop();

                        // Remover!!!, depois
                        if (!this->stack.empty() && this->stack.top() == '\\')
                            this->stack.pop();

                        //valor = resultado de aplicar operador atual sobre os
                        //        operandos op1 e op2
                        this->stack.push(op1);
                    } else {
                        std::cout << "Erro:\nFalta de Operandos.1" << std::endl;
                        return false;
                    }
                }
            } else {
                std::cout << "Erro:\nFalta de Operandos.2" << std::endl;
                return false;
            }
        }
    }

    if (!this->stack.empty()){

        op1 = this->stack.top();
        this->stack.pop();

        // Remover!!!, depois
        if (!this->stack.empty() && this->stack.top() == '\\')
            this->stack.pop();

        if (this->stack.empty())
        {
            std::cout << "Expressão válida" << std::endl;
            //std::cout << "Resultado:\n" << op1 << std::endl;
            return true;
        }
    }
    std::cout << "Erro:\nFalta de Operandos.3" << std::endl;
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
