/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hfanzaou <hfanzaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/08 16:03:40 by ebensalt          #+#    #+#             */
/*   Updated: 2023/06/13 04:16:43 by hfanzaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Socket.hpp"
#include "../Get.hpp"
Socket::Socket(void)
{
	hostname = "127.0.0.1";
	servname = "8080";
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	memset(&acpt_addr, 0, sizeof(acpt_addr));
	acpt_addr_len = sizeof(acpt_addr);
	buff = new char[2];
	FD_ZERO(&main);
	FD_ZERO(&read);
	FD_ZERO(&write);
}

Socket::Socket(const Socket &o)
{
	*this = o;
}

Socket::~Socket(void) {}

Socket	&Socket::operator=(const Socket &o)
{
	if (this != &o)
	{
		hostname = o.hostname;
		servname = o.servname;
		hints = o.hints;
		sock_fd = o.sock_fd;
		acpt_fd = o.acpt_fd;
		acpt_addr = o.acpt_addr;
		acpt_addr_len = o.acpt_addr_len;
		main = o.main;
		read = o.read;
		set_max = o.set_max;
		req_str = o.req_str;
	}
	return (*this);
}

bool	Socket::start_server(void)
{
	int	opt_va = 1;
	int	opt_va_len = sizeof(opt_va);
	Get response;
	if (getaddrinfo(hostname.c_str(), servname.c_str(), &hints, &res))
		return (ft_errors());
	if ((sock_fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) == -1)
		return (ft_errors());
	if (fcntl(sock_fd, F_SETFL, O_NONBLOCK) == -1)
		return (ft_errors());
	if (setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &opt_va, (socklen_t)opt_va_len))
		return (ft_errors());
	if (bind(sock_fd, res->ai_addr, res->ai_addrlen))
		return (ft_errors());
	if (listen(sock_fd, 10))
		return (ft_errors());
	set_max = sock_fd + 1;
	FD_SET(sock_fd, &main);
	while (1)
	{
		read = main;
		select(set_max, &read, NULL, NULL, NULL);
		for (int i = 0; i < set_max; i++)
		{
			if (FD_ISSET(i, &read))
			{
				if (i == sock_fd)
				{
					if ((acpt_fd = accept(sock_fd, (sockaddr *)&acpt_addr, (socklen_t *)&acpt_addr_len)) == -1)
						return (ft_errors());
					FD_SET(acpt_fd, &main);
					if (acpt_fd > set_max - 1)
						set_max = acpt_fd + 1;
					break ;
				}
				else
				{
					while (recv(i, buff, 1, 0) == 1)
					{
						buff[1] = 0;
						req_str = req_str + buff;
					}
					add_req(i);
					req_str.clear();
					write = main;
					select(set_max, NULL, &write, NULL, NULL);
					for (int j = 0; j < set_max; j++)
					{
						if (FD_ISSET(j, &write))
						{
							// std::cout << j << std::endl;
							response.generate_response();
					//std::cout << write << std::endl;
							send(j, response.get_head().c_str(), response.get_head().length(), 0);
							std::vector<char> res = response.get_body();
							send(j, &res[0], res.size(), 0);
							close(j);
							FD_CLR(j, &main);
							rmv_req(j);
							// for (unsigned int i = 0; i < req.size(); i++)
							// 	req[i].print_req();
						}
					}
				}
			}
		}
	}
	return (true);
}

bool	Socket::ft_errors(void)
{
	std::cerr << "Error : failed to start the webserver " << std::endl;
	return (false);
}

void	Socket::add_req(int f)
{
	Request	tmp(req_str, f);
	req.push_back(tmp);
}

void	Socket::rmv_req(int f)
{
	for (unsigned int i = 0; i < req.size(); i++)
		if (req[i].get_fd() == f)
		{
			req.erase(req.begin() + i);
			return ;
		}
}
