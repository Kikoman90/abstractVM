/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AVMException.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/05 17:15:10 by fsidler           #+#    #+#             */
/*   Updated: 2018/12/06 20:01:13 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "AVMException.hpp"

AVMException::AVMException() {}

AVMException::AVMException(const std::string msg, unsigned int line) { addMsg(msg, line); }

AVMException::AVMException(AVMException const &src) { _msg = src._msg; }

const char  *AVMException::what() const throw() { return (_msg.c_str()); }

bool        AVMException::empty() const { return (_msg.empty()); }

void        AVMException::addMsg(const std::string msg, unsigned int line) {

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

AVMException::~AVMException() throw() {}