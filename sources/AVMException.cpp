/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AVMException.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/05 17:15:10 by fsidler           #+#    #+#             */
/*   Updated: 2018/12/14 18:26:52 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "AVMException.hpp"

AVMException::AVMException() {}

AVMException::AVMException(std::string const msg) : _msg(msg) {}

AVMException::AVMException(AVMException const &src) { _msg = src._msg; }

const char          *AVMException::what() const throw() { return (_msg.c_str()); }

AVMException::~AVMException() throw() {}