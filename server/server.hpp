/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hfanzaou <hfanzaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/14 23:24:59 by ebensalt          #+#    #+#             */
/*   Updated: 2023/07/15 09:37:56 by hfanzaou         ###   ########.fr       */
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
# include <cstdio>

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
		int							bytes_read;
		std::string					hex;
		bool						chunk;
		// int 						content_len;
		// size_t						chunkSize;
		// std::vector<char>			chunk;
		std::ofstream				*file;
		std::map<int, Response*> 	response;
	public	:
		server(Config &c);

		void			init_server(ServerConfig &s);
		void			start_server(void);
		void			multiplex_server(void);
		void			accept_server(int s);
		bool			read_server(int i);
		void			write_server(int i);
		bool			parse_req(int i);
		void			parse_req_line(int i);
		void			set_status(const std::string &m, int s, int i);
		int				parse_header(int i);
		void			check_header(std::map<std::string, std::string> &h, int fd);
		std::string		remove_r(std::string &s);
		int				find_req(int i);
		void			erase_req(int i);
		void			post(int fd, int j);
		// void		handleChuncked(int fd, int ind);
		// void		openFile(int fd);
		void			drop_client(int i);
		// int			read_chunkSize(int ind);
		void			add_req(int s);
		int				get_chunk_size(std::vector<char> &b);
		bool			check_chunk_end(std::vector<char> &b);
		ServerConfig&	get_config(std::string &host);
		void			Drop_Response(int i);
		// void		init_fd_set(void);
};

#endif
