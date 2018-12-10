/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Operand.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/07 11:12:40 by fsidler           #+#    #+#             */
/*   Updated: 2018/12/10 21:47:56 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OPERAND_HPP
#define OPERAND_HPP

#include "AVMException.hpp"
#include "IOperand.hpp"
#include <iomanip>
#include <float.h>
#include <climits>

class OpFactory;

template<typename T>
class Operand : public IOperand {

public:

    Operand(std::string const &str, int precision, eOperandType type, OpFactory const *opf) : _precision(precision), _type(type), _opf(opf) {
    
        std::stringstream strs;

        try {
            if (type < Float) {
                long long val = std::stoll(str);
                if (check_overflow<long long>(val, type))
                    throw AVMException("parser error: integer overflow");
                _value = static_cast<T>(val);
                strs << std::setprecision(precision) << val;
                _str = strs.str();
            }
            else {
                long double val = std::stold(str);
                if (check_overflow<long double>(val, _type))
                    throw AVMException("parser error: floating point overflow");
                if (check_underflow<long double>(val, _type))
                    throw AVMException("parser error: floating point underflow");
                _value = static_cast<T>(val);
                strs << std::setprecision(precision) << val;
                _str = strs.str();
            }
        }
        catch (std::out_of_range &oor) {
            throw AVMException("parser error: overflow or underflow");
        }
    
    }

    ~Operand() {}

    int                 getPrecision(void) const { return (_precision); }

    eOperandType        getType(void) const { return (_type); }

    T                   getValue(void) const { return (_value); }

    IOperand const      *operator+(IOperand const &rhs) const {
        
        std::stringstream   strs;

        int precision = (_precision >= rhs.getPrecision()) ? _precision : rhs.getPrecision();
        eOperandType type = (_type >= rhs.getType()) ? _type : rhs.getType();
        try {
            if (type < Float) {
                long long val = static_cast<long long>(_value) + std::stoll(rhs.toString());
                if (check_overflow<long long>(val, type))
                    throw AVMException("parser error: integer addition leads to overflow"); // ADD TYPE (INT_MAX or CHAR_MAX, SHORT_MIN etc.)
                strs << std::setprecision(precision) << val;
            }
            else {
                long double val = static_cast<long double>(_value) + std::stold(rhs.toString());
                if (check_overflow<long double>(val, type))
                    throw AVMException("parser error: floating point addition leads to overflow"); // ADD TYPE (DBL_MAX, DBL_MIN, FLOAT_MAX etc.)
                if (check_underflow<long double>(val, type))
                    throw AVMException("parser error: floating point addition leads to underflow"); // ADD TYPE (DBL_MAX, DBL_MIN, FLOAT_MAX etc.)
                strs << std::setprecision(precision) << val;
            }
            return _opf->createOperand(type, strs.str());
        }
        catch (std::out_of_range &e) {
            throw AVMException("parser error: overflow or underflow");
        }
        return (NULL);

    }

    IOperand const      *operator-(IOperand const &rhs) const {
        
        std::stringstream   strs;

        int precision = (_precision >= rhs.getPrecision()) ? _precision : rhs.getPrecision();
        eOperandType type = (_type >= rhs.getType()) ? _type : rhs.getType();
        try {
            if (type < Float) {
                long long val = static_cast<long long>(_value) - std::stoll(rhs.toString());
                if (check_overflow<long long>(val, type))
                    throw AVMException("parser error: integer substraction leads to overflow"); // ADD TYPE (INT_MAX or CHAR_MAX, SHORT_MIN etc.)
                strs << std::setprecision(precision) << val;
            }
            else {
                long double val = static_cast<long double>(_value) - std::stold(rhs.toString());
                if (check_overflow<long double>(val, type))
                    throw AVMException("parser error: floating point substraction leads to overflow"); // ADD TYPE (DBL_MAX, DBL_MIN, FLOAT_MAX etc.)
                if (check_underflow<long double>(val, type))
                    throw AVMException("parser error: floating point substraction leads to underflow"); // ADD TYPE (DBL_MAX, DBL_MIN, FLOAT_MAX etc.)
                strs << std::setprecision(precision) << val;
            }
            return _opf->createOperand(type, strs.str());
        }
        catch (std::out_of_range &e) {
            throw AVMException("parser error: overflow or underflow");
        }
        return (NULL);

    }

    IOperand const      *operator*(IOperand const &rhs) const {
        
        std::stringstream   strs;

        int precision = (_precision >= rhs.getPrecision()) ? _precision : rhs.getPrecision();
        eOperandType type = (_type >= rhs.getType()) ? _type : rhs.getType();
        try {
            if (type < Float) {
                long long val = static_cast<long long>(_value) * std::stoll(rhs.toString());
                if (check_overflow<long long>(val, type))
                    throw AVMException("parser error: integer multiplication leads to overflow"); // ADD TYPE (INT_MAX or CHAR_MAX, SHORT_MIN etc.)
                strs << std::setprecision(precision) << val;
            }
            else {
                long double val = static_cast<long double>(_value) - std::stold(rhs.toString());
                if (check_overflow<long double>(val, type))
                    throw AVMException("parser error: floating point multiplication leads to overflow"); // ADD TYPE (DBL_MAX, DBL_MIN, FLOAT_MAX etc.)
                if (check_underflow<long double>(val, type))
                    throw AVMException("parser error: floating point multiplication leads to underflow"); // ADD TYPE (DBL_MAX, DBL_MIN, FLOAT_MAX etc.)
                strs << std::setprecision(precision) << val;
            }
            return _opf->createOperand(type, strs.str());
        }
        catch (std::out_of_range &e) {
            throw AVMException("parser error: overflow or underflow");
        }
        return (NULL);

    }

    // CHECK FOR DIVISION BY ZERO
    IOperand const      *operator/(IOperand const &rhs) const {
        
        std::stringstream   strs;

        int precision = (_precision >= rhs.getPrecision()) ? _precision : rhs.getPrecision();
        eOperandType type = (_type >= rhs.getType()) ? _type : rhs.getType();
        try {
            if (type < Float) {
                long long val = static_cast<long long>(_value) / std::stoll(rhs.toString());
                if (check_overflow<long long>(val, type))
                    throw AVMException("parser error: integer division leads to overflow"); // ADD TYPE (INT_MAX or CHAR_MAX, SHORT_MIN etc.)
                strs << std::setprecision(precision) << val;
            }
            else {
                long double val = static_cast<long double>(_value) / std::stold(rhs.toString());
                if (check_overflow<long double>(val, type))
                    throw AVMException("parser error: floating point division leads to overflow"); // ADD TYPE (DBL_MAX, DBL_MIN, FLOAT_MAX etc.)
                if (check_underflow<long double>(val, type))
                    throw AVMException("parser error: floating point division leads to underflow"); // ADD TYPE (DBL_MAX, DBL_MIN, FLOAT_MAX etc.)
                strs << std::setprecision(precision) << val;
            }
            return _opf->createOperand(type, strs.str());
        }
        catch (std::out_of_range &e) {
            throw AVMException("parser error: overflow or underflow");
        }
        return (NULL);

    }

    // CHECK FOR MODULO BY ZERO
    IOperand const      *operator%(IOperand const &rhs) const {
        
        std::stringstream   strs;

        int precision = (_precision >= rhs.getPrecision()) ? _precision : rhs.getPrecision();
        eOperandType type = (_type >= rhs.getType()) ? _type : rhs.getType();
        try {
            if (type < Float) {
                long long val = static_cast<long long>(_value) % std::stoll(rhs.toString());
                if (check_overflow<long long>(val, type))
                    throw AVMException("parser error: integer modulo leads to overflow"); // ADD TYPE (INT_MAX or CHAR_MAX, SHORT_MIN etc.)
                strs << std::setprecision(precision) << val;
            }
            else {
                long double val = static_cast<long double>(_value) % std::stold(rhs.toString());
                if (check_overflow<long double>(val, type))
                    throw AVMException("parser error: floating point modulo leads to overflow"); // ADD TYPE (DBL_MAX, DBL_MIN, FLOAT_MAX etc.)
                if (check_underflow<long double>(val, type))
                    throw AVMException("parser error: floating point modulo leads to underflow"); // ADD TYPE (DBL_MAX, DBL_MIN, FLOAT_MAX etc.)
                strs << std::setprecision(precision) << val;
            }
            return _opf->createOperand(type, strs.str());
        }
        catch (std::out_of_range &e) {
            throw AVMException("parser error: overflow or underflow");
        }
        return (NULL);

    }

/*
    template<typename U>
    bool                check_addition_overflow(U v1, U v2, eOperandType type) {

        switch (type) {
            case Int8:
                return ((v2 > 0 && v1 > SCHAR_MAX - v2) || (v2 < 0 && v1 < SCHAR_MIN - v2));
            case Int16:
                return ((v2 > 0 && v1 > SHRT_MAX - v2) || (v2 < 0 && v1 < SHRT_MIN - v2));
            case Int32:
                return ((v2 > 0 && v1 > INT_MAX - v2) || (v2 < 0 && v1 < INT_MIN - v2));
            case Float:
                return ((v2 > 0 && v1 > FLT_MAX - v2) || (v2 < 0 && v1 < FLT_MIN - v2));
            case Double:
                return ((v2 > 0 && v1 > DBL_MAX - v2) || (v2 < 0 && v1 < DBL_MIN - v2));
        }
        return (true);

    }

    template<typename U>
    bool                check_substraction_overflow(U v1, U v2, eOperandType type) {

        switch (type) {
            case Int8:
                return ((v2 < 0 && v1 > SCHAR_MAX + v2) || (v2 > 0 && v1 < SCHAR_MIN + v2));
            case Int16:
                return ((v2 < 0 && v1 > SHRT_MAX + v2) || (v2 > 0 && v1 < SHRT_MIN + v2));
            case Int32:
                return ((v2 < 0 && v1 > INT_MAX + v2) || (v2 > 0 && v1 < INT_MIN + v2));
            case Float:
                return ((v2 < 0 && v1 > FLT_MAX + v2) || (v2 > 0 && v1 < FLT_MIN + v2));
            case Double:
                return ((v2 < 0 && v1 > DBL_MAX + v2) || (v2 > 0 && v1 < DBL_MIN + v2));
        }
        return (true);

    }
*/

    std::string const   &toString(void) const { return (_str); } 

private:

    Operand();
    Operand(Operand const &src);

    Operand             &operator=(Operand const &rhs);

    template<typename U>
    static bool         check_overflow(U v, eOperandType type) {

        switch (type) {
            case Int8:
                return (v > SCHAR_MAX || v < SCHAR_MIN);
            case Int16:
                return (v > SHRT_MAX || v < SHRT_MIN);
            case Int32:
                return (v > INT_MAX || v < INT_MIN);
            case Float:
                return (v > FLT_MAX || v < -FLT_MAX);
            case Double:
                return (v > DBL_MAX || v < -DBL_MAX);
        }
        return (true);

    }

    template<typename U>
    static bool         check_underflow(U v, eOperandType type) {

        switch (type) {
            case Float:
                return (v < FLT_TRUE_MIN);
            case Double:
                return (v < DBL_TRUE_MIN);
        }
        return (true);

    }

    T               _value;
    std::string     _str;
    int             _precision;
    eOperandType    _type;
    OpFactory const *_opf;

};

#endif