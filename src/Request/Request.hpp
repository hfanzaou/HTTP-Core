/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebensalt <ebensalt@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/10 20:50:00 by ebensalt          #+#    #+#             */
/*   Updated: 2023/06/11 21:00:09 by ebensalt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
# define REQUEST_HPP
# include <iostream>

class Request
{
	private	:
		std::string	req;
		int			fd;
	public	:
		Request(void);
		Request(const Request &o);
		~Request(void);
		Request(std::string r, int f);

		Request	&operator=(const Request &o);

		void	print_req(void);
		int		get_fd(void);
};

#endif
