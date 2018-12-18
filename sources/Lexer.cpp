/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Lexer.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/05 16:10:22 by fsidler           #+#    #+#             */
/*   Updated: 2018/12/18 22:58:49 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Lexer.hpp"

Lexer::Lexer(int ac, char **av) : _inputFromFile(false), _lexemes(0) {

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

    _reg_init = std::regex(     "[\\r\\t\\f ]*"
                                "(?:"
                                    "(?:\\b(?:(push)|(pop)|(dump)|(assert)|"
                                    "(add)|(sub)|(mul)|(div)|(mod)|(print)|(exit))\\b)|"
                                    "(;;(?:[\\r\\t\\f ]*)(?:$|\\n))|(;.*)|(\\n)|"
                                    "(?:\\b(?:(int8\\()|(int16\\()|(int32\\()|(float\\()|(double\\()))|(.*)"
                                "))");

    _reg_integer = std::regex(  "\\s*"
                                "([-+]?)"
                                "(0+\\B)?"
                                "(\\d+)"
                                "(?:([eE][-+]?)(0+\\B)?(\\d+))?"
                                "\\s*\\)\\s*");

    _reg_float = std::regex(    "\\s*"
                                "([-+]?)"
                                "(?:"
                                    "(?:(0+\\B)?"
                                    "((?:\\d+)(?:[.]\\d*)?))|"
                                    "([.]\\d+)"
                                ")"
                                "(?:([eE][-+]?)(0+\\B)?(\\d+))?"
                                "(?:[f]?)"
                                "\\s*\\)\\s*");

}

lexeme                      Lexer::match(std::string const &line, size_t *offset) const {

    std::smatch match;
    lexeme      lex({static_cast<eToken>(0), ""});

    if (std::regex_search(line.begin() + *offset, line.end(), match, _reg_init)) {
        *offset += match.length();
        for (size_t i = 1; i < match.size(); ++i) {
            if (!match[i].str().empty()) {
                if ((lex.type = static_cast<eToken>(i)) >= INT8 && lex.type <= INT32) {
                    if (std::regex_match(line.begin() + *offset, line.end(), match, _reg_integer)) {
                        if (line.back() == '\n')
                            *offset -= 1;
                        *offset += match.length();
                        lex.value = match[1].str() + match[3].str() + match[4].str() + match[6].str();
                    }
                    else
                        lex.type = ERROR;
                }
                else if (lex.type == FLOAT || lex.type == DOUBLE) {
                    if (std::regex_match(line.begin() + *offset, line.end(), match, _reg_float)) {
                        if (line.back() == '\n')
                            *offset -= 1;
                        *offset += match.length();
                        lex.value = match[1].str();
                        lex.value += (match[4].matched) ? match[4].str() : match[3].str();
                        lex.value += match[5].str() + match[7].str();
                    }
                    else
                        lex.type = ERROR;
                }
                else {
                    if (lex.type == END && (*offset - match.length()))
                        lex.type = COMMENT;
                    lex.value = match[i].str();
                }
                return (lex);
            }
        }
    }
    return (lex);
}

void                    Lexer::exec() {
    
    size_t              offset;
    lexeme              lex;
    std::string         line;
    std::istream        &istr = (_inputFromFile) ? _filestream : std::cin;

    while (std::getline(istr, line)) {
        offset = 0;
        if (_inputFromFile && istr.peek() != EOF)
            line.append("\n");
        while ((lex = match(line, &offset)).type) {
            if (lex.type == END) {
                if (!_inputFromFile)
                    return ;
                _lexemes.push_back({ERROR, ""});
            }
            else {
                _lexemes.push_back(lex);
                if (lex.type == EOL)
                    break;
            }
        }
        if (!_inputFromFile)
            _lexemes.push_back({EOL, "\n"});
    }
    if (_inputFromFile)
        _filestream.close();

}

std::list<lexeme> const &Lexer::getLexemes() const { return (_lexemes); }

Lexer::~Lexer() {}