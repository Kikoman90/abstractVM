/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Operand.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/07 11:12:40 by fsidler           #+#    #+#             */
/*   Updated: 2018/12/07 19:23:23 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OPERAND_HPP
#define OPERAND_HPP

#include "IOperand.hpp"

class OpFactory;

template<typename T>
class Operand : public IOperand {

public:

    Operand(std::string const &str, int precision, eOperandType type, OpFactory const &opf) : _str(str), _precision(precision), _type(type), _opf(opf) {
    
        T lol;
        (void)lol;
    
    }
    ~Operand() {}

    int                 getPrecision(void) const { return (_precision); }
    eOperandType        getType(void) const { return (_type); }

    IOperand const      *operator+(IOperand const &rhs) const {
        //int precision = (_precision >= rhs._precision) ? _precision : rhs._precision;
        //eOperandType type = (_type >= rhs._type) ? _type : rhs._type;
        (void)rhs;
        return (NULL);
    }

    IOperand const      *operator-(IOperand const &rhs) const {
        (void)rhs;
        return (NULL);
    }

    IOperand const      *operator*(IOperand const &rhs) const {
        (void)rhs;
        return (NULL);
    }

    IOperand const      *operator/(IOperand const &rhs) const {
        (void)rhs;
        return (NULL);
    }

    IOperand const      *operator%(IOperand const &rhs) const {
        (void)rhs;
        return (NULL);
    }

    std::string const   &toString(void) const { return (_str); }

    

private:

    Operand();
    Operand(Operand const &src);

    Operand             &operator=(Operand const &rhs);

    T               _value;
    std::string     _str;
    int             _precision;
    eOperandType    _type;
    OpFactory const &_opf;

};

#endif