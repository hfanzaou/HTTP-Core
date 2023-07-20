/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Post.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebensalt <ebensalt@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/14 10:07:32 by ebensalt          #+#    #+#             */
/*   Updated: 2023/07/20 03:37:56 by ebensalt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef POST_HPP
# define POST_HPP
# include <iostream>
# include <vector>
# include <unistd.h>

class Post
{
	private	:
		int					i;
		std::string			hex;
		std::vector<char>	body;
		int					bytes_read;
		int					chunk_size;
		bool				exp;
		int					file;
		unsigned long		body_size;
		std::string			file_name;
	public	:
		Post(void);

		int					get_i(void);
		void				set_i(int a);
		std::vector<char>	&get_body(void);
		std::string			&get_hex(void);
		void				set_hex(std::string h);
		int					get_bytes_read(void);
		void				set_bytes_read(int b);
		int					get_chunk_size(void);
		void				set_chunk_size(int f);
		bool				get_exp(void);
		void				set_exp(bool f);
		int					get_file(void);
		void				set_file(int f);
		unsigned long		get_body_size(void);
		void				set_body_size(unsigned long u);
		std::string			get_file_name(void);
		void				set_file_name(std::string f);
};

#endif
