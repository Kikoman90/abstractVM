/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AVMException.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/05 16:52:16 by fsidler           #+#    #+#             */
/*   Updated: 2018/12/06 19:57:05 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AVM_EXCEPTION_HPP
#define AVM_EXCEPTION_HPP

#include <exception>
#include <sstream>
#include <string>

class AVMException : std::exception {

public:
    
    AVMException();
    AVMException(const std::string msg, unsigned int line);
    AVMException(AVMException const &src);
    ~AVMException() throw();

    const char      *what() const throw();

    bool            empty() const;
    void            addMsg(const std::string msg, unsigned int line);

private:
    
    AVMException    &operator=(AVMException const &rhs);
    
    std::string     _msg;

};

#endif