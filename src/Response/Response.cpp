#include "Response.hpp"

Response::Response()
{
	//std::cout << "Default Constructor called" << std::endl;
}

Response::Response(const ServerConfig& config) : _config(config)
{
	//std::cout << "Constructor called" << std::endl;
}

Response::Response(const Response& obj) 
{
	//std::cout << "Copy Constructor called" << std::endl;
	*this = obj;
}

Response& Response::operator=(const Response& obj)
{
	//std::cout << "Copy assignement constructor called" << std::endl;
	if (this != &obj)
	{
		this->_config = obj.get_servconfig();
	}
	return (*this);
}

Response::~Response()
{
	//std::cout << "Destructor called" << std::endl;
}

void	Response::set_servconfig(const ServerConfig& config)
{
	this->_config = config;
}

const  ServerConfig&	Response::get_servconfig() const
{
	return (this->_config);
}
size_t Response::get_len() const
{
	return (this->_content_length);
}

const std::string& Response::get_head() const
{
	return (this->_head);
}

const std::vector<char>& Response::get_body() const
{
	return (this->_body);
}

std::string Response::generate_error(short error)
{
	//std::string ret = "";
	switch(error)
	{
		case OK:
			return ("200 OK");
		case NOT_FOUND:
			return ("404 Not Found");
		case MV_PERM:
			return ("301 Moved Permanently");
		case FORBIDDEN:
			return ("403 Forbidden"); 					
		case CREATED:
			return ("201 Created");
		case NO_CONTENT:
			return ("204 No Content");
		case IN_SERVERROR:
			return ("500 Internal Server Error");
		case CONFLICT:
			return ("409 Conflict");
		case NOT_ALLOWED:
			return ("405 Method Not Allowed");			
	}
	return ("500 Internal Server Error");
}

void	Response::match_method()
{
	std::string path =  "/1.png";
	std::string method = "GET";
	const ServerConfig& config = Response::get_servconfig();
	const std::vector<Location>& Locations = config.getLocations();
	for (std::vector<Location>::const_iterator it = Locations.cbegin(); it != Locations.cend(); ++it)
	{
		std::cout << it->getPath() << std::endl;
		if (path == it->getPath())
		{
			this->req_path = it->getroot() + path;
			const std::vector<std::string>& methods = it->getMethods();
			for (std::vector<std::string>::const_iterator it2 = methods.cbegin(); it2 != methods.cend(); ++it2)
			{
				if (method == *it2)
				{
					std::cout << *it2 << std::endl;
					return ;
				}
			}
			throw 405;
		}
	}
	throw 404;
}
// void	Response::send_chunked(int fd)
// {
// 	//size_t chunk = 1000;

// 	if (this->_body.size() < 1000)
// 	{
// 		//send(i, &this->_body[0], this->_body.size(), 0);
// 		return ;
// 	}

// }