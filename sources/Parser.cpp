/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/06 17:30:14 by fsidler           #+#    #+#             */
/*   Updated: 2018/12/06 19:56:23 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parser.hpp"

Parser::Parser(const std::list<lexeme> &lexemes) : _line(1), _lexemes(lexemes) {
    _operations[0] = &Parser::push;
    _operations[1] = &Parser::pop;
    _operations[2] = &Parser::dump;
    _operations[3] = &Parser::assert;
    _operations[4] = &Parser::add;
    _operations[5] = &Parser::sub;
    _operations[6] = &Parser::mul;
    _operations[7] = &Parser::div;
    _operations[8] = &Parser::mod;
    _operations[9] = &Parser::print;
    _operations[10] = &Parser::exit;
}

void        Parser::exec() {

    for (std::list<lexeme>::const_iterator it = _lexemes.begin(); it != _lexemes.end(); ++it) {
        if (it->type == EOL)
            _line++;
        if (it->type < END)
            (this->*_operations.at(it->type - PUSH))(it);
    }
    // add to exception instead of throwing
    throw AVMException("Parser error: missing exit instruction", 0);

}

void        Parser::push(const std::list<lexeme>::const_iterator &it) const {

    (void)it;
    try {
        std::cout << "push" << std::endl;
        ;//factory create stuff();
    }
    catch (std::out_of_range &oor) {
        // add to exception instead of throwing
        throw AVMException("Parser error: push must be followed by a value", _line);
    }
}

void        Parser::pop(const std::list<lexeme>::const_iterator &it) const {
    (void)it;
}

void        Parser::dump(const std::list<lexeme>::const_iterator &it) const {
    (void)it;
}

void        Parser::assert(const std::list<lexeme>::const_iterator &it) const {
    (void)it;
}

void        Parser::add(const std::list<lexeme>::const_iterator &it) const {
    (void)it;
}

void        Parser::sub(const std::list<lexeme>::const_iterator &it) const {
    (void)it;
}

void        Parser::mul(const std::list<lexeme>::const_iterator &it) const {
    (void)it;
}

void        Parser::div(const std::list<lexeme>::const_iterator &it) const {
    (void)it;
}

void        Parser::mod(const std::list<lexeme>::const_iterator &it) const {
    (void)it;
}

void        Parser::print(const std::list<lexeme>::const_iterator &it) const {
    (void)it;
}

void        Parser::exit(const std::list<lexeme>::const_iterator &it) const {
    (void)it;
}

Parser::~Parser() {}