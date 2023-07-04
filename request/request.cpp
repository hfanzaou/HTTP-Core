/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebensalt <ebensalt@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/15 21:05:41 by ebensalt          #+#    #+#             */
/*   Updated: 2023/06/26 11:20:41 by ebensalt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "request.hpp"

request::request(void)
{
	msg = "OK";
	status_code = 200;
}

request::~request(void)
{
	// std::cout << "request deleted" << std::endl;
}

void	request::set_method(std::string &m)
{
	method = m;
}

void	request::set_request_uri(std::string &r)
{
	request_uri = r;
}

void	request::set_http_version(std::string &h)
{
	http_version = h;
}

void	request::set_msg(const std::string &m)
{
	msg = m;
}

void	request::set_status_code(int s)
{
	status_code = s;
}

void	request::set_header(std::map<std::string, std::string> &h)
{
	header = h;
}

void	request::print_all(void)
{
	std::cout << "Method      : " << method << std::endl;
	std::cout << "Request_uri : " << request_uri << std::endl;
	std::cout << "Http_version: " << http_version << std::endl;
	std::cout << "Msg         : " << msg << std::endl;
	std::cout << "Status_code : " << status_code << std::endl;
	std::cout << "headers     {" << std::endl;
	for (std::map<std::string, std::string>::iterator i = header.begin(); i != header.end(); i++)
		std::cout << i->first << " = " << i->second << std::endl;
	std::cout << "}" << std::endl;
}
