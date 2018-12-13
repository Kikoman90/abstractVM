/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/06 17:30:14 by fsidler           #+#    #+#             */
/*   Updated: 2018/12/13 19:19:27 by fsidler          ###   ########.fr       */
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

void        Parser::exec() {

    for (std::list<lexeme>::const_iterator it = _lexemes.begin(); it != _lexemes.end(); ++it) {
        if (it->type == EOL)
            _line++;
        else if (std::next(it) != _lexemes.end() && !(it->type & (PUSH | ASSERT | COMMENT)) && !(std::next(it)->type & (COMMENT | EOL)))
            _avme.addMsg("parser error: multiple instructions", _line);
        if (it->type == EXIT) {
            if (!_avme.empty())
                throw _avme;
            return ;
        }
        else if (it->type < EXIT)
            (this->*_operations[it->type])(it);
    }
    _avme.addMsg("parser error: missing 'exit' instruction");
    throw _avme;
}

void        Parser::push(const std::list<lexeme>::const_iterator &it) {

    std::list<lexeme>::const_iterator it_next;

    if ((it_next = std::next(it)) == _lexemes.end() || it_next->type < INT8) {
        _avme.addMsg("parser error: 'push' instruction must be followed by a value", _line);
        return ;
    }
    try {
        IOperand const *op = _opf.createOperand(_operandType[it_next->type], it_next->value);
        _operands.push_front(op);
    }
    catch (AVMException &e) {
        _avme.addMsg(e.getMsg(), _line);
    }

}

void        Parser::pop(const std::list<lexeme>::const_iterator &it) {
    
    (void)it;
    if (_operands.empty()) {
        _avme.addMsg("parser error: 'pop' instruction on empty stack", _line);
        return ;
    }
    delete _operands.front();
    _operands.pop_front();

}

void        Parser::dump(const std::list<lexeme>::const_iterator &it) {
    
    (void)it;
    for (std::list<IOperand const *>::const_iterator it = _operands.begin(); it != _operands.end(); ++it)
        std::cout << (*it)->toString() << std::endl;

}

void        Parser::assert(const std::list<lexeme>::const_iterator &it) {
    
    std::list<lexeme>::const_iterator   it_next;
    int                                 error = 0;
    
    if (_operands.empty()) {
        _avme.addMsg("parser error: 'assert' instruction on empty stack", _line);
        error = 1;
    }
    if ((it_next = std::next(it)) == _lexemes.end() || it_next->type < INT8) {
        _avme.addMsg("parser error: 'assert' instruction must be followed by a value", _line);
        error = 1;
    }
    if (error)
        return ;
    IOperand const *op = _opf.createOperand(_operandType[it_next->type], it_next->value);
    if (op->getType() != _operands.front()->getType() || op->toString().compare(_operands.front()->toString()))
        _avme.addMsg("parser error: 'assert' instruction is false", _line);

}

void        Parser::add(const std::list<lexeme>::const_iterator &it) {

    (void)it;
    if (_operands.size() < 2) {
        _avme.addMsg("parser error: 'add' operation; less than 2 values on the stack", _line);
        return ;
    }
    std::list<const IOperand *>::const_iterator iter = _operands.begin();
    IOperand const *v1 = *iter;
    std::advance(iter, 1);
    IOperand const *v2 = *iter;
    IOperand const *sub;
    try {
        sub = *v2 + *v1;
    }
    catch (AVMException &e) {
        _avme.addMsg(e.getMsg(), _line);
        return ;
    }
    _operands.pop_front();
    _operands.pop_front();
    _operands.push_front(sub);
    delete v1;
    delete v2;

}

void        Parser::sub(const std::list<lexeme>::const_iterator &it) {

    (void)it;
    if (_operands.size() < 2) {
        _avme.addMsg("parser error: 'sub' operation; less than 2 values on the stack", _line);
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
        _avme.addMsg(e.getMsg(), _line);
        return ;
    }
    _operands.pop_front();
    _operands.pop_front();
    _operands.push_front(sub);
    delete v1;
    delete v2;

}

void        Parser::mul(const std::list<lexeme>::const_iterator &it) {

    (void)it;
    if (_operands.size() < 2) {
        _avme.addMsg("parser error: 'mul' operation; less than 2 values on the stack", _line);
        return ;
    }
    std::list<const IOperand *>::const_iterator iter = _operands.begin();
    IOperand const *v1 = *iter;
    std::advance(iter, 1);
    IOperand const *v2 = *iter;
    IOperand const *sub;
    try {
        sub = *v2 * *v1;
    }
    catch (AVMException &e) {
        _avme.addMsg(e.getMsg(), _line);
        return ;
    }
    _operands.pop_front();
    _operands.pop_front();
    _operands.push_front(sub);
    delete v1;
    delete v2;

}

void        Parser::div(const std::list<lexeme>::const_iterator &it) {

    (void)it;
    if (_operands.size() < 2) {
        _avme.addMsg("parser error: 'div' operation; less than 2 values on the stack", _line);
        return ;
    }
    std::list<const IOperand *>::const_iterator iter = _operands.begin();
    IOperand const *v1 = *iter;
    std::advance(iter, 1);
    IOperand const *v2 = *iter;
    IOperand const *sub;
    try {
        sub = *v2 / *v1;
    }
    catch (AVMException &e) {
        _avme.addMsg(e.getMsg(), _line);
        return ;
    }
    _operands.pop_front();
    _operands.pop_front();
    _operands.push_front(sub);
    delete v1;
    delete v2;

}

void        Parser::mod(const std::list<lexeme>::const_iterator &it) {

    (void)it;
    if (_operands.size() < 2) {
        _avme.addMsg("parser error: 'mod' operation; less than 2 values on the stack", _line);
        return ;
    }
    std::list<const IOperand *>::const_iterator iter = _operands.begin();
    IOperand const *v1 = *iter;
    std::advance(iter, 1);
    IOperand const *v2 = *iter;
    IOperand const *sub;
    try {
        sub = *v2 % *v1;
    }
    catch (AVMException &e) {
        _avme.addMsg(e.getMsg(), _line);
        return ;
    }
    _operands.pop_front();
    _operands.pop_front();
    _operands.push_front(sub);
    delete v1;
    delete v2;

}

void        Parser::print(const std::list<lexeme>::const_iterator &it) {

    (void)it;
    if (_operands.empty()) {
        _avme.addMsg("parser error: 'print' instruction on empty stack", _line);
        return ;
    }
    IOperand const *op = _operands.front();
    if (op->getType() != Int8) {
        _avme.addMsg("parser error: 'print' instruction; type is not 8-bit integer", _line);
        return ;
    }
    std::cout << static_cast<char>(std::stoi(op->toString())) << std::endl;

}

Parser::~Parser() {
    
    for (std::list<IOperand const *>::iterator it = _operands.begin(); it != _operands.end(); ++it)
        delete *it;

}