/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebensalt <ebensalt@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/15 21:05:41 by ebensalt          #+#    #+#             */
/*   Updated: 2023/07/20 10:45:13 by ebensalt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "request.hpp"

request::request(int i, std::string &h) : msg("OK"), status_code(200), fd(i), req_l(false), req_h(false), req_b(true), host(h), done(false) {}

request::~request(void) {}

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
	std::cout << "Name        : " << name << std::endl;
	std::cout << "Query       : " << query << std::endl;
	std::cout << "Method      : " << method << std::endl;
	std::cout << "Request_uri : " << request_uri << std::endl;
	std::cout << "Http_version: " << http_version << std::endl;
	std::cout << "Msg         : " << msg << std::endl;
	std::cout << "Status_code : " << status_code << std::endl;
	std::cout << "headers     {" << std::endl;
	for (std::map<std::string, std::string>::iterator i = header.begin(); i != header.end(); i++)
		std::cout << i->first << " = " << i->second << std::endl;
	std::cout << "}" << std::endl;
	std::cout << "host => " << host << std::endl;
}

int	request::get_fd(void)
{
	return (fd);
}

bool	request::get_req_l(void)
{
	return (req_l);
}

bool	request::get_req_h(void)
{
	return (req_h);
}

bool	request::get_req_b(void)
{
	return (req_b);
}

void	request::set_req_l(bool r)
{
	req_l = r;
}

void	request::set_req_h(bool r)
{
	req_h = r;
}

void	request::set_req_b(bool r)
{
	req_b = r;
}

std::map<std::string, std::string>	&request::get_header(void)
{
	return (header);
}

std::string	&request::get_read_line(void)
{
	return (read_line);
}

void	request::set_read_line(std::string r)
{
	read_line = r;
}

void	request::add_to_read_line(char *b)
{
	read_line += b;
}

std::string	&request::get_method(void)
{
	return (method);
}

Post	&request::get_post(void)
{
	return (post);
}

std::string	&request::get_name(void)
{
	return (name);
}

std::string	&request::get_query(void)
{
	return (query);
}

void	request::set_name(std::string &n)
{
	name = n;
}

void	request::set_query(std::string &q)
{
	query = q;
}

std::string &request::get_uri(void)
{
	return (request_uri);
}
int request::get_status_code(void)
{
	return (status_code);
}

std::string	&request::get_host(void)
{
	return (host);
}

bool	request::get_done(void)
{
	return (done);
}

void	request::set_done(bool d)
{
	done = d;
}
