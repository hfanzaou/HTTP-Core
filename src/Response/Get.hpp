#ifndef GET_HPP
#define GET_HPP
#include "Response.hpp"

class Response;
class Get : public Response {
	public :
		Get();
		Get(const std::map<std::string, std::map<std::string, std::string> >& );
		Get(const Get& );
		Get& operator=(const Get& obj);
		~Get();
		void	handle_request();
		void generate_response();
};

#endif