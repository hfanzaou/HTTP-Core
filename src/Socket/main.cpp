/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hfanzaou <hfanzaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/08 22:12:15 by ebensalt          #+#    #+#             */
/*   Updated: 2023/06/23 01:47:14 by hfanzaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Socket.hpp"
#include "../config/ConfigParse/Config.hpp"
int	main(int ac, char **av)
{
	(void)ac;
	Socket	server;
	try {
		Config	config(av[1]);
		config.parse();
		config.printServers();
		server.start_server(config.getServers().at(0));
	}
	catch (std::exception &e)
	{
		std::cerr << e.what() << std::endl;
	}
}
