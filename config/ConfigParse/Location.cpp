/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajana <ajana@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/08 22:26:31 by ajana             #+#    #+#             */
/*   Updated: 2023/06/21 02:10:19 by ajana            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"

void	Location::setPath(std::istringstream& lineStream)
{
	if (!(lineStream >> path) || path[0] != '/')
		throw std::runtime_error("Invalid location path");
}

void	Location::setRoot(std::istringstream& lineStream)
{
	if (!(lineStream >> root) || (!lineStream.eof()) || (!opendir(root.c_str())))
		throw std::runtime_error("Invalid location root");
}

void	Location::setIndex(std::istringstream& lineStream) {
	lineStream >> index;
}

void	Location::addMethod(std::istringstream& lineStream)
{
	std::string	method;
	while (lineStream >> method)
	{
		if (method != "GET" && method != "POST" && method != "DELETE")
			throw std::runtime_error("Invalid method");
		methods.push_back(method);
	}
}

void	Location::setAutoIndex(std::istringstream& lineStream)
{
	std::string	index;
	lineStream >> index;
	if (index == "on" && lineStream.eof())
		auto_index = true;
	else if (index == "off" && lineStream.eof())
		auto_index = false;
	else
		throw std::runtime_error("Invalid auto index!");
}

const std::string&	Location::getPath() const { return path; }

const std::string&	Location::getroot() const { return root; }

const std::string&	Location::getIndex() const { return index; }

const std::vector<std::string>& Location::getMethods() const { return methods; }

bool	Location::getAutoIndex() const { return auto_index; }

void	Location::printLocation() const
{
	std::cout << "-Location: " << path << std::endl;

	std::cout << "	Methods:";
	for (size_t i = 0; i < methods.size(); ++i) {
		std::cout << " " << methods[i];
	}
	std::cout << std::endl;
	std::cout << "	index: " << index << std::endl
			  << "	root: " << root << std::endl
			  << "	AutoIndex: " << auto_index << std::endl;
}