/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hfanzaou <hfanzaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/14 23:24:59 by ebensalt          #+#    #+#             */
/*   Updated: 2023/07/02 16:21:17 by hfanzaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP
# include <sys/types.h>
# include <sys/socket.h>
# include <netdb.h>
# include <string.h>
# include <fcntl.h>
# include <sys/select.h>
# include "../request/request.hpp"
# include "../Response/Response.hpp"
# include <vector>
# include <fstream>
# include <cctype>
#include "../config/ConfigParse/Config.hpp"

class server
{
	private	:
		std::string				hostname;
		std::string				servname;
		struct addrinfo			hints;
		struct addrinfo			*res;
		int						sock_fd;
		int						option_value;
		int						option_len;
		struct sockaddr_storage	acpt_addr;
		int						acpt_len;
		int						acpt_fd;
		char					buff[1024];
		fd_set					read;
		fd_set					write;
		int						nfds;
		request					*req;
		std::string				read_line;
		bool					req_l;
		bool					req_h;
		bool					req_b;
		int						body_len;
		std::map<int, Response*>	response;
	public	:
		void		init_server(void);
		void		start_server(void);
		void		multiplex_server(ServerConfig& config);
		void		accept_server(void);
		bool		read_server(int i);
		void		write_server(int i);
		bool		parse_req(void);
		void		parse_req_line(void);
		void		set_status(const std::string &m, int s);
		void		parse_header(void);
		void		check_header(std::map<std::string, std::string> &h);
		std::string	remove_r(std::string &s);
};

#endif
