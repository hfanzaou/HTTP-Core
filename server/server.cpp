/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hfanzaou <hfanzaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/14 23:24:46 by ebensalt          #+#    #+#             */
/*   Updated: 2023/07/04 00:17:36 by hfanzaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

void	server::init_server(void)
{
	hostname = "127.0.0.1";
	// hostname = c.getHost();
	servname = "8080";
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	option_value = 1;
	option_len = sizeof(option_value);
	memset(&acpt_addr, 0, sizeof(acpt_addr));
	acpt_len = sizeof(acpt_addr);
	FD_ZERO(&read);
	FD_ZERO(&write);
	req_l = false;
	req_h = false;
	req_b = true;
	body_len = 0;
}

void	server::start_server(void)
{
	if (getaddrinfo(hostname.c_str(), servname.c_str(), &hints, &res))
	{
		std::cerr << "Error : getaddrinfo!" << std::endl;
		return ;
	}
	if ((sock_fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) == -1)
	{
		std::cerr << "Error : socket!" << std::endl;
		return ;
	}
	if (fcntl(sock_fd, F_SETFL, O_NONBLOCK) == -1)
	{
		std::cerr << "Error : fcntl!" << std::endl;
		return ;
	}
	if (setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &option_value, (socklen_t)option_len))
	{
		std::cerr << "Error : setsockopt!" << std::endl;
		return ;
	}
	if (bind(sock_fd, res->ai_addr, res->ai_addrlen))
	{
		std::cerr << "Error : bind!" << std::endl;
		return ;
	}
	freeaddrinfo(res);
	if (listen(sock_fd, 10))
	{
		std::cerr << "Error : listen!" << std::endl;
		return ;
	}
	FD_SET(sock_fd, &read);
	FD_SET(sock_fd, &write);
	nfds = sock_fd;
}

void	server::multiplex_server(ServerConfig& config)
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
		tmp_w = write;
		if (select((nfds + 1), &tmp_r, &tmp_w, NULL, NULL) == -1)
		{
			std::cerr << "Error : select!" << std::endl;
			return ;
		}
		for (int i = 3; i <= nfds; i++)
		{
			if (FD_ISSET(i, &tmp_r))
			{
				if (i == sock_fd)
				{
					accept_server();
					FD_SET(acpt_fd, &read);
					if (acpt_fd > nfds)
						nfds = acpt_fd;
				}
				else
				{
					if (read_server(i))
					{
						FD_CLR(i, &read);
						FD_SET(i, &write);
					}
				}
			}
			else if (FD_ISSET(i, &tmp_w))
			{
				if (this->response.find(i) == this->response.end())
					this->response.insert(std::pair<int, Response*>(i, new Response(this->req, config)));
				write_server(i);
				FD_CLR(i, &write);
			}
		}
	}
}

void	server::accept_server(void)
{
	if ((acpt_fd = accept(sock_fd, (sockaddr *)&acpt_addr, (socklen_t *)&acpt_len)) == -1)
	{
		std::cerr << "Error : accept!" << std::endl;
		return ;
	}
}

bool	server::read_server(int i)
{
	int		j;

	if ((j = recv(i, buff, 1023, 0)) < 1)
	{
		std::cerr << "Error : recv!" << std::endl;
		return (false);
	}
	buff[j] = 0;
	read_line += buff;
	if (body_len > 0)
	{
		// std::cout << "\033[1;32m";
		// std::cout << body_len << std::endl;
		// std::cout << "\033[0m";
		body_len -= j;
		// std::cout << body_len << std::endl;
	}
	// for (size_t i = 0; i < read_line.size(); i++)
	// {
	// 	if (read_line[i] == '\r')
	// 		std::cout << "\\r";
	// 	if (read_line[i] == '\n')
	// 		std::cout << "\\n" << "\n";
	// 	// else
	// 	// 	std::cout << read_line[i];
	// }
	// std::cout << std::endl;
	// exit(0);
	// std::cout << "\033[1;31m" << "buff : " << "\033[1;32m" << read_line << "\033[0m" /*<< buff */<< std::endl;
	// std::cout << "read_line : " << read_line << std::endl;
	return (parse_req());
}

void	server::write_server(int i)
{
	while (!response[i]->get_send_status())
	{
		response[i]->generate_response();
		std::string res = response[i]->get_res();
		if (!res.empty())
		{
			if (send(i, res.c_str(), res.size(), 0) == -1)
			{
				std::cerr << "Error : send!" << std::endl;
				return ;
			}
		}
	}
	if (response[i]->get_send_status())
	{
		delete response.find(i)->second;
		response.erase(response.find(i));
		delete req;
	}
	//std::cout << "fga" << std::endl;
	close(i);
}

bool	server::parse_req(void)
{
	if (read_line.find("\r\n") == std::string::npos && (!req_l || !req_h))
	{
		return (false);
	}
	if (!req_l)
		parse_req_line();
	if (!req_h)
		parse_header();
	// else if (!req_b)
		// parse_req_line(); //ajana;
	// std::cout << req_l << " " << req_h << " " << req_b << " " << body_len << std::endl;
	if (req_l && req_h && req_b && body_len <= 0)
	{
		read_line.erase();
		req_l = false;
		req_h = false;
		req_b = true;
		req->print_all();
		//delete req;
		return (true);
	}
	return (false);
}

void	server::parse_req_line(void)
{
	req = new request;
	std::cout << "////" << req->get_status_code() << std::endl;
	req_l = true;
	std::stringstream	ss(read_line);
	std::string			m;
	std::string			u;
	std::string			h;

	ss >> m >> u >> h;
	if ((m != "GET" && m != "POST" && m != "DELETE") || h != "HTTP/1.1" || u.find_first_not_of("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-._~:/?#[]@!$&'()*+,;=%") != std::string::npos)
	{
		set_status("Bad Request", 400);
		return ;
	}
	if (m == "POST")
		req_b = false;
	req->set_method(m);
	req->set_http_version(h);
	if (u.size() > 2048)
	{
		set_status("URI Too Long", 414);
		return ;
	}
	req->set_request_uri(u);
	std::stringstream	ss0;

	getline(ss, m);
	ss0 << ss.rdbuf();
	read_line = ss0.str();
}

void	server::set_status(const std::string &m, int s)
{
	req->set_msg(m);
	req->set_status_code(s);
}

void	server::parse_header(void)
{
	std::stringstream					ss(read_line);
	std::string							tmp;
	std::string							s1;
	std::string							s2;
	std::map<std::string, std::string>	h;

	// read_line.clear();
	while (getline(ss, tmp))
	{
		// read_line += tmp;
		if (tmp.find(':') != std::string::npos && tmp.find('\r') != std::string::npos)
		{
			s1 = tmp.substr(0, tmp.find(':'));
			s2 = tmp.substr(tmp.find(':') + 2, tmp.find('\r'));
			// std::cout << "ok" << std::endl;
			s2 = remove_r(s2);
			h.insert(std::make_pair(s1, s2));
		}
		else if (tmp == "\r")
		{
			req_h = true;
			std::stringstream	ss0;

			ss0 << ss.rdbuf();
			read_line = ss0.str();
			// std::cout << read_line << std::endl;
			check_header(h);
			req->set_header(h);
			// std::cout << read_line << std::endl;
			return ;
		}
		// else
		// {
		// 	std::stringstream	ss0;

		// 	ss0 << ss.rdbuf();
		// 	read_line = tmp;
		// 	read_line += ss0.str();
		// }
	}
	// check_header(h);
	// req->set_header(h);
	// std::cout << read_line << std::endl;
}

void	server::check_header(std::map<std::string, std::string> &h)
{
	std::map<std::string, std::string>::iterator	i = h.find("Transfer-Encoding");
	std::map<std::string, std::string>::iterator	j = h.find("Content-Length");

	if (j != h.end())
	{
		// req_b = false;
		std::stringstream	ss(j->second);

		ss >> body_len;
		body_len = body_len - read_line.size();
	}
	if (i != h.end())
	{
		if (i->second != "chunked")
		{
			set_status("Not Implemented", 501);
			return ;
		}
	}
	if ((!req_b && i == h.end() && j == h.end()) || (i != h.end() && j != h.end()))
	{
		set_status("Bad Request", 400);
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
