/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   OpFactory.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/07 14:47:54 by fsidler           #+#    #+#             */
/*   Updated: 2018/12/14 15:40:31 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OP_FACTORY_HPP
#define OP_FACTORY_HPP

#include "IOperand.hpp"
#include <array>
#include <map>

class OpFactory {

public:

    OpFactory();
    ~OpFactory();

    IOperand const  *createOperand(eOperandType type, std::string const &value) const;

    static std::map<eOperandType, std::string>  _opMap;

private:

    OpFactory(OpFactory const &src);

    OpFactory       &operator=(OpFactory const &rhs);

    IOperand const  *createInt8(std::string const &value) const;
    IOperand const  *createInt16(std::string const &value) const;
    IOperand const  *createInt32(std::string const &value) const;
    IOperand const  *createFloat(std::string const &value) const;
    IOperand const  *createDouble(std::string const &value) const;

    std::array<IOperand const *(OpFactory::*)(std::string const &) const, 5>    _tab;

};

#endif