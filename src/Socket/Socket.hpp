/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebensalt <ebensalt@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/08 16:03:36 by ebensalt          #+#    #+#             */
/*   Updated: 2023/06/10 20:47:58 by ebensalt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_HPP
# define SOCKET_HPP
# include <iostream>
# include <sys/types.h>
# include <sys/socket.h>
# include <netdb.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/select.h>

class Socket
{
	private	:
		std::string				hostname;
		std::string				servname;
		struct addrinfo			hints;
		struct addrinfo			*res;
		int						sock_fd;
		int						acpt_fd;
		struct sockaddr_storage	acpt_addr;
		int						acpt_addr_len;
		char					*buff;
		// char					buff[1024];
		fd_set					main;
		fd_set					read;
		int						set_max;
		std::string				Request;
	public	:
		Socket(void);
		Socket(const Socket &o);
		~Socket(void);

		Socket	&operator=(const Socket &o);

		bool	start_server(void);
		bool	ft_errors(void);
};

#endif
