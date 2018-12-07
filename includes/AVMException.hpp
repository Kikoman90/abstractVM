/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AVMException.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/05 16:52:16 by fsidler           #+#    #+#             */
/*   Updated: 2018/12/07 19:38:39 by fsidler          ###   ########.fr       */
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
    AVMException(std::string const msg, unsigned int line = 0);
    AVMException(AVMException const &src);
    ~AVMException() throw();

    const char          *what() const throw();

    bool                empty() const;
    void                addMsg(std::string const msg, unsigned int line = 0);
    std::string const   &getMsg() const;

private:
    
    AVMException        &operator=(AVMException const &rhs);
    
    std::string _msg;

};

#endif