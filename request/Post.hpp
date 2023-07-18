/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Post.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebensalt <ebensalt@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/14 10:07:32 by ebensalt          #+#    #+#             */
/*   Updated: 2023/07/15 22:05:31 by ebensalt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef POST_HPP
# define POST_HPP
# include <fstream>
# include <iostream>
# include <vector>

class Post
{
	private	:
		int					i;
		std::ofstream		*file;
		std::string			hex;
		std::vector<char>	body;
		int					bytes_read;
		bool				file_open;
		int					chunk_size;
		bool				exp;
	public	:
		Post(void);

		int					get_i(void);
		void				set_i(int a);
		std::ofstream		&get_file(void);
		void				set_file(std::string s);
		void				delete_file(void);
		std::vector<char>	&get_body(void);
		std::string			&get_hex(void);
		void				set_hex(std::string h);
		int					get_bytes_read(void);
		void				set_bytes_read(int b);
		bool				get_file_open(void);
		void				set_file_open(bool f);
		int					get_chunk_size(void);
		void				set_chunk_size(int f);
		bool				get_exp(void);
		void				set_exp(bool f);
};

#endif
