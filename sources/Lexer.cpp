/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Lexer.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/05 16:10:22 by fsidler           #+#    #+#             */
/*   Updated: 2018/12/10 16:10:05 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Lexer.hpp"
#include <utility> // remove me

std::pair<eToken, std::string>  pairs[] = { std::make_pair(PUSH, "PUSH"),
                                            std::make_pair(POP, "POP"),
                                            std::make_pair(DUMP, "DUMP"),
                                            std::make_pair(ASSERT, "ASSERT"),
                                            std::make_pair(ADD, "ADD"),
                                            std::make_pair(SUB, "SUB"),
                                            std::make_pair(MUL, "MUL"),
                                            std::make_pair(DIV, "DIV"),
                                            std::make_pair(MOD, "MOD"),
                                            std::make_pair(PRINT, "PRINT"),
                                            std::make_pair(EXIT, "EXIT"),
                                            std::make_pair(END, "END"),
                                            std::make_pair(COMMENT, "COMMENT"),
                                            std::make_pair(EOL, "EOL"),
                                            std::make_pair(INT8, "INT8"),
                                            std::make_pair(INT16, "INT16"),
                                            std::make_pair(INT32, "INT32"),
                                            std::make_pair(FLOAT, "FLOAT"),
                                            std::make_pair(DOUBLE, "DOUBLE"),
                                            std::make_pair(ERROR, "ERROR") }; // tmp

std::map<eToken, std::string> Lexer::_toktype(pairs, pairs + 20); // tmp

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
        if (token == ERROR || (token == END && _inputFromFile)) {
            while ((token = yylex()) && token != EOL)
                ;
            avme.addMsg("lexer error: invalid or unknown instruction", line);
        }
        else if (token == END)
            break;
        if (token) {
            (token == EOL) ? line++ : 0;
            value = yytext;
            _lexemes.push_back({static_cast<eToken>(token), value});
        }
    }
    if (!avme.empty())
        throw avme;

}

// remove me
void                    Lexer::showLexemes() const {
    
    std::cout << "list length = " << _lexemes.size() << std::endl;
    for (std::list<lexeme>::const_iterator it = _lexemes.begin(); it != _lexemes.end(); ++it) {
        std::cout << '<' << _toktype[it->type];
        if ((*it).type != EOL)
            std::cout << ", " << it->value;
        std::cout << '>' << std::endl;
    }

}

std::list<lexeme> const &Lexer::getLexemes() const { return (_lexemes); }

Lexer::~Lexer() {}