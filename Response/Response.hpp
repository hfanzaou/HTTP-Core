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
#include <utility>
#include <cstdio>

class Response {
	protected :
		request _req;
		ServerConfig _config;
		std::string req_uri;
		std::string _method;
		int status_code;
		bool _headers_status;
		bool _body_status;
		//bool _dir;
		bool auto_index;
		std::string index;
		int _fd;
		std::string _head;
		std::string _body;
		std::string _res;
		std::ifstream _file;
		bool Allow_method;
		DIR *_dir;
		size_t _pos;
		std::map<int, std::pair<std::ifstream, size_t> > _client;
		std::ifstream::pos_type _content_length;
		size_t _head_len;
		std::ifstream::pos_type c;
	public :
		Response(request, const ServerConfig&);
		Response(const Response& );
		virtual ~Response();
		Response& operator=(const Response& );
		void	set_servconfig(const ServerConfig& );
		const 	ServerConfig&	get_servconfig() const;
		size_t get_len() const;
		const	std::string& get_head() const;
		std::string generate_error(short error);
		std::string& get_body();
		std::string& get_res();
		bool		get_send_status();
		void		generate_response();
		void		match();
		void		match_fd();
		void		handle_err(int err);
		void		handle_get();
		void		handle_delete(DIR* dir, std::string rq);
		void		check_method(const std::vector<std::string>& methods);
		void		index_dir(DIR *dir, std::string& path);
		//static void open(const );
		std::string set_head();
		std::ifstream& get_file();
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
};

#endif