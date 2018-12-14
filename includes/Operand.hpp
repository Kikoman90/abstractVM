/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Operand.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/07 11:12:40 by fsidler           #+#    #+#             */
/*   Updated: 2018/12/14 20:17:54 by fsidler          ###   ########.fr       */
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
            long long val = static_cast<long long>(_value) + std::stoll(rhs.toString());
            if (check_overflow<long long>(val, type))
                throw AVMException("error: integer addition leads to overflow");
            strs << std::setprecision(precision) << val;
        }
        else {
            long double val = static_cast<long double>(_value) + std::stold(rhs.toString());
            if (check_overflow<long double>(val, type))
                throw AVMException("error: floating point addition leads to overflow");
            if (check_underflow<long double>(val, type))
                throw AVMException("error: floating point addition leads to underflow");
            strs << std::setprecision(precision) << val;
        }
        return _opf->createOperand(type, strs.str());

    }

    IOperand const      *operator-(IOperand const &rhs) const {
        
        std::stringstream   strs;

        int precision = (_precision >= rhs.getPrecision()) ? _precision : rhs.getPrecision();
        eOperandType type = (_type >= rhs.getType()) ? _type : rhs.getType();
        if (type < Float) {
            long long val = static_cast<long long>(_value) - std::stoll(rhs.toString());
            if (check_overflow<long long>(val, type))
                throw AVMException("error: integer substraction leads to overflow");
            strs << std::setprecision(precision) << val;
        }
        else {
            long double val = static_cast<long double>(_value) - std::stold(rhs.toString());
            if (check_overflow<long double>(val, type))
                throw AVMException("error: floating point substraction leads to overflow");
            if (check_underflow<long double>(val, type))
                throw AVMException("error: floating point substraction leads to underflow");
            strs << std::setprecision(precision) << val;
        }
        return _opf->createOperand(type, strs.str());

    }

    IOperand const      *operator*(IOperand const &rhs) const {
        
        std::stringstream   strs;

        int precision = (_precision >= rhs.getPrecision()) ? _precision : rhs.getPrecision();
        eOperandType type = (_type >= rhs.getType()) ? _type : rhs.getType();
        if (type < Float) {
            long long val = static_cast<long long>(_value) * std::stoll(rhs.toString());
            if (check_overflow<long long>(val, type))
                throw AVMException("error: integer multiplication leads to overflow");
            strs << std::setprecision(precision) << val;
        }
        else {
            long double val = static_cast<long double>(_value) * std::stold(rhs.toString());
            if (check_overflow<long double>(val, type))
                throw AVMException("error: floating point multiplication leads to overflow");
            if (check_underflow<long double>(val, type))
                throw AVMException("error: floating point multiplication leads to underflow");
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
            long long val = static_cast<long long>(_value) / std::stoll(rhs.toString());
            if (check_overflow<long long>(val, type))
                throw AVMException("error: integer division leads to overflow");
            strs << std::setprecision(precision) << val;
        }
        else {
            long double val = static_cast<long double>(_value) / std::stold(rhs.toString());
            if (check_overflow<long double>(val, type))
                throw AVMException("error: floating point division leads to overflow");
            if (check_underflow<long double>(val, type))
                throw AVMException("error: floating point division leads to underflow");
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
            long long val = static_cast<long long>(_value) % std::stoll(rhs.toString());
            if (check_overflow<long long>(val, type))
                throw AVMException("error: integer modulo leads to overflow");
            strs << std::setprecision(precision) << val;
        }
        else {
            long double val = fmodl(static_cast<long double>(_value), std::stold(rhs.toString()));
            if (check_overflow<long double>(val, type))
                throw AVMException("error: floating point modulo leads to overflow");
            if (check_underflow<long double>(val, type))
                throw AVMException("error: floating point modulo leads to underflow");
            strs << std::setprecision(precision) << val;
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
    static bool         check_underflow(__unused U v, eOperandType type) {

        (void)v;
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