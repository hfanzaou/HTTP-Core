/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hfanzaou <hfanzaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/08 21:58:22 by ajana             #+#    #+#             */
/*   Updated: 2023/06/27 02:01:37 by hfanzaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef	SERVERCONFIG_HPP
#define SERVERCONFIG_HPP

#include "Location.hpp"
#include <map>
#include <unistd.h>

class ServerConfig
{
	private :
		std::string					host;
		unsigned int				port;
		std::vector<std::string>	server_names;
		unsigned long				MaxBodySize;
		std::vector<Location>		locations;
		std::map<short, std::string> err_pages;
	public :
		ServerConfig();

		void	setHost(std::istringstream& lineStream);
		void	setPort(std::istringstream& lineStream);
		void	addServerName(std::istringstream& lineStream);
		void	setMaxBodySize(std::istringstream& lineStream);
		void	addLocation(Location& loc);
		void	addErrorPage(std::istringstream& lineStream);

		const std::string&				getHost() const;
		short							getPort() const;
		const std::vector<std::string>&	getServerNames() const;
		unsigned long					getMaxBodySize() const;
		const std::vector<Location>&	getLocations() const;
		const std::map<short, std::string>& getErrorPages() const;

		void	printServer() const;
};

std::ostream&	operator<<(std::ostream out, ServerConfig& server);

#endif