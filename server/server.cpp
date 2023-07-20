/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebensalt <ebensalt@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/14 23:24:46 by ebensalt          #+#    #+#             */
/*   Updated: 2023/07/20 03:47:10 by ebensalt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

server::server(Config &c) : config(c)
{
	FD_ZERO(&read);
	FD_ZERO(&writ);
}

void	server::init_server(ServerConfig &s)
{
	(void)config;
	hostname = s.getHost();
	servname = s.getPort();
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	option_value = 1;
	option_len = sizeof(option_value);
	memset(&acpt_addr, 0, sizeof(acpt_addr));
	acpt_len = sizeof(acpt_addr);
	name = s.getServerNames();
}

void	server::start_server(void)
{
	int	s;

	if (getaddrinfo(hostname.c_str(), servname.c_str(), &hints, &res))
	{
		std::cerr << "Error : getaddrinfo!" << std::endl;
		return ;
	}
	if ((s = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) == -1)
	{
		std::cerr << "Error : socket!" << std::endl;
		return ;
	}
	signal(SIGPIPE, SIG_IGN);
	if (fcntl(s, F_SETFL, O_NONBLOCK) == -1)
	{
		std::cerr << "Error : fcntl!" << std::endl;
		return ;
	}
	if (setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &option_value, (socklen_t)option_len))
	{
		std::cerr << "Error : setsockopt!" << std::endl;
		return ;
	}
	if (bind(s, res->ai_addr, res->ai_addrlen))
		return ;
	freeaddrinfo(res);
	if (listen(s, 128))
	{
		std::cerr << "Error : listen!" << std::endl;
		return ;
	}
	std::cout << "\033[1;31mSERVER " << hostname << ":" << servname << " IS LISTENING" << "\033[0m" << std::endl;
	FD_SET(s, &read);
	FD_SET(s, &writ);
	if (nfds < s)
		nfds = s;
	sock_fd.push_back(s);
	std::string	tmp = hostname + ":" + servname;

	serv.push_back(Socket(s, tmp));
}

void	server::multiplex_server(void)
{
	fd_set	tmp_r;
	fd_set	tmp_w;
	struct timeval	tv;

	tv.tv_sec = 0;
	tv.tv_usec = 1000;
	while (1337)
	{
		FD_ZERO(&tmp_r);
		FD_ZERO(&tmp_w);
		tmp_r = read;
		tmp_w = writ;
		if (select((nfds + 1), &tmp_r, &tmp_w, NULL, &tv) == -1)
		{
			std::cerr << "Error : select!" << std::endl;
			return ;
		}
		for (int i = 3; i <= nfds; i++)
		{
			if (FD_ISSET(i, &tmp_r))
			{
				size_t it = 0;
				for (; it < sock_fd.size(); it++)
				{
					if (i == sock_fd[it])
					{
						accept_server(sock_fd[it]);
						FD_SET(acpt_fd, &read);
						if (acpt_fd > nfds)
							nfds = acpt_fd;
						break ;
					}
				}
				if (it == sock_fd.size() && read_server(i))
				{
					if (this->response.find(i) != this->response.end())
					{
						//std::cout << "ok" <<std::endl;
						Drop_Response(i);
						//this->response.insert(std::pair<int, Response*>(i, new Response(reqs[find_req(i)], get_config(reqs[find_req(i)].get_host(), reqs[find_req(i)].get_name()))));
					}
					FD_SET(i, &writ);
				}
			}
			else if (FD_ISSET(i, &tmp_w))
			{
				if (this->response.find(i) == this->response.end())
				{
					std::cout << "\033[1;32mREQUEST: \033[0m" << reqs[find_req(i)].get_method() << " " << reqs[find_req(i)].get_uri() << std::endl;
					this->response.insert(std::pair<int, Response*>(i, new Response(reqs[find_req(i)], get_config(reqs[find_req(i)].get_host(), reqs[find_req(i)].get_name()))));
					//reqs[find_req(i)].print_all();
				}
				write_server(i);
			}
		}
	}
}

void	server::accept_server(int s)
{
	std::cout << "\033[1;32mNEW CONECTION\033[0m" << std::endl;
	if ((acpt_fd = accept(s, (sockaddr *)&acpt_addr, (socklen_t *)&acpt_len)) == -1)
	{
		std::cerr << "Error : accept!" << std::endl;
		return ;
	}
	add_req(s);
}

bool	server::read_server(int i)
{
	if ((read_len = recv(i, buff, 70000, 0)) < 1)
	{
		std::cerr << "Error : recv!" << std::endl;
		if (this->response.find(i) != this->response.end())
		{
			Drop_Response(i);
			//this->response.insert(std::pair<int, Response*>(i, new Response(reqs[find_req(i)], get_config(reqs[find_req(i)].get_host()))));
		}
		drop_client(i);
		return (false);
	}
	reqs[find_req(i)].get_post().set_bytes_read(reqs[find_req(i)].get_post().get_bytes_read() + read_len);
	if (reqs[find_req(i)].get_req_l() && reqs[find_req(i)].get_req_h() && reqs[find_req(i)].get_req_b())
		return (false);
	if (!reqs[find_req(i)].get_req_h())
		reqs[find_req(i)].add_to_read_line(buff);
	return (parse_req(i));
}

void	server::write_server(int i)
{
	response[i]->generate_response();
	std::string res = response[i]->get_res();
	if (send(i, res.c_str(), res.size(), 0) == -1)
	{
		std::cerr << "Error : send!" << std::endl;
		drop_client(i);
		return ;
	}
	if (response[i]->get_send_status())
	{
		if (response[i]->get_code() == 200)
			std::cout << "\033[1;32mRESPONSE:\033[0m 200 OK"<< std::endl;
		else
			std::cout << "\033[31mRESPONSE: \033[0m" << generate_error(response[i]->get_code()) << std::endl;
		Drop_Response(i);
		drop_client(i);
	}
	// reqs[find_req(i)].print_all();
}

bool	server::parse_req(int i)
{
	int	j = 0;

	if (!reqs[find_req(i)].get_req_h())
	{
		std::string	read_line = reqs[find_req(i)].get_read_line();
		if (read_line.find("\r\n") == std::string::npos && (!reqs[find_req(i)].get_req_l() || !reqs[find_req(i)].get_req_h()))
			return (false);
	}
	if (!reqs[find_req(i)].get_req_l())
		parse_req_line(i);
	if (!reqs[find_req(i)].get_req_h())
		j = parse_header(i);
	if (!reqs[find_req(i)].get_req_b())
		post(i, j);
	if (reqs[find_req(i)].get_req_l() && reqs[find_req(i)].get_req_h() && reqs[find_req(i)].get_req_b())
		return (true);
	return (false);
}

void	server::parse_req_line(int i)
{
	reqs[find_req(i)].set_req_l(true);
	std::stringstream	ss(reqs[find_req(i)].get_read_line());
	std::string			m;
	std::string			u;
	std::string			h;

	ss >> m >> u >> h;
	if ((m != "GET" && m != "POST" && m != "DELETE") || h != "HTTP/1.1" || u.find_first_not_of("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-._~:/#[]@!$&'()*+,;=?") != std::string::npos)
	{
		set_status("Bad Request", 400, i);
		return ;
	}
	if (m == "POST")
		reqs[find_req(i)].set_req_b(false);
	reqs[find_req(i)].set_method(m);
	reqs[find_req(i)].set_http_version(h);
	if (u.size() > 2048)
	{
		set_status("URI Too Long", 414, i);
		return ;
	}
	if (u.find('?') != std::string::npos)
	{
		std::stringstream	ss0(u);
		u.clear();
		getline(ss0, u, '?');
		reqs[find_req(i)].set_request_uri(u);
		u.clear();
		getline(ss0, u, '?');
		reqs[find_req(i)].set_query(u);
	}
	else
		reqs[find_req(i)].set_request_uri(u);
	std::stringstream	ss0;

	getline(ss, m);
	ss0 << ss.rdbuf();
	reqs[find_req(i)].set_read_line(ss0.str());
}

void	server::set_status(const std::string &m, int s, int i)
{
	reqs[find_req(i)].set_msg(m);
	reqs[find_req(i)].set_status_code(s);
	reqs[find_req(i)].set_req_l(true);
	reqs[find_req(i)].set_req_h(true);
	reqs[find_req(i)].set_req_b(true);
}

int	server::parse_header(int i)
{
	std::stringstream					ss(reqs[find_req(i)].get_read_line());
	std::string							tmp;
	std::string							s1;
	std::string							s2;
	std::map<std::string, std::string>	h;

	while (getline(ss, tmp))
	{
		if (tmp.find(':') != std::string::npos && tmp.find('\r') != std::string::npos)
		{
			s1 = tmp.substr(0, tmp.find(':'));
			s2 = tmp.substr(tmp.find(':') + 2, tmp.find('\r'));
			s2 = remove_r(s2);
			h.insert(std::make_pair(s1, s2));
		}
		else if (tmp == "\r")
		{
			reqs[find_req(i)].set_req_h(true);
			check_header(h, i);
			if (h.find("Host") != h.end())
				reqs[find_req(i)].set_name(h.find("Host")->second);
			reqs[find_req(i)].set_header(h);
			// response.insert(std::pair<int, Response*>(i, new Response(reqs[find_req(i)], get_config(reqs[find_req(i)].get_host(), reqs[find_req(i)].get_name()))));
			// response[i]->generate_response();
			// if (response[i]->get_code() == NOT_ALLOWED)
			// 	set_status("Method Not Allowed", NOT_ALLOWED, i);
			// Drop_Response(i);
			int	j = 0;
			for (; j < read_len; j++)
				if (j + 3 < read_len && buff[j] == '\r' && buff[j + 1] == '\n' && buff[j + 2] == '\r' && buff[j + 3] == '\n')
				{
					j += 4;
					reqs[find_req(i)].get_post().set_bytes_read(reqs[find_req(i)].get_post().get_bytes_read() - j);
					return (j);
				}
		}
	}
	return (0);
}

void	server::check_header(std::map<std::string, std::string> &h, int fd)
{
	std::map<std::string, std::string>::iterator	i = h.find("Transfer-Encoding");
	std::map<std::string, std::string>::iterator	j = h.find("Content-Length");

	if (i != h.end())
	{
		if (i->second != "chunked")
		{
			set_status("Not Implemented", 501, fd);
			return ;
		}
	}
	if ((!reqs[find_req(fd)].get_req_b() && i == h.end() && j == h.end()) || (i != h.end() && j != h.end()))
	{
		set_status("Bad Request", 400, fd);
		return ;
	}
}

std::string	server::remove_r(std::string &s)
{
	std::string tmp;
	
	for (size_t i = 0; i < s.size(); i++)
		if (s[i] != '\r')
			tmp += s[i];
	return (tmp);
}

int	server::find_req(int i)
{
	for (size_t j = 0; j < reqs.size(); j++)
	{
		if (reqs[j].get_fd() == i)
			return (j);
	}
	return (-1);
}

void	server::erase_req(int i)
{
	for (size_t j = 0; j < reqs.size(); j++)
	{
		if (reqs[j].get_fd() == i)
		{
			reqs.erase(reqs.begin() + j);
			return ;
		}
	}
}

void	server::drop_client(int i)
{
	close(i);
	FD_CLR(i, &read);
	FD_CLR(i, &writ);
	erase_req(i);
}

void	server::post(int fd, int j)
{
	if (reqs[find_req(fd)].get_header().find("Content-Length") != reqs[find_req(fd)].get_header().end())
		post_cl(fd, j);
	else
		post_ch(fd, j);
}

void	server::add_req(int s)
{
	for (size_t i = 0; i < serv.size(); i++)
	{
		if (serv[i].get_fd() == s)
		{
			reqs.push_back(request(acpt_fd, serv[i].get_host()));
			break ;
		}
	}
}

void	server::post_cl(int fd, int j)
{
	if (reqs[find_req(fd)].get_post().get_file() == 0)
	{
		int	tmp;

		std::stringstream(reqs[find_req(fd)].get_header()["Content-Length"]) >> tmp;
		reqs[find_req(fd)].get_post().set_body_size(tmp);
		if (reqs[find_req(fd)].get_post().get_body_size() > get_body_size(fd))
		{
			set_status("Payload Too Large", 413, fd);
			return ;
		}
		reqs[find_req(fd)].get_post().set_i(tmp);
		open_file(fd);
	}
	write(reqs[find_req(fd)].get_post().get_file(), &buff[j], reqs[find_req(fd)].get_post().get_bytes_read());
	reqs[find_req(fd)].get_post().set_i(reqs[find_req(fd)].get_post().get_i() - reqs[find_req(fd)].get_post().get_bytes_read());
	reqs[find_req(fd)].get_post().set_bytes_read(0);
	if (reqs[find_req(fd)].get_post().get_i() == 0)
	{
		close(reqs[find_req(fd)].get_post().get_file());
		reqs[find_req(fd)].set_req_b(true);
		return ;
	}
}

void	server::post_ch(int fd, int j)
{
	if (reqs[find_req(fd)].get_post().get_file() == 0)
		open_file(fd);
	for (;j < read_len; j++)
		reqs[find_req(fd)].get_post().get_body().push_back(buff[j]);
	if (!reqs[find_req(fd)].get_post().get_chunk_size())
	{
		while (!reqs[find_req(fd)].get_post().get_body().empty())
		{
			if (reqs[find_req(fd)].get_post().get_body()[0] == '\r')
			{
				reqs[find_req(fd)].get_post().get_body().erase(reqs[find_req(fd)].get_post().get_body().begin());
				reqs[find_req(fd)].get_post().set_bytes_read(reqs[find_req(fd)].get_post().get_bytes_read() - 1);
				continue ;
			}
			if (reqs[find_req(fd)].get_post().get_body()[0] == '\n')
			{
				reqs[find_req(fd)].get_post().get_body().erase(reqs[find_req(fd)].get_post().get_body().begin());
				reqs[find_req(fd)].get_post().set_bytes_read(reqs[find_req(fd)].get_post().get_bytes_read() - 1);
				reqs[find_req(fd)].get_post().set_chunk_size(std::strtol(reqs[find_req(fd)].get_post().get_hex().c_str(), NULL, 16));
				reqs[find_req(fd)].get_post().get_hex().clear();
				reqs[find_req(fd)].get_post().set_body_size(reqs[find_req(fd)].get_post().get_body_size() + reqs[find_req(fd)].get_post().get_chunk_size());
				if (reqs[find_req(fd)].get_post().get_body_size() > get_body_size(fd))
				{
					set_status("Payload Too Large", 413, fd);
					return ;
				}
				if (!reqs[find_req(fd)].get_post().get_chunk_size())
				{
					close(reqs[find_req(fd)].get_post().get_file());
					reqs[find_req(fd)].set_req_b(true);
					return ;
				}
				else
					break ;
			}
			reqs[find_req(fd)].get_post().set_hex(reqs[find_req(fd)].get_post().get_hex() + reqs[find_req(fd)].get_post().get_body()[0]);
			reqs[find_req(fd)].get_post().get_body().erase(reqs[find_req(fd)].get_post().get_body().begin());
			reqs[find_req(fd)].get_post().set_bytes_read(reqs[find_req(fd)].get_post().get_bytes_read() - 1);
		}
	}
	if (reqs[find_req(fd)].get_post().get_chunk_size())
	{
		if (reqs[find_req(fd)].get_post().get_i() + 2 >= reqs[find_req(fd)].get_post().get_bytes_read())
		{
			reqs[find_req(fd)].get_post().set_i(reqs[find_req(fd)].get_post().get_i() - reqs[find_req(fd)].get_post().get_bytes_read());
			reqs[find_req(fd)].get_post().set_bytes_read(0);
		}
		else
		{
			write(reqs[find_req(fd)].get_post().get_file(), &reqs[find_req(fd)].get_post().get_body()[0], reqs[find_req(fd)].get_post().get_chunk_size());
			reqs[find_req(fd)].get_post().get_body().erase(reqs[find_req(fd)].get_post().get_body().begin(), reqs[find_req(fd)].get_post().get_body().begin() + reqs[find_req(fd)].get_post().get_chunk_size() + 2);
			reqs[find_req(fd)].get_post().set_bytes_read(reqs[find_req(fd)].get_post().get_bytes_read() - reqs[find_req(fd)].get_post().get_i() - 2);
			reqs[find_req(fd)].get_post().set_chunk_size(0);
			post_ch(fd, j);
		}
	}
}

ServerConfig& server::get_config(std::string &host, std::string &name)
{
	std::vector<ServerConfig>& configs = config.getServers();
	for (std::vector<ServerConfig>::iterator it = configs.begin(); it != configs.end(); ++it)
	{
		//std::cout << "host = " << host << " it->getHost() = " << it->getHost() << std::endl;
		std::vector<std::string>::const_iterator it2;
		for (it2 = it->getServerNames().cbegin(); it2 != it->getServerNames().cend(); ++it2)
		{
			//std::cout << "it2 = " << *it2 << " name " << name << std::endl;
			if (name == *it2)
				return (*it);
		}
	}
	for (std::vector<ServerConfig>::iterator it = configs.begin(); it != configs.end(); ++it)
	{
		//std::cout << "host = " << host << " it->getHost() = " << it->getHost() << std::endl;
		if ((it->getHost() + ":" + it->getPort()) == host)
			return (*it);
	}
	// std::cout << "here" << std::endl;
	return (config.getServers()[0]);
}

void	server::Drop_Response(int i)
{
	response[i]->Drop_file();
	delete response[i];
	response.erase(i);
}

unsigned long	server::get_body_size(int fd)
{
	for (size_t i = 0; i < config.getServers().size(); i++)
		for (size_t j = 0; j < config.getServers()[i].getServerNames().size(); j++)
			if (config.getServers()[i].getServerNames()[j] == reqs[find_req(fd)].get_name())
				return (config.getServers()[i].getMaxBodySize());
	for (size_t i = 0; i < config.getServers().size(); i++)
			if (config.getServers()[i].getHost() + ':' + config.getServers()[i].getPort() == reqs[find_req(fd)].get_host())
				return (config.getServers()[i].getMaxBodySize());
	return (0);
}

void	server::open_file(int fd)
{
	std::string			file_name = "test";
	
	std::srand(static_cast<unsigned int>(std::time(0)));
	static int			i = std::rand() % 10000;
	std::stringstream	ss0;

	ss0 << i;
	file_name += ss0.str();
	if (reqs[find_req(fd)].get_header().find("Content-Type") != reqs[find_req(fd)].get_header().end())
	{
		file_name += '.';
		std::stringstream	ss(reqs[find_req(fd)].get_header().find("Content-Type")->second);
		std::string			ex;

		getline(ss, ex, '/');
		getline(ss, ex);
		file_name += ex;
	}
	i++;
	reqs[find_req(fd)].get_post().set_file_name(file_name);
	reqs[find_req(fd)].get_post().set_file(open(file_name.c_str(), O_CREAT | O_TRUNC | O_WRONLY, 0644));
}
