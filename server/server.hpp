/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebensalt <ebensalt@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/14 23:24:59 by ebensalt          #+#    #+#             */
/*   Updated: 2023/07/20 03:39:18 by ebensalt         ###   ########.fr       */
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
# include "../ConfigParse/ServerConfig.hpp"
# include "../ConfigParse/Location.hpp"
# include "../Response/Response.hpp"
# include "Socket.hpp"
# include <cstdio>

class server
{
	private	:
		std::string					hostname;
		std::string					servname;
		//
		std::vector<std::string>	name;
		//
		struct addrinfo				hints;
		struct addrinfo				*res;
		std::vector<int>			sock_fd;
		std::vector<Socket>			serv;
		int							option_value;
		int							option_len;
		struct sockaddr_storage		acpt_addr;
		int							acpt_len;
		int							acpt_fd;
		char						buff[70000];
		fd_set						read;
		fd_set						writ;
		int							nfds;
		std::vector<request>		reqs;
		int							read_len;
		Config						&config;
		std::map<int, Response*>	response;
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
		int			parse_header(int i);
		void		check_header(std::map<std::string, std::string> &h, int fd);
		std::string	remove_r(std::string &s);
		int			find_req(int i);
		void		erase_req(int i);
		void		post(int fd, int j);
		void		drop_client(int i);
		void		add_req(int s);
		void		post_cl(int fd, int j);
		void		post_ch(int fd, int j);
		ServerConfig &get_config(std::string &host, std::string& name);
		void		Drop_Response(int i);
		void		open_file(int fd);
		unsigned long	get_body_size(int fd);
};

#endif
