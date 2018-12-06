/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/06 17:28:10 by fsidler           #+#    #+#             */
/*   Updated: 2018/12/06 19:35:36 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_HPP
#define PARSER_HPP

#include "Lexer.hpp"
#include <array>

class Parser {

public:

    Parser(const std::list<lexeme> &lexemes);
    ~Parser();

    void        exec();

private:

    Parser();
    Parser(Parser const &src);

    Parser      &operator=(Parser const &rhs);

    void        push(const std::list<lexeme>::const_iterator &it) const;
    void        pop(const std::list<lexeme>::const_iterator &it) const;
    void        dump(const std::list<lexeme>::const_iterator &it) const;
    void        assert(const std::list<lexeme>::const_iterator &it) const;
    void        add(const std::list<lexeme>::const_iterator &it) const;
    void        sub(const std::list<lexeme>::const_iterator &it) const;
    void        mul(const std::list<lexeme>::const_iterator &it) const;
    void        div(const std::list<lexeme>::const_iterator &it) const;
    void        mod(const std::list<lexeme>::const_iterator &it) const;
    void        print(const std::list<lexeme>::const_iterator &it) const;
    void        exit(const std::list<lexeme>::const_iterator &it) const;

    unsigned int            _line;
    
    const std::list<lexeme> _lexemes;

    std::array<void (Parser::*)(const std::list<lexeme>::const_iterator &) const, 11>   _operations;

};

#endif