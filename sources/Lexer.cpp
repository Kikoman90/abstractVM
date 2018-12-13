/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Lexer.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/05 16:10:22 by fsidler           #+#    #+#             */
/*   Updated: 2018/12/13 18:45:28 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Lexer.hpp"

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
            throw AVMException("lexer error: file does not exist or could not be opened");
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

std::list<lexeme> const &Lexer::getLexemes() const { return (_lexemes); }

Lexer::~Lexer() {}