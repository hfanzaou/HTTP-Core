/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajana <ajana@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/05 16:41:53 by ajana             #+#    #+#             */
/*   Updated: 2023/06/21 08:07:20 by ajana            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"
#include "Location.hpp"

Config::Config(const std::string& file) : _config(file), servers()
{
	if (!_config.is_open())
		throw std::runtime_error("Error: Could not open file!");
}

Location	parseLocationContext(std::istringstream& lineStream, std::string& context)
{
    std::istringstream	locationStream(context);
    std::string			line;
	Location			location;

	location.setPath(lineStream);
    while (std::getline(locationStream, line)) {
		if (line.find("}") != std::string::npos) {
            break;
        }
		if (line.empty())
			continue ;
        std::istringstream	lineStream(line);
		std::string	token;
		lineStream >> token;

		if (token == "methods") {
			location.addMethod(lineStream);
		} else if (token == "root") {
			location.setRoot(lineStream);
		} else if (token == "index") {
			location.setIndex(lineStream);
		} else if (token == "autoindex") {
			location.setAutoIndex(lineStream);
		}
		else if (token == "{" || token == "}" || token.empty())
			continue ;
		else {
			throw std::runtime_error("Invalid identifier inside location context!");
		}
	}
	return (location);
}


ServerConfig parseServerContext(std::string& context)
{
	ServerConfig server;
    std::string line;
	std::istringstream	ss(context);

    while (std::getline(ss, line)) {

        std::istringstream lineStream(line);
        std::string token;
        lineStream >> token;

        if (token == "host") {
            server.setHost(lineStream);
        } else if (token == "port") {
            server.setPort(lineStream);
        } else if (token == "server_name") {
            server.addServerName(lineStream);
        } else if (token == "max_body_size") {
            server.setMaxBodySize(lineStream);
        } else if (token == "error_page") {
            server.addErrorPage(lineStream);
		}
		else if (token == "location") {
			std::string locationBlock;
            std::getline(ss, locationBlock, '}');
			Location	loc = parseLocationContext(lineStream, locationBlock);
			server.addLocation(loc);
		}
		else if (token == "{" || token == "}" || token.empty())
			continue ;
		else {
			std::cout << token << std::endl;
			throw std::runtime_error("Invalid identifier inside server context!");
		}
	}
    return server;
}

void removeSpaces(std::string &config)
{
	size_t	i;
	for (i = 0; config[i] && isspace(config[i]); i++);
	config = config.substr(i);

	for (i = config.length() - 1; i > 0 && isspace(config[i]); i--);
	config = config.substr(0, i + 1);
}

size_t scopeStart(size_t start, std::string &content)
{
	size_t i = 0;

	for (i = start; content[i]; i++)
	{
		if (content[i] == 's')
			break ;
		if (!isspace(content[i]))
			throw  std::runtime_error("Invalid character out server scope!");
	}
	if (!content[i])
		return (start);
	if (content.compare(0, 6, "server") != 0)
		throw std::runtime_error("Invalid character out server scope!");
	i += 6;
	while (content[i] && isspace(content[i]))
		i++;
	if (content[i] == '{')
		return (i);
	else
		throw  std::runtime_error("Invalid character out server scope!");

}

size_t scopeEnd(size_t start, std::string &content)
{
	size_t	i, scope = 0;
	for (i = start + 1; content[i]; i++)
	{
		if (content[i] == '{')
			scope++;
		else if (content[i] == '}')
		{
			if (!scope)
				return (i);
			scope--;
		}
	}
	return (start);
}

void Config::splitServers(std::string &config)
{
	size_t start = 0, end = 1;

	if (config.find("server", 0) == std::string::npos)
		throw std::runtime_error("Error: No Server context!");

	while (start != end && start < config.length())
	{
		start = scopeStart(start, config);
		end = scopeEnd(start, config);
		if (start == end)
			throw std::runtime_error("Error!");
		serverBlocks.push_back(config.substr(start, end - start + 1));
		start = end + 1;
	}
}

void	Config::parse()
{
	std::stringstream ss;
	ss << _config.rdbuf();
	std::string content(ss.str());

	removeSpaces(content);
	splitServers(content);
	for (size_t i = 0; i < serverBlocks.size(); ++i) {
		servers.push_back(parseServerContext(serverBlocks[i]));
	}
}

std::vector<ServerConfig>&	Config::getServers() { return servers; }

void	Config::printServers() const
{
	for (size_t i = 0; i < servers.size(); ++i) {
		servers[i].printServer();
	}
}