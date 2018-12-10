/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/06 17:28:10 by fsidler           #+#    #+#             */
/*   Updated: 2018/12/10 15:32:02 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_HPP
#define PARSER_HPP

#include "Lexer.hpp"
#include "OpFactory.hpp"
#include <map>

class Parser {

public:

    Parser(std::list<lexeme> const &lexemes);
    ~Parser();

    void        exec();

private:

    Parser();
    Parser(Parser const &src);

    Parser      &operator=(Parser const &rhs);

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
    AVMException                _avme;
    std::list<lexeme> const     _lexemes;
    std::list<IOperand const *> _operands;

    std::map<eToken, eOperandType>                                                  _operandType;
    std::map<eToken, void (Parser::*)(std::list<lexeme>::const_iterator const &)>   _operations;

};

#endif