/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Lexer.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/05 16:10:22 by fsidler           #+#    #+#             */
/*   Updated: 2018/12/07 20:23:56 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Lexer.hpp"
#include <cmath>//
///
//
//
///
//
//
//
//
//
//
//
//
//
//


extern int      yylex(void);
extern FILE*    yyin;
extern char*    yytext;
extern int      yyleng;

Lexer::Lexer(int ac, char **av) : _lexemes(0), _inputFromFile(false) {

    if (ac > 2)
        throw AVMException("lexer error: too many arguments");
    else if (ac == 2) {
        _inputFromFile = true;
        if (!(yyin = fopen(av[1], "r")))
            throw AVMException("lexer error: file does not exist or couldn't be opened");
    }

}

void                    Lexer::exec() {
    
    int             token;
    std::string     value;
    AVMException    avme;
    unsigned int    line;

    line = 1;
    while ((token = yylex()))
    {
        if (token == EOL)
            line++;
        else if (token == ERROR || (token == END && _inputFromFile))
            avme.addMsg("lexer error: invalid or unknown instruction", line);
        else if (token == END)
            break;
        value = yytext;
        _lexemes.push_back({static_cast<eToken>(token), value});
    }
    if (!avme.empty())
        throw avme;

}

// remove me
void                    Lexer::showLexemes() const {
    
    const std::string   tokenTypes[] = {\
        "PUSH", "POP", "DUMP", "ASSERT", "ADD", "SUB", "MUL", \
        "DIV", "MOD", "PRINT", "EXIT", "END", "COMMENT", "EOL", \
        "INT8", "INT16", "INT32", "FLOAT", "DOUBLE", "ERROR" };

    //td::cout << log2((float)ERROR) << std::endl;
    std::cout << "list length = " << _lexemes.size() << std::endl;
    for (std::list<lexeme>::const_iterator it = _lexemes.begin(); it != _lexemes.end(); ++it) {
        std::cout << '<' << tokenTypes[(int)log2((float)it->type)];
        if ((*it).type != EOL)
            std::cout << ", " << it->value;
        std::cout << '>' << std::endl;
    }

}

std::list<lexeme> const &Lexer::getLexemes() const { return (_lexemes); }

Lexer::~Lexer() {}