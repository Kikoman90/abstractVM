/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Lexer.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/05 11:18:54 by fsidler           #+#    #+#             */
/*   Updated: 2018/12/17 19:11:04 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_HPP
#define LEXER_HPP

#include "AVMException.hpp"
#include <sys/stat.h>
#include <errno.h>
#include <fstream>
#include <list>
#include <map> //?
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

enum lexState {
    INITIAL = 1 << 0,
    STATE_INTEGER = 1 << 1,
    STATE_FLOAT = 1 << 2,   
    RESET = 1 << 3
};

struct lexeme {

    eToken      type;
    std::string value;

};

class Lexer {

public:

    Lexer(int ac, char **av);
    ~Lexer();

    lexeme                  match(std::string const &line, int *offset) const;
    //int                     match(std::string const &line, int *offset) const;
    //void                    match(std::list<lexeme> &subList, std::string const &line) const;
    void                    exec();
    void                    showTokens(); // remove me

    std::list<lexeme> const &getLexemes() const;

    static std::map<eToken, std::string>   _toktostr; // rem
    static std::map<std::string, eToken>   _strtotok; // rem

private:
    
    Lexer();
    Lexer(Lexer const &src);

    Lexer                   &operator=(Lexer const &rhs);

    std::regex          _reg_init;
    std::regex          _reg_integer;
    std::regex          _reg_float;
    lexState            _state;
    bool                _inputFromFile;
    std::ifstream       _filestream;
    std::list<lexeme>   _lexemes;

};

#endif