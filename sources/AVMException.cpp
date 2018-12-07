/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AVMException.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/05 17:15:10 by fsidler           #+#    #+#             */
/*   Updated: 2018/12/07 19:09:27 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "AVMException.hpp"

AVMException::AVMException() {}

AVMException::AVMException(std::string const msg, unsigned int line) { addMsg(msg, line); }

AVMException::AVMException(AVMException const &src) { _msg = src._msg; }

const char          *AVMException::what() const throw() { return (_msg.c_str()); }

bool                AVMException::empty() const { return (_msg.empty()); }

void                AVMException::addMsg(std::string const msg, unsigned int line) {

    std::stringstream   cat;

    if (!_msg.empty())
        _msg += "\n";
    if (line == 0)
        _msg += msg;
    else {
        cat << msg << " (line " << line << ")";
        _msg += cat.str();
    }

}

std::string const   &AVMException::getMsg() const { return (_msg); }

AVMException::~AVMException() throw() {}