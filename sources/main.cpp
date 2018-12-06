/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/05 14:53:14 by fsidler           #+#    #+#             */
/*   Updated: 2018/12/06 19:00:28 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parser.hpp"
// #include "Operand.hpp"
// #include "OperandFactory.hpp"

int     main(int argc, char **argv)
{
    try {
        Lexer lexer(argc, argv);
        lexer.exec();
        lexer.showLexemes();
        Parser parser(lexer.getLexemes());
        parser.exec();
    }
    catch (AVMException &e) {
        std::cout << e.what() << std::endl;
    }
    catch (std::exception &e) {
        std::cout << e.what() << std::endl;
    }

}