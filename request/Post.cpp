/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Post.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebensalt <ebensalt@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/14 10:07:43 by ebensalt          #+#    #+#             */
/*   Updated: 2023/07/20 03:38:54 by ebensalt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Post.hpp"

Post::Post(void) : i(0), bytes_read(0), chunk_size(0), exp(true), file(0), body_size(0) {}

int	Post::get_i(void)
{
	return (i);
}

void	Post::set_i(int a)
{
	i = a;
}

std::vector<char>	&Post::get_body(void)
{
	return (body);
}

std::string	&Post::get_hex(void)
{
	return (hex);
}

void	Post::set_hex(std::string h)
{
	hex = h;
}

int	Post::get_bytes_read(void)
{
	return (bytes_read);
}

void	Post::set_bytes_read(int b)
{
	bytes_read = b;
}

int	Post::get_chunk_size(void)
{
	return (chunk_size);
}

void	Post::set_chunk_size(int f)
{
	chunk_size = f;
	i = chunk_size;
}

bool	Post::get_exp(void)
{
	return (exp);
}

void	Post::set_exp(bool f)
{
	exp = f;
}

int	Post::get_file(void)
{
	return (file);
}

void	Post::set_file(int f)
{
	file = f;
}

unsigned long	Post::get_body_size(void)
{
	return (body_size);
}

void	Post::set_body_size(unsigned long u)
{
	body_size = u;
}

std::string	Post::get_file_name(void)
{
	return (file_name);
}

void	Post::set_file_name(std::string f)
{
	file_name = f;
}
