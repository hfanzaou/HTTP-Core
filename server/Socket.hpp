/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebensalt <ebensalt@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/08 16:56:52 by ebensalt          #+#    #+#             */
/*   Updated: 2023/07/09 08:32:38 by ebensalt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_HPP
# define SOCKET_HPP
# include <iostream>
# include <vector>

class Socket
{
	private	:
		int							fd;
		std::string					host;
	public	:
		Socket(int f, std::string &h);

		int							get_fd(void);
		std::string					&get_host(void);
};

#endif
