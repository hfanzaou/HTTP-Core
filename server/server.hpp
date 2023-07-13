/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hfanzaou <hfanzaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/14 23:24:59 by ebensalt          #+#    #+#             */
/*   Updated: 2023/07/13 13:53:07 by hfanzaou         ###   ########.fr       */
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
# include "../ConfigParse/Config.hpp"
# include "../Response/Response.hpp"
# include "Socket.hpp"

class server
{
	private	:
		std::string					hostname;
		std::string					servname;
		std::vector<std::string>	name;
		struct addrinfo				hints;
		struct addrinfo				*res;
		std::vector<int>			sock_fd;
		std::vector<Socket>			serv;
		int							option_value;
		int							option_len;
		struct sockaddr_storage		acpt_addr;
		int							acpt_len;
		int							acpt_fd;
		char						buff[1024];
		fd_set						read;
		fd_set						write;
		int							nfds;
		std::vector<request>		reqs;
		int							read_len;
		Config						&config;
		std::map<int, Response*> response;
	public	:
		server(Config &c);

		void		init_server(ServerConfig &s);
		void		start_server(void);
		void		multiplex_server(void);
		void		accept_server(int s);
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
		void		drop_client(int i);
		void		add_req(int s);
		ServerConfig get_config(std::string &host);
		// void		init_fd_set(void);
};

#endif
