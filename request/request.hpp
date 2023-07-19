/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebensalt <ebensalt@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/15 21:05:30 by ebensalt          #+#    #+#             */
/*   Updated: 2023/07/17 08:43:45 by ebensalt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
# define REQUEST_HPP
# include <string>
# include <map>
# include <iostream>
# include <sstream>
# include <string>
# include <unistd.h>
# include <vector>
# include "Post.hpp"

class request
{
	private	:
		std::string							read_line;
		std::string							method;
		std::string							request_uri;
		std::string							http_version;
		std::map<std::string, std::string>	header;
		std::string							msg;
		int									status_code;
		int									fd;
		bool								req_l;
		bool								req_h;
		bool								req_b;
		std::string							host;
		Post								post;
		std::string							name;
		std::string							query;
	public	:
		request(int i, std::string &h);
		~request(void);

		void								set_method(std::string &m);
		void								set_request_uri(std::string &r);
		void								set_http_version(std::string &h);
		void								set_msg(const std::string &m);
		void								set_status_code(int s);
		void								set_header(std::map<std::string, std::string> &h);
		void								print_all(void);
		int									get_fd(void);
		bool								get_req_l(void);
		bool								get_req_h(void);
		bool								get_req_b(void);
		void								set_req_l(bool r);
		void								set_req_h(bool r);
		void								set_req_b(bool r);
		std::map<std::string, std::string>	&get_header(void);
		std::string							&get_read_line(void);
		void								set_read_line(std::string r);
		void								add_to_read_line(char *b);
		std::string							&get_method(void);
		Post								&get_post(void);
		std::string							&get_name(void);
		std::string							&get_query(void);
		void								set_name(std::string &n);
		void								set_query(std::string &q);
		std::string							&get_uri();
		int 								get_status_code();
		std::string							&get_host();
};

#endif
