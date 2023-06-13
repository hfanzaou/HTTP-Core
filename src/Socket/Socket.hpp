/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebensalt <ebensalt@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/08 16:03:36 by ebensalt          #+#    #+#             */
/*   Updated: 2023/06/11 20:59:38 by ebensalt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_HPP
# define SOCKET_HPP
# include <sys/types.h>
# include <sys/socket.h>
# include <netdb.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/select.h>
# include "../Request/Request.hpp"
# include <vector>

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
		fd_set					main;
		fd_set					read;
		fd_set					write;
		int						set_max;
		std::string				req_str;
		std::vector<Request>	req;
	public	:
		Socket(void);
		Socket(const Socket &o);
		~Socket(void);

		Socket	&operator=(const Socket &o);

		bool	start_server(void);
		bool	ft_errors(void);
		void	add_req(int f);
		void	rmv_req(int f);
};

#endif
