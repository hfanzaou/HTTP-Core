/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Post.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebensalt <ebensalt@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/14 10:07:32 by ebensalt          #+#    #+#             */
/*   Updated: 2023/07/18 07:57:27 by ebensalt         ###   ########.fr       */
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
};

#endif
