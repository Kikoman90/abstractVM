/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Lexer.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/05 11:18:54 by fsidler           #+#    #+#             */
/*   Updated: 2018/12/14 18:45:59 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_HPP
#define LEXER_HPP

#include "AVMException.hpp"
#include <sys/stat.h>
#include <errno.h>
#include <list>

enum eToken {
    PUSH = 1 << 0,
    POP = 1 << 1,
    DUMP = 1 << 2,
    ASSERT = 1 << 3,
    ADD = 1 << 4,
    SUB = 1 << 5,
    MUL = 1 << 6,
    DIV = 1 << 7,
    MOD = 1 << 8,
    PRINT = 1 << 9,
    EXIT = 1 << 10,
    END = 1 << 11,
    COMMENT = 1 << 12,
    EOL = 1 << 13,
    INT8 = 1 << 14,
    INT16 = 1 << 15,
    INT32 = 1 << 16,
    FLOAT = 1 << 17,
    DOUBLE = 1 << 18,
    ERROR = 1 << 19
};

struct lexeme {

    eToken              type;
    std::string const   value;

};

class Lexer {

public:

    Lexer(int ac, char **av);
    ~Lexer();

    void                    exec();

    std::list<lexeme> const &getLexemes() const;

private:
    
    Lexer();
    Lexer(Lexer const &src);

    Lexer                   &operator=(Lexer const &rhs);

    std::list<lexeme>   _lexemes;
    bool                _inputFromFile;

};

#endif