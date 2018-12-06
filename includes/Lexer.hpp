/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Lexer.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/05 11:18:54 by fsidler           #+#    #+#             */
/*   Updated: 2018/12/06 18:57:23 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_HPP
#define LEXER_HPP

#include "AVMException.hpp"
#include <iostream>
#include <list>

enum eToken {
    PUSH = 1,
    POP,
    DUMP,
    ASSERT,
    ADD,
    SUB,
    MUL,
    DIV,
    MOD,
    PRINT,
    EXIT,
    END,
    COMMENT,
    EOL,
    INT8,
    INT16,
    INT32,
    FLOAT,
    DOUBLE,
    ERROR
};

struct lexeme {

    eToken              type;
    const std::string   value;

};

class Lexer {

public:

    Lexer(int ac, char **av);
    ~Lexer();

    void                    exec();

    void                    showLexemes() const; // remove me
    const std::list<lexeme> &getLexemes() const;

private:
    
    Lexer();
    Lexer(Lexer const &src);

    Lexer                   &operator=(Lexer const &rhs);

    std::list<lexeme>   _lexemes;
    bool                _inputFromFile;

};

#endif