/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Operand.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/07 11:12:40 by fsidler           #+#    #+#             */
/*   Updated: 2019/01/02 14:47:15 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OPERAND_HPP
#define OPERAND_HPP

#include "AVMException.hpp"
#include "OpFactory.hpp"
#include <iomanip>
#include <cmath>
#include <limits>
#include <climits>
#include <cfloat>

#define FLT_TRUE_MIN ((float)1.4012984643248170709237295832899161312802619419e-45)
#define DBL_TRUE_MIN ((double)4.9406564584124654417656879286822137236505980262e-324)

template<typename T>
class Operand : public IOperand {

public:

    Operand(std::string const &str, int precision, eOperandType type, OpFactory const *opf) : _precision(precision), _type(type), _opf(opf) {
    
        std::stringstream strs;

        try {
            if (type < Float) {
                long long val = std::stoll(str);
                if (check_overflow<long long>(val, type))
                    throw AVMException("error: integer overflow");
                _value = static_cast<T>(val);
                strs << std::setprecision(precision) << val;
                _str = strs.str();
            }
            else {
                long double val = std::stold(str);
                if (check_overflow<long double>(val, _type))
                    throw AVMException("error: floating point overflow");
                if (check_underflow<long double>(val, _type))
                    throw AVMException("error: floating point underflow");
                _value = static_cast<T>(val);
                strs << std::setprecision(precision) << val;
                _str = strs.str();
            }
        }
        catch (std::out_of_range &oor) {
            throw AVMException("error: overflow or underflow");
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
        if (type < Float) {
            long long val = std::stoll(toString()) + std::stoll(rhs.toString());
            if (check_overflow<long long>(val, type))
                throw AVMException("error: addition leads to integer overflow");
            strs << std::setprecision(precision) << val;
        }
        else {
            long double val = std::stold(toString()) + std::stold(rhs.toString());
            if (check_overflow<long double>(val, type))
                throw AVMException("error: addition leads to floating point overflow");
            if (check_underflow<long double>(val, type))
                throw AVMException("error: addition leads to floating point underflow");
            strs << std::setprecision(precision) << val;
        }
        return _opf->createOperand(type, strs.str());

    }

    IOperand const      *operator-(IOperand const &rhs) const {
        
        std::stringstream   strs;

        int precision = (_precision >= rhs.getPrecision()) ? _precision : rhs.getPrecision();
        eOperandType type = (_type >= rhs.getType()) ? _type : rhs.getType();
        if (type < Float) {
            long long val = std::stoll(toString()) - std::stoll(rhs.toString());
            if (check_overflow<long long>(val, type))
                throw AVMException("error: substraction leads to integer overflow");
            strs << std::setprecision(precision) << val;
        }
        else {
            long double val = std::stold(toString()) - std::stold(rhs.toString());
            if (check_overflow<long double>(val, type))
                throw AVMException("error: substraction leads to floating point overflow");
            if (check_underflow<long double>(val, type))
                throw AVMException("error: substraction leads to floating point underflow");
            strs << std::setprecision(precision) << val;
        }
        return _opf->createOperand(type, strs.str());

    }

    IOperand const      *operator*(IOperand const &rhs) const {
        
        std::stringstream   strs;

        int precision = (_precision >= rhs.getPrecision()) ? _precision : rhs.getPrecision();
        eOperandType type = (_type >= rhs.getType()) ? _type : rhs.getType();
        if (type < Float) {
            long long val = std::stoll(toString()) * std::stoll(rhs.toString());
            if (check_overflow<long long>(val, type))
                throw AVMException("error: multiplication leads to integer overflow");
            strs << std::setprecision(precision) << val;
        }
        else {
            long double val = std::stold(toString()) * std::stold(rhs.toString());
            if (check_overflow<long double>(val, type))
                throw AVMException("error: multiplication leads to floating point overflow");
            if (check_underflow<long double>(val, type))
                throw AVMException("error: multiplication leads to floating point underflow");
            strs << std::setprecision(precision) << val;
        }
        return _opf->createOperand(type, strs.str());

    }

    IOperand const      *operator/(IOperand const &rhs) const {
        
        std::stringstream   strs;

        int precision = (_precision >= rhs.getPrecision()) ? _precision : rhs.getPrecision();
        eOperandType type = (_type >= rhs.getType()) ? _type : rhs.getType();
        if (std::stold(rhs.toString()) == 0)
            throw AVMException("error: division by zero");
        if (type < Float) {
            long long val = std::stoll(toString()) / std::stoll(rhs.toString());
            if (check_overflow<long long>(val, type))
                throw AVMException("error: division leads to integer overflow");
            strs << std::setprecision(precision) << val;
        }
        else {
            long double val = std::stold(toString()) / std::stold(rhs.toString());
            if (check_overflow<long double>(val, type))
                throw AVMException("error: division leads to floating point overflow");
            if (check_underflow<long double>(val, type))
                throw AVMException("error: division leads to floating point underflow");
            strs << std::setprecision(precision) << val;
        }
        return _opf->createOperand(type, strs.str());

    }

    IOperand const      *operator%(IOperand const &rhs) const {
        
        std::stringstream   strs;

        int precision = (_precision >= rhs.getPrecision()) ? _precision : rhs.getPrecision();
        eOperandType type = (_type >= rhs.getType()) ? _type : rhs.getType();
        if (std::stold(rhs.toString()) == 0)
            throw AVMException("error: modulo by zero");
        if (type < Float) {
            long long val = std::stoll(toString()) % std::stoll(rhs.toString());
            if (check_overflow<long long>(val, type))
                throw AVMException("error: modulo leads to integer overflow");
            strs << std::setprecision(precision) << val;
        }
        else {
            long double val = fmodl(std::stold(toString()), std::stold(rhs.toString()));
            if (check_overflow<long double>(val, type))
                throw AVMException("error: modulo leads to floating point overflow");
            if (check_underflow<long double>(val, type))
                throw AVMException("error: modulo leads to floating point underflow");
            strs << std::setprecision(precision) << val;
        }
        return _opf->createOperand(type, strs.str());

    }

    IOperand const      *operator^(IOperand const &rhs) const {
    
        std::stringstream   strs;
        eOperandType        type = getType();
        int                 precision = getPrecision();
        long long           exponent = std::stoll(rhs.toString());

        if (exponent < 0)
            throw AVMException("error: exponentiation only works with a positive exponent. yes im lazy");
        else if (!exponent)
            strs << std::setprecision(precision) << 1;
        else {
            if (type < Float) {
                long long v = std::stoll(toString());
                long long val = v;
                for (long long i = 1; i < exponent; ++i) {
                    val *= v;
                    if (check_overflow<long long>(val, type))
                        throw AVMException("error: exponentiation leads to integer overflow");
                }
                strs << std::setprecision(precision) << val;
            }
            else {
                long double v = std::stold(toString());
                long double val = v;
                for (long long i = 1; i < exponent; ++i) {
                    val *= v;
                    if (check_overflow<long double>(val, type))
                        throw AVMException("error: exponentiation leads to floating point overflow");
                    if (check_underflow<long double>(val, type))
                        throw AVMException("error: exponentiation leads to floating point underflow");
                }
                strs << std::setprecision(precision) << val;
            }
        }
        return _opf->createOperand(type, strs.str());

    }

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
                return ((v > 0 && v < FLT_TRUE_MIN) || (v < 0 && v > -FLT_TRUE_MIN));
            case Double:
                return ((v > 0 && v < DBL_TRUE_MIN) || (v < 0 && v > -DBL_TRUE_MIN));
            default:
                break;
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