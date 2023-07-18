/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Post.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebensalt <ebensalt@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/14 10:07:43 by ebensalt          #+#    #+#             */
/*   Updated: 2023/07/15 22:04:56 by ebensalt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Post.hpp"

Post::Post(void) : i(0), bytes_read(0), file_open(false), chunk_size(0), exp(true) {}

int	Post::get_i(void)
{
	return (i);
}

void	Post::set_i(int a)
{
	i = a;
}

std::ofstream	&Post::get_file(void)
{
	return (*file);
}

void	Post::set_file(std::string s)
{
	file = new std::ofstream(s, std::ios::binary);
}

void	Post::delete_file(void)
{
	delete file;
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

bool	Post::get_file_open(void)
{
	return (file_open);
}

void	Post::set_file_open(bool f)
{
	file_open = f;
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
