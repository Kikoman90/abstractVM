/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IOperand.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/04 18:41:47 by fsidler           #+#    #+#             */
/*   Updated: 2018/12/13 18:49:36 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef I_OPERAND_HPP
#define I_OPERAND_HPP

#include <string>

enum eOperandType {
	Int8,
	Int16,
	Int32,
	Float,
	Double
};

class IOperand {

public:

	virtual int					getPrecision(void) const = 0;
	virtual eOperandType		getType(void) const = 0;

	virtual IOperand const		*operator+(IOperand const &rhs) const = 0;
	virtual IOperand const		*operator-(IOperand const & rhs) const = 0;
	virtual IOperand const		*operator*(IOperand const & rhs) const = 0;
	virtual IOperand const		*operator/(IOperand const &rhs) const = 0;
	virtual IOperand const		*operator%(IOperand const &rhs) const = 0;

	virtual std::string const	&toString(void) const = 0;

	virtual ~IOperand(void) {}

};

#endif
