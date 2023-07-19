/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajana <ajana@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/08 22:17:38 by ajana             #+#    #+#             */
/*   Updated: 2023/07/14 07:15:36 by ajana            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerConfig.hpp"

ServerConfig::ServerConfig() : MaxBodySize(-1)
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
	int	_port;
	if (!(lineStream >> _port) || !lineStream.eof()
		|| _port <= 1024 || _port > 65536)
		throw std::runtime_error("Invalid port");
	port = std::to_string(_port);
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
		throw std::runtime_error("Invalid error page path");
	}
	err_pages[err_num] = err_page;
}

void	ServerConfig::checkServer() const
{
	if (port.empty() || host.empty())
		throw std::runtime_error("Server port or host is missing!");
}

const std::string& ServerConfig::getHost() const { return host; }

const std::string&	ServerConfig::getPort() const { return port; }

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
