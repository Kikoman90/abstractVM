/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/06 17:30:14 by fsidler           #+#    #+#             */
/*   Updated: 2018/12/18 23:00:04 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parser.hpp"

Parser::Parser(const std::list<lexeme> &lexemes) : _line(1), _lexemes(lexemes) {

    _operations[PUSH] = &Parser::push;
    _operations[POP] = &Parser::pop;
    _operations[DUMP] = &Parser::dump;
    _operations[ASSERT] = &Parser::assert;
    _operations[ADD] = &Parser::add;
    _operations[SUB] = &Parser::sub;
    _operations[MUL] = &Parser::mul;
    _operations[DIV] = &Parser::div;
    _operations[MOD] = &Parser::mod;
    _operations[PRINT] = &Parser::print;

    _operandType[INT8] = Int8;
    _operandType[INT16] = Int16;
    _operandType[INT32] = Int32;
    _operandType[FLOAT] = Float;
    _operandType[DOUBLE] = Double;

}

void        Parser::avm_info(const std::string instr, bool success) const {

    std::cout << ((success) ? "\033[32;4m" : "\033[31;4m");
    std::cout << "[AVM:" << _line << "] " << instr << std::endl;
    std::cout << "\033[0;0m";

}

void        Parser::avm_info(const std::string instr, const eOperandType type, const std::string value, bool success) const {

    std::cout << ((success) ? "\033[32;4m" : "\033[31;4m");
    std::cout << "[AVM:" << _line << "] " << instr << ' ' << _opf._opMap[type] << '(' << value << ")\n";
    std::cout << "\033[0;0m";

}

void        Parser::avm_info(const std::string op, IOperand const *v1, IOperand const *v2, bool success) const {
    
    std::cout << ((success) ? "\033[32;4m" : "\033[31;4m");
    std::cout << "[AVM:" << _line<< "] " << op << '{' << _opf._opMap[v1->getType()] << '(' << v1->toString() << "), " << _opf._opMap[v2->getType()] << '(' << v2->toString() << ")}\n";
    std::cout << "\033[0;0m";
    
}

void        Parser::avm_error(const std::string msg, unsigned int line) {

    std::stringstream strs;

    strs << "\033[31m";
    if (line == 0)
        strs << msg;
    else
        strs << msg << " (line " << line << ")";
    if (AVM_NO_THROW)
        std::cout << strs.str() << std::endl;
    else
        throw AVMException(strs.str());
    std::cout << "\033[0m";

}

void        Parser::exec() {

    bool    error;
    eToken  prev_type = EOL;

    for (std::list<lexeme>::const_iterator it = _lexemes.begin(); it != _lexemes.end(); ++it) {
        error = false;
        std::list<lexeme>::const_iterator fst = it;
        while (it != _lexemes.end() && it->type != EOL) {
            if (it->type <= EXIT && prev_type != EOL)
                error = true;
            else if (it->type == ERROR)
                error = true;
            else if (it->type >= INT8 && (prev_type != PUSH && prev_type != ASSERT))
                error = true;
            prev_type = it->type;
            it++;
        }
        if (error) {
            avm_error("error: multiple/invalid/unknown instructions", _line);
            if (AVM_INFO)
                std::cout << std::endl;
            if (it == _lexemes.end())
                break;
        }
        else
            it = fst;
        if (it != _lexemes.end() && it->type == EOL)
            _line++;
        else if (it != _lexemes.end() && it->type == EXIT) {
            if (AVM_INFO)
                avm_info("exit");
            return ; 
        }
        else if (it != _lexemes.end() && it->type < EXIT) {
            (this->*_operations[it->type])(it);
            if (AVM_INFO)
                std::cout << std::endl;
        }
        prev_type = it->type;
    }
    avm_error("parser error: missing 'exit' instruction");

}

void        Parser::push(const std::list<lexeme>::const_iterator &it) {

    std::list<lexeme>::const_iterator it_next;

    if ((it_next = std::next(it)) == _lexemes.end() || it_next->type < INT8) {
        if (AVM_INFO)
            avm_info("push", false);
        avm_error("error: 'push' instruction must be followed by a value", _line);
        return ;
    }
    if (AVM_INFO)
        avm_info("push", _operandType[it_next->type], it_next->value);
    try {
        IOperand const *op = _opf.createOperand(_operandType[it_next->type], it_next->value);
        _operands.push_front(op);
    }
    catch (AVMException &e) {
        avm_error(e.what(), _line);
    }

}

void        Parser::pop(const std::list<lexeme>::const_iterator &it) {
    
    (void)it;
    if (_operands.empty()) {
        if (AVM_INFO)
            avm_info("pop", false);
        avm_error("error: 'pop' instruction on empty stack", _line);
        return ;
    }
    if (AVM_INFO)
        avm_info("pop");
    delete _operands.front();
    _operands.pop_front();

}

void        Parser::dump(const std::list<lexeme>::const_iterator &it) {
    
    (void)it;
    if (AVM_INFO)
        avm_info("dump");
    for (std::list<IOperand const *>::const_iterator it = _operands.begin(); it != _operands.end(); ++it) {
        if (AVM_INFO)
            std::cout << _opf._opMap[(*it)->getType()] << '(' << (*it)->toString() << ')' << std::endl;
        else
            std::cout << (*it)->toString() << std::endl;
    }

}

void        Parser::assert(const std::list<lexeme>::const_iterator &it) {
    
    std::list<lexeme>::const_iterator   it_next;
    int                                 error = 0;
    
    if ((it_next = std::next(it)) == _lexemes.end() || it_next->type < INT8) {
        if (AVM_INFO)
            avm_info("assert", false);
        avm_error("error: 'assert' instruction must be followed by a value", _line);
        error = 1;
    }
    if (_operands.empty()) {
        if (!error && AVM_INFO)
            avm_info("assert", _operandType[it_next->type], it_next->value, false);
        avm_error("error: 'assert' instruction on empty stack", _line);        
        error = 1;
    }
    if (error)
        return ;
    IOperand const *op = _opf.createOperand(_operandType[it_next->type], it_next->value);
    if (op->getType() != _operands.front()->getType() || op->toString().compare(_operands.front()->toString())) {
        if (AVM_INFO)
            avm_info("assert", _operandType[it_next->type], it_next->value, false);
        avm_error("error: 'assert' instruction is false", _line);        
    }
    else if (AVM_INFO)
        avm_info("assert", _operandType[it_next->type], it_next->value);

}

void        Parser::add(const std::list<lexeme>::const_iterator &it) {

    (void)it;
    if (_operands.size() < 2) {
        if (AVM_INFO)
            avm_info("add", false);
        avm_error("error: 'add' operation; less than two values on the stack", _line);
        return ;
    }
    std::list<const IOperand *>::const_iterator iter = _operands.begin();
    IOperand const *v1 = *iter;
    std::advance(iter, 1);
    IOperand const *v2 = *iter;
    IOperand const *add;
    try {
        add = *v2 + *v1;
    }
    catch (AVMException &e) {
        if (AVM_INFO)
            avm_info("add", v2, v1, false);
        avm_error(e.what(), _line);
        return ;
    }
    if (AVM_INFO)
        avm_info("add", v2, v1);
    _operands.pop_front();
    _operands.pop_front();
    _operands.push_front(add);
    delete v1;
    delete v2;

}

void        Parser::sub(const std::list<lexeme>::const_iterator &it) {

    (void)it;
    if (_operands.size() < 2) {
        if (AVM_INFO)
            avm_info("sub", false);
        avm_error("error: 'sub' operation; less than two values on the stack", _line);
        return ;
    }
    std::list<const IOperand *>::const_iterator iter = _operands.begin();
    IOperand const *v1 = *iter;
    std::advance(iter, 1);
    IOperand const *v2 = *iter;
    IOperand const *sub;
    try {
        sub = *v2 - *v1;
    }
    catch (AVMException &e) {
        if (AVM_INFO)
            avm_info("sub", v2, v1, false);
        avm_error(e.what(), _line);
        return ;
    }
    if (AVM_INFO)
        avm_info("sub", v2, v1);
    _operands.pop_front();
    _operands.pop_front();
    _operands.push_front(sub);
    delete v1;
    delete v2;

}

void        Parser::mul(const std::list<lexeme>::const_iterator &it) {

    (void)it;
    if (_operands.size() < 2) {
        if (AVM_INFO)
            avm_info("mul", false);
        avm_error("error: 'mul' operation; less than two values on the stack", _line);
        return ;
    }
    std::list<const IOperand *>::const_iterator iter = _operands.begin();
    IOperand const *v1 = *iter;
    std::advance(iter, 1);
    IOperand const *v2 = *iter;
    IOperand const *mul;
    try {
        mul = *v2 * *v1;
    }
    catch (AVMException &e) {
        if (AVM_INFO)
            avm_info("mul", v2, v1, false);
        avm_error(e.what(), _line);
        return ;
    }
    if (AVM_INFO)
        avm_info("mul", v2, v1);
    _operands.pop_front();
    _operands.pop_front();
    _operands.push_front(mul);
    delete v1;
    delete v2;

}

void        Parser::div(const std::list<lexeme>::const_iterator &it) {

    (void)it;
    if (_operands.size() < 2) {
        if (AVM_INFO)
            avm_info("div", false);
        avm_error("error: 'div' operation; less than two values on the stack", _line);
        return ;
    }
    std::list<const IOperand *>::const_iterator iter = _operands.begin();
    IOperand const *v1 = *iter;
    std::advance(iter, 1);
    IOperand const *v2 = *iter;
    IOperand const *div;
    try {
        div = *v2 / *v1;
    }
    catch (AVMException &e) {
        if (AVM_INFO)
            avm_info("div", v2, v1, false);
        avm_error(e.what(), _line);
        return ;
    }
    if (AVM_INFO)
        avm_info("div", v2, v1);
    _operands.pop_front();
    _operands.pop_front();
    _operands.push_front(div);
    delete v1;
    delete v2;

}

void        Parser::mod(const std::list<lexeme>::const_iterator &it) {

    (void)it;
    if (_operands.size() < 2) {
        if (AVM_INFO)
            avm_info("mod", false);
        avm_error("error: 'mod' operation; less than two values on the stack", _line);
        return ;
    }
    std::list<const IOperand *>::const_iterator iter = _operands.begin();
    IOperand const *v1 = *iter;
    std::advance(iter, 1);
    IOperand const *v2 = *iter;
    IOperand const *mod;
    try {
        mod = *v2 % *v1;
    }
    catch (AVMException &e) {
        if (AVM_INFO)
            avm_info("mod", v2, v1, false);
        avm_error(e.what(), _line);        
        return ;
    }
    if (AVM_INFO)
        avm_info("mod", v2, v1);
    _operands.pop_front();
    _operands.pop_front();
    _operands.push_front(mod);
    delete v1;
    delete v2;

}

void        Parser::print(const std::list<lexeme>::const_iterator &it) {

    (void)it;
    if (_operands.empty()) {
        if (AVM_INFO)
            avm_info("print", false);
        avm_error("error: 'print' instruction on empty stack", _line);
        return ;
    }
    IOperand const *op = _operands.front();
    if (op->getType() != Int8) {
        if (AVM_INFO)
            avm_info("print", false);
        avm_error("error: 'print' instruction; type is not 8-bit integer", _line);       
        return ;
    }
    if (AVM_INFO)
        avm_info("print");
    std::cout << static_cast<char>(std::stoi(op->toString())) << std::endl;

}

Parser::~Parser() {
    
    for (std::list<IOperand const *>::iterator it = _operands.begin(); it != _operands.end(); ++it)
        delete *it;

}