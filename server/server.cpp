/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hfanzaou <hfanzaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/14 23:24:46 by ebensalt          #+#    #+#             */
/*   Updated: 2023/07/15 08:28:49 by hfanzaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

server::server(Config &c) : config(c)
{
	FD_ZERO(&read);
	FD_ZERO(&write);
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
	// chunkSize = 0;
	bytes_read = 0;
	name = s.getServerNames();
	chunk = false;
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
	if (listen(s, 100))
	{
		std::cerr << "Error : listen!" << std::endl;
		return ;
	}
	std::cout << "\033[1;31mSERVER " << hostname << ":" << servname << " IS LISTENING" << "\033[0m" << std::endl;
	FD_SET(s, &read);
	FD_SET(s, &write);
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
						// response[i]->Drop_file();
						// delete response[i];
						// response.erase(i);
						Drop_Response(i);
						this->response.insert(std::pair<int, Response*>(i, new Response(reqs[find_req(i)], get_config(reqs[find_req(i)].get_host()))));
					}
					FD_SET(i, &write);
				}
			}
			else if (FD_ISSET(i, &tmp_w))
			{

				if (this->response.find(i) == this->response.end())
				{
					this->response.insert(std::pair<int, Response*>(i, new Response(reqs[find_req(i)], get_config(reqs[find_req(i)].get_host()))));
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
	if ((read_len = recv(i, buff, 1024, 0)) < 1)
	{
		
		std::cerr << "Error : recv!" << std::endl;
		if (this->response.find(i) != this->response.end())
		{
			// response[i]->Drop_file();
			// delete response[i];
			// response.erase(i);
			Drop_Response(i);
			this->response.insert(std::pair<int, Response*>(i, new Response(reqs[find_req(i)], get_config(reqs[find_req(i)].get_host()))));
		}
		drop_client(i);
		return (false);
	}
	bytes_read += read_len;
	if (reqs[find_req(i)].get_req_l() && reqs[find_req(i)].get_req_h() && reqs[find_req(i)].get_req_b())
		return (false);
	if (!reqs[find_req(i)].get_req_h())
		reqs[find_req(i)].add_to_read_line(buff);
	return (parse_req(i));
}

void	server::write_server(int i)
{
	// while (!response[i]->get_send_status())
	// {
		response[i]->generate_response();
		std::string res = response[i]->get_res();
		//std::cout << res << std::endl;
		if (send(i, res.c_str(), res.size(), 0) == -1)
		{
			//std::cout << "mehdi l7imar " << std::endl; 
				std::cerr << "Error : send!" << std::endl;
			return ;
		}
	// }
	if (response[i]->get_send_status())
	{
		// std::cout << "lmhidi l7imari" << std::endl;
		// response[i]->Drop_file();
 		// delete response[i];
		// response.erase(i);
		Drop_Response(i);
		// erase_req(i);
		//delete req;
		//close(i);
		//reqs[find_req(i)].print_all();
		// std::cout << "this fd is droped = " << reqs[find_req(i)].get_fd() << std::endl;
		drop_client(i);
	}
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
	{
		std::cout << "done" << std::endl;
		return (true);
	}
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
	}
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
			reqs[find_req(i)].set_header(h);
			int	j = 0;

			for (; j < read_len; j++)
			if (j + 3 < read_len && buff[j] == '\r' && buff[j + 1] == '\n' && buff[j + 2] == '\r' && buff[j + 3] == '\n')
			{
				j += 4;
				bytes_read -= j;
				break ;
			}
			return (j);
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
	FD_CLR(i, &write);
	erase_req(i);
}

// int	server::read_chunkSize(int ind)
// {
// 	chunkSize = 0;
// 	for (; ind < read_len && strncmp(&buff[ind], "\r\n", 2); ind++)
// 	{
// 		std::stringstream	ss;
// 		int					tmp = 0;
// 		ss << buff[ind];
// 		ss >> std::hex >> tmp;
// 		chunkSize *= 16;
// 		chunkSize += tmp;
// 	}
// 	return (ind + 2);
// }

// void	server::handleChuncked(int fd, int ind)
// {
// 	if (bytes_read == 0)
// 		ind = read_chunkSize(ind);

// 	bytes_read += read_len;
// 	for (; ind < read_len; ind++)
// 	{
// 		if (chunk.size() == chunkSize)
// 		{
// 			file.write(&chunk[0], chunk.size());
// 			chunk.clear();
// 			ind += 2;
// 			ind = read_chunkSize(ind);
// 		}
// 		if (chunkSize == 0)
// 		{
// 			reqs[find_req(fd)].set_req_b(true);
// 			bytes_read = 0;
// 			file.close();
// 			return ;
// 		}
// 		chunk.push_back(buff[ind]);
// 	}
// }

// void	server::openFile(int fd)
// {
// 	std::map<std::string, std::string>::iterator it = reqs[find_req(fd)].get_header().find("Content-Type");
// 	size_t	pos = it->second.find('/', 0);
// 	std::string name = "test." + it->second.substr(pos + 1);

// 	file.open(name.c_str(), std::ios::binary);
// 	if (file.is_open() == false)
// 		std::cout << "Error opening file" << std::endl;
// }

void	server::post(int fd, int j)
{
	static int i = 0;
	// int j = 0;

	// std::cout << "i :" << i << std::endl;
	if (i == 0)
	{
		file = new std::ofstream("test" , std::ios::binary);
		// for (; j < read_len; j++)
		// 	reqs[find_req(fd)].get_body().push_back(buff[j]);
		if (reqs[find_req(fd)].get_header().find("Content-Length") != reqs[find_req(fd)].get_header().end())
		{
			std::stringstream(reqs[find_req(fd)].get_header()["Content-Length"]) >> i;
			file->rdbuf()->pubsetbuf(nullptr, i);
		}
		// else
		// {
		// 	for (size_t it = 0; it < reqs[find_req(fd)].get_body().size(); it++)
		// 	{
		// 		if (it + 1 < reqs[find_req(fd)].get_body().size() && reqs[find_req(fd)].get_body()[it] == '\r' && reqs[find_req(fd)].get_body()[it + 1] == '\n')
		// 		{
		// 			i = std::strtol(hex.c_str(), NULL, 16);
		// 			// std::cout << "i1 : " << i << std::endl;
		// 			hex.clear();
		// 			file->rdbuf()->pubsetbuf(nullptr, i);
		// 			reqs[find_req(fd)].get_body().erase(reqs[find_req(fd)].get_body().begin(), reqs[find_req(fd)].get_body().begin() + it + 2);
		// 			bytes_read -= 2;
		// 			break ;
		// 		}
		// 		hex += reqs[find_req(fd)].get_body()[it];
		// 		bytes_read--;
		// 	}
		// }
	}
	// else
	// 	for (; j < read_len; j++)
	// 		reqs[find_req(fd)].get_body().push_back(buff[j]);
	if (reqs[find_req(fd)].get_header().find("Content-Length") != reqs[find_req(fd)].get_header().end())
	{
		i -= bytes_read;
		file->write(&buff[j], bytes_read);
		bytes_read = 0;
		if (i == 0)
		{
			std::cout << "close" << std::endl;
			file->close();
			file->clear();
			delete file;
			reqs[find_req(fd)].set_req_b(true);
		}
	}
	// else
	// {
	// 	if (i > bytes_read)
	// 	{
	// 		i -= bytes_read;
	// 		file->write(&reqs[find_req(fd)].get_body()[0], bytes_read);
	// 		reqs[find_req(fd)].get_body().clear();
	// 		bytes_read = 0;
	// 	}
	// 	else if (i != 0)
	// 	{
	// 		file->write(&reqs[find_req(fd)].get_body()[0], i);
	// 		i += 2;
	// 		reqs[find_req(fd)].get_body().erase(reqs[find_req(fd)].get_body().begin(), reqs[find_req(fd)].get_body().begin() + i);
	// 		bytes_read -= i;
	// 		i = 0;
	// 	}
	// 	if (i == 0)
	// 	{
	// 		for (size_t it = 0; it < reqs[find_req(fd)].get_body().size(); it++)
	// 		{
	// 		// std::cout << "i :" << i << std::endl;
	// 			if (it + 1 < reqs[find_req(fd)].get_body().size() && reqs[find_req(fd)].get_body()[it] == '\r' && reqs[find_req(fd)].get_body()[it + 1] == '\n')
	// 			{
	// 				i = std::strtol(hex.c_str(), NULL, 16);
	// 				// std::cout << "i2 : " << i << std::endl;
	// 				hex.clear();
	// 				if (i == 0)
	// 				{
	// 					bytes_read = 0;
	// 					std::cout << "close" << std::endl;
	// 					file->close();
	// 					file->clear();
	// 					delete file;
	// 					reqs[find_req(fd)].set_req_b(true);
	// 					return ;
	// 				}
	// 				reqs[find_req(fd)].get_body().erase(reqs[find_req(fd)].get_body().begin(), reqs[find_req(fd)].get_body().begin() + it + 2);
	// 				bytes_read -= 2;
	// 				break ;
	// 			}
	// 			hex += reqs[find_req(fd)].get_body()[it];
	// 			bytes_read--;
	// 		}
	// 	}
	// }
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

int	server::get_chunk_size(std::vector<char> &b)
{
	std::string	hex;

	check_chunk_end(b);
	for (size_t i = 0; i < b.size(); i++)
	{
		// if (b[i] == '\r')
		// {

		// }
		// else
		// 	hex += b[i];
		if (b[i] == '\r')
			std::cout << "\\r" << std::endl;
		else if (b[i] == '\n')
			std::cout << "\\n" << std::endl;
		else
			std::cout << b[i] << std::endl;
	}
	exit(10);
	return (0);
}

bool	server::check_chunk_end(std::vector<char> &b)
{
	std::vector<char>::iterator	i = std::find(b.begin(), b.end(), '\r');

	if (i != b.end())
	{
		size_t	it = std::distance(b.begin(), i) + 1;
		if (it < b.size() && b[it] == '\n')
			return (true);
			
	}
	return (false);
}

ServerConfig server::get_config(std::string &host)
{
	std::vector<ServerConfig> configs = config.getServers();
	for (std::vector<ServerConfig>::iterator it = configs.begin(); it != configs.end(); ++it)
	{
		std::cout << "host = " << host << " it->getHost() = " << it->getHost() << std::endl;
		if (host == it->getHost() + ":" + it->getPort())
			return (*it);
	}
	return (config.getServers()[0]);
}


void	server::Drop_Response(int i)
{
	response[i]->Drop_file();
	delete response[i];
	response.erase(i);
}