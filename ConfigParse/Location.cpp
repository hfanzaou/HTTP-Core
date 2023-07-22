/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebensalt <ebensalt@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/08 22:26:31 by ajana             #+#    #+#             */
/*   Updated: 2023/07/20 21:26:10 by ebensalt         ###   ########.fr       */
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
	DIR *dir = NULL;
	if (!(lineStream >> root) || (!lineStream.eof()) || !(dir = opendir(root.c_str())))
	{
		if (dir)
			closedir(dir);
		throw std::runtime_error("Invalid location root");
	}
	if (dir)
		closedir(dir);
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

void	Location::setRedirect(std::istringstream& lineStream)
{
	if (!(lineStream >> redirect) || !lineStream.eof())
		throw std::runtime_error("Invalid redirection");
}

void	Location::setUploadPath(std::istringstream& lineStream)
{
	if (!(lineStream >> upload_path) || !lineStream.eof())
		throw std::runtime_error("Invalid redirection");
}

void	Location::checkLocation() const
{
	if (root.empty())
		throw std::runtime_error("Location root missing!");
	else if (path.empty())
		throw std::runtime_error("Location path missing!");
}

const std::string&	Location::getPath() const { return path; }

const std::string&	Location::getroot() const { return root; }

const std::string&	Location::getIndex() const { return index; }

const std::string&	Location::getRedirect() const { return redirect; }

const std::string&	Location::getUploadPath() const { return upload_path; }

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
			  << "	AutoIndex: " << auto_index << std::endl
			  << "	Redirect: " << redirect << std::endl;
}