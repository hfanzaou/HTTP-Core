/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajana <ajana@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/08 22:00:29 by ajana             #+#    #+#             */
/*   Updated: 2023/07/14 07:01:06 by ajana            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef	CONFIG_HPP
#define CONFIG_HPP

#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <exception>
#include <iostream>
#include <sstream>
#include <sys/stat.h>
#include "ServerConfig.hpp"

class Config
{
	private :
		std::ifstream						_config;
		std::vector<ServerConfig>			servers;
		std::vector<std::string>			serverBlocks;
	public :
		Config(const std::string& file);
		void	parse();
		void	splitServers(std::string& config);

		std::vector<ServerConfig>& getServers();
		void	printServers() const;
};

#endif
