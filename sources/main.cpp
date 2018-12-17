/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <fsidler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/05 14:53:14 by fsidler           #+#    #+#             */
/*   Updated: 2018/12/17 19:33:32 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parser.hpp"

int     main(int argc, char **argv)
{

    try {
        Lexer lexer(argc, argv);
        lexer.exec();
        lexer.showTokens();
        //Parser parser(lexer.getLexemes());
        //parser.exec();
    }
    catch (AVMException &e) {
        std::cout << e.what() << std::endl;
    }
    catch (std::exception &e) {
        std::cout << e.what() << std::endl;
    }
    /*float bo = 3.f;
    float ba = .3;
    float e = 3.4e2;
    float er = E2f; //
    float w = .5e2f;*/
}