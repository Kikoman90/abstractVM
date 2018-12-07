/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/06 17:30:14 by fsidler           #+#    #+#             */
/*   Updated: 2018/12/07 20:20:38 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parser.hpp"
#include <cmath>

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
    _operations[EXIT] = &Parser::exit;

    _operandType[INT8] = Int8;
    _operandType[INT16] = Int16;
    _operandType[INT32] = Int32;
    _operandType[FLOAT] = Float;
    _operandType[DOUBLE] = Double;

}

// instructions Push and Assert MUST be followed by a value (else throw exception)
// instructions (or values (case above)) MUST be followed by either a comment or Eol EXCEPT if it is the last token
// comments MUST be followed by Eol EXCEPT if it is the last token

void        Parser::exec() {

    int iter = 0;

    for (std::list<lexeme>::const_iterator it = _lexemes.begin(); it != _lexemes.end(); ++it) {
        std::cout << "iter = " << iter++ << std::endl;
        if (it->type == EOL)
            _line++;
        else if (it != _lexemes.end() && !(it->type & (PUSH | ASSERT | COMMENT)) && !(std::next(it)->type & (COMMENT | EOL))) {
            std::cout << "LINE IS " << _line << std::endl;
            std::cout << "reminded comment " << COMMENT << " and EOL " << EOL << std::endl;
            std::cout << "reminded both " << (EOL | COMMENT) << std::endl;
            std::cout << std::next(it)->type << std::endl;
            _avme.addMsg("Parser error: multiple instructions", _line);
        }
        if (it->type < END)
            (this->*_operations[it->type])(it);
    }
    _avme.addMsg("Parser error: missing 'exit' instruction");
    throw _avme;

}

void        Parser::push(const std::list<lexeme>::const_iterator &it) {

    std::list<lexeme>::const_iterator it_next;

    if (it == _lexemes.end() || (it_next = std::next(it))->type < INT8) {
        _avme.addMsg("Parser error: 'push' instruction must be followed by a value", _line);
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
        _avme.addMsg("Parser error: 'pop' instruction on empty stack", _line);
        return ;
    }
    _operands.pop_front();

}

void        Parser::dump(const std::list<lexeme>::const_iterator &it) {
    
    (void)it;
    for (std::list<IOperand const *>::const_iterator it = _operands.begin(); it != _operands.end(); ++it)
        std::cout << (*it)->toString() << std::endl;

}

void        Parser::assert(const std::list<lexeme>::const_iterator &it) {
    
    std::list<lexeme>::const_iterator it_next;
    
    if (_operands.empty()) {
        _avme.addMsg("Parser error: 'assert' instruction on empty stack", _line);
        return ;
    }
    if (it == _lexemes.end() || (it_next = std::next(it))->type < INT8) {
        _avme.addMsg("Parser error: 'assert' instruction must be followed by a value", _line);
        return ;
    }
    if ((_operandType[it_next->type] != _operands.front()->getType()) || !it_next->value.compare(_operands.front()->toString()))
        _avme.addMsg("Parser error: 'assert' instruction is false");

}

void        Parser::add(const std::list<lexeme>::const_iterator &it) {
    (void)it;
}

void        Parser::sub(const std::list<lexeme>::const_iterator &it) {
    (void)it;
}

void        Parser::mul(const std::list<lexeme>::const_iterator &it) {
    (void)it;
}

void        Parser::div(const std::list<lexeme>::const_iterator &it) {
    (void)it;
}

void        Parser::mod(const std::list<lexeme>::const_iterator &it) {
    (void)it;
}

void        Parser::print(const std::list<lexeme>::const_iterator &it) {
    (void)it;
}

void        Parser::exit(const std::list<lexeme>::const_iterator &it) {
    
    (void)it;
    if (!_avme.empty())
        throw _avme;
    std::exit(0);

}

Parser::~Parser() {}