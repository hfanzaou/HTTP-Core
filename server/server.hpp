/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hfanzaou <hfanzaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/14 23:24:59 by ebensalt          #+#    #+#             */
/*   Updated: 2023/07/05 17:14:34 by hfanzaou         ###   ########.fr       */
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
# include <vector>
# include <fstream>
# include <cctype>
# include <algorithm>
#include "../ConfigParse/Config.hpp"
#include "../Response/Response.hpp"

class server
{
	private	:
		// Config					config;
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
		char					buff[102400];
		// char					buff
		fd_set					read;
		fd_set					write;
		int						nfds;
		// std::string				read_line;
		// request					*req;
		std::vector<request>	reqs;
		// bool					req_l;
		// bool					req_h;
		// bool					req_b;
		// int						body_len;
		int						read_len;
		std::map<int, Response*> response;
	public	:
		// server(Config c);

		void		init_server(void);
		void		start_server(void);
		void		multiplex_server(ServerConfig& config);
		void		accept_server(void);
		bool		read_server(int i);
		void		write_server(int i);
		bool		parse_req(int i);
		void		parse_req_line(int i);
		void		set_status(const std::string &m, int s, int i);
		void		parse_header(int i);
		void		check_header(std::map<std::string, std::string> &h, int fd);
		std::string	remove_r(std::string &s);
		int			find_req(int i);
		void		erase_req(int i);
		void		post(int fd);
};

#endif
