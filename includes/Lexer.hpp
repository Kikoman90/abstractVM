/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Lexer.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/05 11:18:54 by fsidler           #+#    #+#             */
/*   Updated: 2019/01/02 15:18:06 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_HPP
#define LEXER_HPP

#include "AVMException.hpp"
#include <sys/stat.h>
#include <errno.h>
#include <iostream>
#include <fstream>
#include <list>
#include <regex>

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
    POW,
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

    eToken      type;
    std::string value;

};

class Lexer {

public:

    Lexer(int ac, char **av);
    ~Lexer();

    lexeme                  match(std::string const &line, size_t *offset) const;
    void                    exec();

    std::list<lexeme> const &getLexemes() const;

private:
    
    Lexer();
    Lexer(Lexer const &src);

    Lexer                   &operator=(Lexer const &rhs);

    std::regex          _reg_init;
    std::regex          _reg_integer;
    std::regex          _reg_float;
    bool                _inputFromFile;
    std::ifstream       _filestream;
    std::list<lexeme>   _lexemes;

};

#endif