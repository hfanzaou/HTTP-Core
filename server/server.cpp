/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hfanzaou <hfanzaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/14 23:24:46 by ebensalt          #+#    #+#             */
/*   Updated: 2023/07/10 10:22:04 by hfanzaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

// server::server(Config c) : config(c) {}

void	server::init_server(void)
{
	hostname = "127.0.0.1";
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
	// req_l = false;
	// req_h = false;
	// req_b = true;
	// body_len = 0;
	// hostname = config.getServers()[0].getHost();
	// servname = "8080";
	// memset(&hints, 0, sizeof(hints));
	// hints.ai_family = AF_INET;
	// hints.ai_socktype = SOCK_STREAM;
	// option_value = 1;
	// option_len = sizeof(option_value);
	// memset(&acpt_addr, 0, sizeof(acpt_addr));
	// acpt_len = sizeof(acpt_addr);
	// FD_ZERO(&read);
	// FD_ZERO(&write);
	// req_l = false;
	// req_h = false;
	// req_b = true;
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
	signal(SIGPIPE, SIG_IGN);
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
		if (select((nfds + 1), &tmp_r, &tmp_w, NULL, &tv) == -1)
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
						if (this->response.find(i) == this->response.end())
							this->response.insert(std::pair<int, Response*>(i, new Response(reqs[find_req(i)], config)));	
						// std::cout << "ok" << std::endl;
						FD_SET(i, &write);
						// FD_CLR(i, &read);
					}
				}
			}
			else if (FD_ISSET(i, &tmp_w))
			{
				// std::cout << "ok" << std::endl;
				FD_CLR(i, &read);
				write_server(i);
				// FD_CLR(i, &write);
			}
		}
	}
}

void	server::accept_server(void)
{
	std::cout << "\033[1;32mNEW CONECTION\033[0m" << std::endl;
	if ((acpt_fd = accept(sock_fd, (sockaddr *)&acpt_addr, (socklen_t *)&acpt_len)) == -1)
	{
		std::cerr << "Error : accept!" << std::endl;
		return ;
	}
	reqs.push_back(request(acpt_fd));
	// req = new request(acpt_fd);
}

bool	server::read_server(int i)
{
	// buff = new char[10240];
	if ((read_len = recv(i, buff, 102400, 0)) < 1)
	{
		std::cerr << "Error : recv!" << std::endl;
		return (false);
	}
	// std::cout << buff << std::endl;
	// buff[j] = 0;
	// read_line += buff;
	reqs[find_req(i)].add_to_read_line(buff);
	// if (body_len > 0)
	// 	body_len -= j;
	// if (!find_req(i))
	// {
		// std::copy(buff, buff + read_len, std::back_inserter(reqs[find_req(i)].get_read_line()));
		return (parse_req(i));
	// // }
	// else
	// 	return (false);
}

void	server::write_server(int i)
{
	response[i]->generate_response();
	std::string res = response[i]->get_res();
	if (!res.empty())
	{
		//std::cout << res << std::endl;
		if (send(i, res.c_str(), res.size(), 0) == -1)
		{
			//std::cout << "mehdi l7imar " << std::endl; 
			//std::cerr << "Error : send!" << std::endl;
			return ;
		}
	}
	if (response[i]->get_send_status())
	{
		// std::cout << "lmhidi l7imari" << std::endl;
 		delete response[i];
		response.erase(i);
		erase_req(i);
		FD_CLR(i, &write);
		//delete req;
		close(i);
	}
	//std::cout << "fga" << std::endl;
	// std::cout << "ok" << std::endl;
}

bool	server::parse_req(int i)
{
	std::string	read_line = reqs[find_req(i)].get_read_line();
	if (read_line.find("\r\n") == std::string::npos && (!reqs[find_req(i)].get_req_l() || !reqs[find_req(i)].get_req_h()))
	{
		return (false);
	}
	if (!reqs[find_req(i)].get_req_l())
		parse_req_line(i);
	if (!reqs[find_req(i)].get_req_h())
		parse_header(i);
	if (!reqs[find_req(i)].get_req_b())
		post(i);
	// std::cout << req_l << " " << req_h << " " << req_b << " " << std::endl;
	if (reqs[find_req(i)].get_req_l() && reqs[find_req(i)].get_req_h() && reqs[find_req(i)].get_req_b()/* && body_len <= 0*/)
	{
		// reqs.push_back(*req);
		read_line.erase();
		reqs[find_req(i)].set_read_line(read_line);
		// req_l = false;
		// req_h = false;
		// req_b = true;
		// reqs[find_req(i)].print_all();
		// erase_req(i);
		// delete req;
		return (true);
	}
	return (false);
}

void	server::parse_req_line(int i)
{
	// req = new request(i);
	reqs[find_req(i)].set_req_l(true);
	std::stringstream	ss(reqs[find_req(i)].get_read_line());
	std::string			m;
	std::string			u;
	std::string			h;

	ss >> m >> u >> h;
	if ((m != "GET" && m != "POST" && m != "DELETE") || h != "HTTP/1.1" || u.find_first_not_of("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-._~:/?#[]@!$&'()*+,;=%") != std::string::npos)
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
	reqs[find_req(i)].set_request_uri(u);
	std::stringstream	ss0;

	getline(ss, m);
	ss0 << ss.rdbuf();
	// read_line = ss0.str();
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

void	server::parse_header(int i)
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
			// std::stringstream	ss0;

			// ss0 << ss.rdbuf();
			// read_line = ss0.str();
			// read_line.erase();
			// std::cout << read_line << std::endl;
			check_header(h, i);
			reqs[find_req(i)].set_header(h);
			return ;
		}
	}
}

void	server::check_header(std::map<std::string, std::string> &h, int fd)
{
	std::map<std::string, std::string>::iterator	i = h.find("Transfer-Encoding");
	std::map<std::string, std::string>::iterator	j = h.find("Content-Length");

	// if (j != h.end())
	// {
	// 	std::stringstream	ss(j->second);

	// 	ss >> body_len;
	// 	body_len = body_len - read_line.size();
	// }
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

void	server::post(int fd)
{
	int i = 0;

	for (; i < read_len; i++)
	{
		if (!strncmp(&(buff[i]), "\r\n\r\n", 4)) {
			i += 4;
			break ;
		}
	}
	if (i == read_len)
		i = 0;

	std::ofstream file("test", std::ios_base::app | std::ios_base::binary);
	if (file.is_open())
	{
		file.write(&(buff[i]), read_len - i);
		// delete[] buff;
		file.seekp(0, file.end);
		long size = file.tellp();
		if (size >= std::stoi(reqs[find_req(fd)].get_header()["Content-Length"])) {
			reqs[find_req(fd)].set_req_b(true);
			file.close();
		}
	}
}

