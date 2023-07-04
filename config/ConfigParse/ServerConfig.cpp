/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajana <ajana@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/08 22:17:38 by ajana             #+#    #+#             */
/*   Updated: 2023/06/21 02:32:12 by ajana            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerConfig.hpp"

ServerConfig::ServerConfig() : host(""), port(0), MaxBodySize(0)
{}

bool	isValidHost(const std::string& host)
{
	std::istringstream	ss(host);
	std::string			tmp;
	int					count = 0;
	while (std::getline(ss, tmp , '.')) {
		count++;
		for (int i = 0; tmp[i]; i++)
		{
			if (!std::isdigit(tmp[i]))
				return false;
		}
		int value = std::atoi(tmp.c_str());
		if (value < 0 || value > 255)
			return false;
	}
	return (count == 4);
}

void	ServerConfig::setHost(std::istringstream& lineStream) {
	if(!(lineStream >> host) || !lineStream.eof() || !isValidHost(host))
		throw std::runtime_error("Invalid host");
}

void	ServerConfig::setPort(std::istringstream& lineStream) 
{
	if (!(lineStream >> port) || !lineStream.eof()
		|| port <= 1024 || port > 65536)
		throw std::runtime_error("Invalid port");
}

void	ServerConfig::addServerName(std::istringstream& lineStream)
{
	std::string	name;
	while (lineStream.good()) {
		lineStream >> name;
		server_names.push_back(name);
	}
}

void	ServerConfig::setMaxBodySize(std::istringstream& lineStream) {
	if (!(lineStream >> MaxBodySize) || !(lineStream.eof()))
		throw std::runtime_error("Invalid max body size");
}

void	ServerConfig::addLocation(Location& loc) {
	locations.push_back(loc);
}

void	ServerConfig::addErrorPage(std::istringstream& lineStream)
{
	short		err_num;
	std::string	err_page;
	if (!(lineStream >> err_num >> err_page) || !(lineStream.eof()))
		throw std::runtime_error("Invalid error page");
	else if (err_num < 100 || err_num > 599)
		throw std::runtime_error("Invalid error code");
	else if (access(err_page.c_str(), R_OK) == -1) {
		std::cout << err_page << std::endl;
		throw std::runtime_error("Invalid error page path");
	}
	err_pages[err_num] = err_page;
}

const std::string& ServerConfig::getHost() const { return host; }

short	ServerConfig::getPort() const { return port; }

const std::vector<std::string>&	ServerConfig::getServerNames() const { return server_names; }

unsigned long	ServerConfig::getMaxBodySize() const { return MaxBodySize; }

const std::vector<Location>&	ServerConfig::getLocations() const { return locations; }

const std::map<short, std::string>& ServerConfig::getErrorPages() const { return err_pages; }

void	ServerConfig::printServer() const
{
    std::cout << "Host: " << host << std::endl;
	std::cout << "Port: " << port << std::endl;

	std::cout << "Server Names:";
	for (size_t i = 0; i < server_names.size(); ++i) {
		std::cout << " " << server_names[i];
	}
	std::cout << std::endl;

    std::cout << "Max Body Size: " << MaxBodySize << std::endl;

    std::cout << "Error Pages:" << std::endl;
	std::map<short, std::string>::const_iterator it = err_pages.begin();
	for (; it != err_pages.end(); ++it) {
		std::cout << "	- Error Code: " << it->first << ", Page: " << it->second << std::endl;
	}

	for (size_t i = 0; i < locations.size(); ++i) {
		locations[i].printLocation();
	}
}
