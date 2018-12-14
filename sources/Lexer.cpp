/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Lexer.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/05 16:10:22 by fsidler           #+#    #+#             */
/*   Updated: 2018/12/14 20:02:27 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Lexer.hpp"

extern int      yylex(void);
extern FILE*    yyin;
extern char*    yytext;
extern int      yyleng;

Lexer::Lexer(int ac, char **av) : _lexemes(0), _inputFromFile(false) {

    if (ac > 2)
        throw AVMException("error: too many arguments");
    else if (ac == 2) {
        struct stat s;
        if (stat(av[1], &s) == 0) {
            if (!(s.st_mode & S_IFREG))
                throw AVMException("error: argument is not a file");
        }   
        else {
            std::stringstream strs;
            strs << "error: " << strerror(errno);
            throw AVMException(strs.str());
        }
        _inputFromFile = true;
        if (!(yyin = fopen(av[1], "r")))
            throw AVMException("error: file does not exist or could not be opened");       
    }

}

void                    Lexer::exec() {
    
    int             token;
    std::string     value;
    unsigned int    line;

    line = 1;
    while ((token = yylex()))
    {
        if (token == ERROR || (token == END && _inputFromFile)) {
            while ((token = yylex()) && token != EOL)
                ;
            _lexemes.push_back({ERROR, ""});
        }
        else if (token == END)
            break;
        if (token) {
            (token == EOL) ? line++ : 0;
            value = yytext;
            _lexemes.push_back({static_cast<eToken>(token), value});
        }
    }
    if (fclose(yyin) != 0)
        throw AVMException("error: file could not be closed");

}

std::list<lexeme> const &Lexer::getLexemes() const { return (_lexemes); }

Lexer::~Lexer() {}