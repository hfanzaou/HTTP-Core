#ifndef GET_HPP
#define GET_HPP
#include "Response.hpp"

class Response;
class Get : public Response {
	public :
		Get();
		Get(const ServerConfig& );
		Get(const Get& );
		Get& operator=(const Get& obj);
		~Get();
		void	handle_request();
		void	generate_response();
		void	handle();
		void	match();
};

#endif