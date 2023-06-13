#include "Response.hpp"

Response::Response()
{
	//std::cout << "Default Constructor called" << std::endl;
}

Response::Response(const std::map<std::string, std::map<std::string, std::string> >& Routes) : _Routes(Routes)
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
		this->_Routes = obj.get_Routes();
	}
	return (*this);
}

Response::~Response()
{
	//std::cout << "Destructor called" << std::endl;
}

void	Response::set_Routes(const std::map<std::string, std::map<std::string, std::string> >& Routes)
{
	this->_Routes = Routes;
}

const  std::map<std::string, std::map<std::string, std::string> >&	Response::get_Routes() const
{
	return (this->_Routes);
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


// void	Response::send_chunked(int fd)
// {
// 	//size_t chunk = 1000;

// 	if (this->_body.size() < 1000)
// 	{
// 		//send(i, &this->_body[0], this->_body.size(), 0);
// 		return ;
// 	}

// }