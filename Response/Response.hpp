#ifndef RESPONSE_HPP
#define	RESPONSE_HPP
#include "iostream"
#include <map>
#include <vector>
#include <sstream>
#include <fstream>
#include "../request/request.hpp"
#include "../ConfigParse/Config.hpp"
#include <dirent.h>
#include <sys/stat.h>
#include <utility>
#include <cstdio>
#include <ctime>
#include "../cgi/cgi.hpp"
#include <stdio.h>

#define DEBUG false

class Response {
	protected :
		request 				&_req;
		const ServerConfig 		&_config;
		std::string 			req_uri;
		const std::string 		_method;
		int 					status_code;
		bool					_headers_status;
		bool					_body_status;
		bool 					auto_index;
		std::string 			index;
		int 					_fd;
		std::string 			_head;
		std::string 			_body;
		std::string 			_res;
		std::ifstream 			_file;
		bool 					Allow_method;
		bool 					redirect;
		bool 					_cgi;
		DIR 					*_dir;
		size_t 					_pos;
		std::ifstream::pos_type _content_length;
		std::ifstream::pos_type c;
	public :
		Response(request&, const ServerConfig&);
		Response(const Response& );
		virtual ~Response();
		//Response& operator=(const Response& );



		const 			ServerConfig&	get_servconfig() const;
		size_t 			get_len() const;
		const			std::string& get_head() const;
		std::string& 	get_body();
		std::string& 	get_res();
		bool			get_send_status();
		void			generate_response();
		void			match();
		void			handle_err(int err);
		void			handle_get();
		void			handle_delete(DIR* dir, std::string rq);
		void			check_method(const std::vector<std::string>& methods);
		void			handle_dir(std::vector<Location>::const_iterator& it, std::string& path1);
		void			index_dir(DIR *dir, std::string& path);
		std::string		set_head();
		std::ifstream& 	get_file();
		void			Drop_file();
		int				get_code();
};

enum
{
	OK = 200,
	NOT_FOUND = 404, 
	MV_PERM = 301, 
	FORBIDDEN = 403,
	CREATED = 201,
	NO_CONTENT = 204,
	IN_SERVERROR = 500,
	CONFLICT = 409,
	NOT_ALLOWED = 405,
	BAD_REQUEST = 400,
	LONG_URI = 414,
	NOT_IMPLEM = 501,
	FOUND = 302,
	BAD_GATEWAY = 502,
	PAYLOAD_TOLARGE = 413, 
};


std::string		get_type(std::string file);
std::string		ft_time();
std::string		generate_error(short error);

#endif