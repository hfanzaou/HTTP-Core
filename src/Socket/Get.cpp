#include "Get.hpp"
#include <fstream>
Get::Get() : Response()
{
	//std::cout << "Default Constructor called" << std::endl;
}

Get::Get(const std::map<std::string, std::map<std::string, std::string> >& Routes) : Response(Routes)
{
	//std::cout << "Constructor called" << std::endl;
}

Get::Get(const Get& obj) 
{
	//std::cout << "Copy Constructor called" << std::endl;
	*this = obj;
}

Get& Get::operator=(const Get& obj)
{
	//std::cout << "Copy assignement constructor called" << std::endl;
	if (this != &obj)
		this->set_Routes(obj.get_Routes());
	return (*this);
}

Get::~Get()
{
	//std::cout << "Destructor called" << std::endl;
}

void	Get::handle_request()
{
	
}

void Get::generate_response()
{
	std::string location = "index.html";
	std::string location2 = "1.png";
	std::ifstream bodyfile(location2.c_str(), std::ios::binary | std::ios::ate);
	if (!bodyfile.is_open())
		throw std::runtime_error("kda");
	this->_content_length = bodyfile.tellg();
   	bodyfile.seekg(0, std::ios::beg);
	std::cout << "--- :" << this->_content_length << std::endl;
	std::vector<char> vec(this->_content_length);
	bodyfile.read(&vec[0], this->_content_length);
	this->_head = "HTTP/1.1 200 OK\r\nContent-Type: image/png\nContent-Length: " + std::to_string(vec.size()) + "\n\r\n";
	this->_body = vec;
	//this->_content_length += this->_head.length();

	//buff[this->_content_length - head.length()] = '\0';
	//std::cout << res << std::endl;
}
