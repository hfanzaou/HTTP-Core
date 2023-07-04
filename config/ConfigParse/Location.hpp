/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajana <ajana@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/08 21:59:13 by ajana             #+#    #+#             */
/*   Updated: 2023/06/21 02:11:05 by ajana            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef	LOCATION_HPP
#define LOCATION_HPP

#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <dirent.h>

class Location
{
	private :
		std::string					path;
		std::string					root;
		std::string					index;
		std::vector<std::string>	methods;
		bool						auto_index;
	public :
		// Location();
		void	setPath(std::istringstream& lineStream);
		void	setRoot(std::istringstream& lineStream);
		void	setIndex(std::istringstream& lineStream);
		void	addMethod(std::istringstream& lineStream);
		void	setAutoIndex(std::istringstream& lineStream);

		const std::string&	getPath() const;
		const std::string&	getroot() const;
		const std::string&	getIndex() const;
		const std::vector<std::string>& getMethods() const;
		bool	getAutoIndex() const;

		void	printLocation() const;
};

#endif