#ifndef RESPONSE_HPP
#define	RESPONSE_HPP
#include "iostream"
#include <map>
#include <vector>
#include <sstream>
#include <fstream>
class Response {
	protected :
		std::map<std::string, std::map<std::string, std::string> > _Routes;
		std::vector<std::string> _request;
		std::string _head;
		std::vector<char> _body;
		std::ifstream::pos_type _content_length;
		size_t _head_len;
	public :
		Response();
		Response(const std::map<std::string, std::map<std::string, std::string> >&);
		Response(const Response& );
		virtual ~Response();
		Response& operator=(const Response& );
		void	set_Routes(const std::map<std::string, std::map<std::string, std::string> >& );
		const 	std::map<std::string, std::map<std::string, std::string> >&	get_Routes() const;
		size_t get_len() const;
		const	std::string& get_head() const;
		const std::vector<char>& get_body() const;
};

#endif