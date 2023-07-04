/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hfanzaou <hfanzaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/15 21:05:30 by ebensalt          #+#    #+#             */
/*   Updated: 2023/06/27 06:00:17 by hfanzaou         ###   ########.fr       */
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

class request
{
	private	:
		std::string							method;
		std::string							request_uri;
		std::string							http_version;
		std::map<std::string, std::string>	header;
		std::string							body;
		// int									fd;
		std::string							msg;
		int									status_code;
	public	:
		request(void);
		~request(void);

		void		set_method(std::string &m);
		void		set_request_uri(std::string &r);
		void		set_http_version(std::string &h);
		void		set_msg(const std::string &m);
		void		set_status_code(int s);
		void		set_header(std::map<std::string, std::string> &h);
		void		print_all(void);
		std::string get_uri()
		{
			return (request_uri);
		}
		std::string get_method()
		{
			return (method);
		}
		int get_status_code()
		{
			return (status_code);
		}
};

#endif
