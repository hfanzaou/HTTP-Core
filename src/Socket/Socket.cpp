/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hfanzaou <hfanzaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/08 16:03:40 by ebensalt          #+#    #+#             */
/*   Updated: 2023/06/13 03:13:28 by hfanzaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Socket.hpp"
#include "../../../Get.hpp"
Socket::Socket(void)
{
	std::cout << "Socket default constructor called" << std::endl;
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
}

Socket::Socket(const Socket &o)
{
	std::cout << "Socket copy constructor called" << std::endl;
	*this = o;
}

Socket::~Socket(void)
{
	std::cout << "Socket destructor called" << std::endl;
}

Socket	&Socket::operator=(const Socket &o)
{
	std::cout << "Socket copy assignment operator called" << std::endl;
	(void)o;
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
		// std::cout << "ok\n";
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
				}
				else
				{
					while (recv(i, buff, 1, 0) == 1)
					{
						buff[1] = 0;
						Request = Request + buff;
						// std::cout << buff;
					}
					std::cout << Request;
					response.generate_response();
					//std::cout << write << std::endl;
					send(i, response.get_head().c_str(), response.get_head().length(), 0);
					send(i, &response.get_body()[0], response.get_body().size(), 0);
					close(i);
					FD_CLR(i, &main);
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
