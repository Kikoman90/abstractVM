/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/06 17:28:10 by fsidler           #+#    #+#             */
/*   Updated: 2018/12/14 20:10:52 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_HPP
#define PARSER_HPP

#include "Lexer.hpp"
#include "OpFactory.hpp"
#include <iostream>
#include <map>

#define AVM_NO_THROW 1
#define AVM_INFO 1

class Parser {

public:

    Parser(std::list<lexeme> const &lexemes);
    ~Parser();

    void        exec();

private:

    Parser();
    Parser(Parser const &src);

    Parser      &operator=(Parser const &rhs);

    void        avm_info(const std::string instr, bool success = true) const;
    void        avm_info(const std::string instr, const eOperandType type, const std::string value, bool success = true) const;
    void        avm_info(const std::string op, IOperand const *v1, IOperand const *v2, bool success = true) const;
    void        avm_error(const std::string msg, unsigned int line = 0);

    void        push(std::list<lexeme>::const_iterator const &it);
    void        pop(std::list<lexeme>::const_iterator const &it);
    void        dump(std::list<lexeme>::const_iterator const &it);
    void        assert(std::list<lexeme>::const_iterator const &it);
    void        add(std::list<lexeme>::const_iterator const &it);
    void        sub(std::list<lexeme>::const_iterator const &it);
    void        mul(std::list<lexeme>::const_iterator const &it);
    void        div(std::list<lexeme>::const_iterator const &it);
    void        mod(std::list<lexeme>::const_iterator const &it);
    void        print(std::list<lexeme>::const_iterator const &it);

    unsigned int                _line;
    OpFactory const             _opf;
    std::list<lexeme>           _lexemes;
    std::list<IOperand const *> _operands;

    std::map<eToken, eOperandType>                                                  _operandType;
    std::map<eToken, void (Parser::*)(std::list<lexeme>::const_iterator const &)>   _operations;

};

#endif