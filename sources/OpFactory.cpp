/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   OpFactory.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/07 15:13:38 by fsidler           #+#    #+#             */
/*   Updated: 2018/12/17 11:47:35 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "OpFactory.hpp"
#include "Operand.hpp"
#include <utility>

std::pair<eOperandType, std::string> pairs[] = {std::make_pair(Int8, "int8"),
                                                std::make_pair(Int16, "int16"),
                                                std::make_pair(Int32, "int32"),
                                                std::make_pair(Float, "float"),
                                                std::make_pair(Double, "double") };

std::map<eOperandType, std::string> OpFactory::_opMap(pairs, pairs + 5);

OpFactory::OpFactory() {
    _tab[0] = &OpFactory::createInt8;
    _tab[1] = &OpFactory::createInt16;
    _tab[2] = &OpFactory::createInt32;
    _tab[3] = &OpFactory::createFloat;
    _tab[4] = &OpFactory::createDouble;
}

IOperand const  *OpFactory::createOperand(eOperandType type, std::string const &value) const {
    return ((this->*_tab.at(type - Int8))(value));
}

IOperand const  *OpFactory::createInt8(std::string const &value) const {
    return (new Operand<char>(value, 0, Int8, this));
}

IOperand const  *OpFactory::createInt16(std::string const &value) const {
    return (new Operand<short>(value, 0, Int16, this));
}

IOperand const  *OpFactory::createInt32(std::string const &value) const {
    return (new Operand<int>(value, 0, Int32, this));
}

IOperand const  *OpFactory::createFloat(std::string const &value) const {
    return (new Operand<float>(value, 7, Float, this));
}

IOperand const  *OpFactory::createDouble(std::string const &value) const {
    return (new Operand<double>(value, 14, Double, this));
}

OpFactory::~OpFactory() {}