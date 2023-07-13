/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebensalt <ebensalt@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/08 16:57:02 by ebensalt          #+#    #+#             */
/*   Updated: 2023/07/09 08:32:53 by ebensalt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Socket.hpp"

Socket::Socket(int f, std::string &h) : fd(f), host(h) {}

int	Socket::get_fd(void)
{
	return (fd);
}

std::string	&Socket::get_host(void)
{
	return (host);
}
