#ifndef RESPONSE_HPP
#define	RESPONSE_HPP
#include "iostream"
#include <map>
#include <vector>
#include <sstream>
#include <fstream>
#include "../Request/Request.hpp"
#include "../config/ConfigParse/Config.hpp"
#include <dirent.h>

class Response {
	protected :
		ServerConfig _config;
		std::vector<std::string> _request;
		std::string req_path;
		std::string _head;
		std::vector<char> _body;
		std::ifstream::pos_type _content_length;
		size_t _head_len;
		std::string res;
	public :
		Response();
		Response(const ServerConfig& );
		Response(const Response& );
		virtual ~Response();
		Response& operator=(const Response& );
		void	set_servconfig(const ServerConfig& );
		const 	ServerConfig&	get_servconfig() const;
		size_t get_len() const;
		const	std::string& get_head() const;
		const	std::vector<char>& get_body() const;
		void	match_method();
		std::string generate_error(short error);
		std::string& get_res()
		{
			return (this->res);
		}
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