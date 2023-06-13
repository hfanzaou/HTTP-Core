/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebensalt <ebensalt@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/10 20:49:58 by ebensalt          #+#    #+#             */
/*   Updated: 2023/06/11 21:01:27 by ebensalt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

Request::Request(void) {}

Request::Request(const Request &o)
{
	*this = o;
}

Request::~Request(void) {}

Request::Request(std::string r, int f) : req(r), fd(f) {}

Request	&Request::operator=(const Request &o)
{
	req = o.req;
	fd = o.fd;
	return (*this);
}

void	Request::print_req(void)
{
	std::cout << "REQUEST : \n" << req << std::endl;
	std::cout << "FD : " << fd << std::endl;
	std::cout << "*******************" << std::endl;
}

int	Request::get_fd(void)
{
	return (fd);
}
