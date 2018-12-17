/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Lexer.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/05 16:10:22 by fsidler           #+#    #+#             */
/*   Updated: 2018/12/17 21:13:13 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Lexer.hpp"

#include <iostream> //remove me thereafter
#include <utility> // remove me thereafter
#include <algorithm>

std::pair<eToken, std::string>  toktostrp[] = { std::make_pair(PUSH, "PUSH"),
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

std::map<eToken, std::string> Lexer::_toktostr(toktostrp, toktostrp + 20); // tmp

extern int      yylex(void);
extern FILE*    yyin;
extern char*    yytext;
extern int      yyleng;

Lexer::Lexer(int ac, char **av) : _state(INITIAL), _inputFromFile(false), _lexemes(0) {

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
        _filestream.open(av[1], std::ifstream::in);
        if (!_filestream)
            throw AVMException("error: file does not exist or could not be opened");
    }

    _reg_init = std::regex("^\\s*(push)|^\\s*(pop)|^\\s*(dump)|^\\s*(assert)|^\\s*(add)|^\\s*(sub)|^\\s*(mul)|^\\s*(div)|\
                            ^\\s*(mod)|^\\s*(print)|^\\s*(exit)|^\\s*((?:^|$|\\n);;)|^\\s*(;.*)|^\\s*(\\n)|\
                            ^\\s*(int8\\()|^\\s*(int16\\()|^\\s*(int32\\()|^\\s*(float\\()|^\\s*(double\\()|^\\s*(.*)");
    _reg_integer = std::regex("\\d+");
    _reg_float = std::regex(" ");
    //_reg_reset = std::regex(")");

}

lexeme                      Lexer::match(std::string const &line, int *offset) const {

    std::smatch match;
    lexeme      lex({static_cast<eToken>(0), ""});

    if (_state & INITIAL) {
        if (std::regex_search(line.begin() + *offset, line.end(), match, _reg_init)) {
            *offset += match.length();
            for (size_t i = 1; i < match.size(); ++i) {
                if (!match[i].str().empty()) {
                    lex.type = static_cast<eToken>(i);
                    lex.value = match[i].str();
                    return (lex);
                }
            }
        }
    }
    else if (_state & STATE_INTEGER) {
        ;//if reg_search() {} 
    }
    else if (_state & STATE_FLOAT) {
        ;
    }
    else if (_state & RESET) {
        ;
    }

    return (lex);

}

void                    Lexer::exec() {
    
    int                 offset;
    lexeme              lex;
    std::string         line;
    std::istream        &istr = (_inputFromFile) ? _filestream : std::cin;

    line = 1;
    while (std::getline(istr, line)) {
        offset = 0;
        if (istr.peek() != EOF)
            line.append("\n");
        std::cout << line;
        // while (match(lex, line, &offset)) with match(lexeme &lex, std::string const line, int *offset) // still not a solution to the int8(value) problem
        while ((lex = match(line, &offset)).type) {
            if (lex.type == END) {
                if (!_inputFromFile)
                    break;
                _lexemes.push_back({ERROR, ""});
            }
            else {
                _lexemes.push_back(lex);
                if (lex.type >= INT8 && lex.type <= INT32)
                    _state = STATE_INTEGER;
                else if (lex.type == FLOAT || lex.type == DOUBLE)
                    _state = STATE_FLOAT;
                if (lex.type == EOL)
                    break;
            }
        }
    }
    if (_inputFromFile)
        _filestream.close();

}

/*void                    Lexer::exec() {
        //std::cout << "NEW" << std::endl;
        //subList.clear();
        //lexeme = match(line);
        //_lexemes.insert(_lexemes.end(), subList.begin(), subList.end());
        //token = &_lexemes.back();
        //token = _lexemes.back().type;
        //if (line.find('\n') != std::string::npos)
        //    _lexemes.push_back({EOL, ""});
        //if (token == ERROR || (token == END && _inputFromFile)) {
        //    _lexemes.push_back({ERROR, ""});
        //    break ;
       // }
        //else if (token == END)
        //    return ;
    
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

}*/

void                    Lexer::showTokens() {

    for (std::list<lexeme>::const_iterator it = _lexemes.begin(); it != _lexemes.end(); ++it) {
        if (it->type != EOL)
            std::cout << '<' << _toktostr[it->type] << ", " << it->value << "> ";
        else
            std::cout << "<EOL>" << std::endl;
    }
    std::cout << std::endl;

}

std::list<lexeme> const &Lexer::getLexemes() const { return (_lexemes); }

Lexer::~Lexer() {}