#include "Get.hpp"
#include <fstream>
Get::Get() : Response()
{
	//std::cout << "Default Constructor called" << std::endl;
}

Get::Get(const ServerConfig& config) : Response(config)
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
		this->set_servconfig(obj.get_servconfig());
	return (*this);
}

Get::~Get()
{
	//std::cout << "Destructor called" << std::endl;
}

void	Get::handle_request()
{
	
}

// std::string res_error(int i, const std::string[7]& errors)
// {
// 	std::string error("errors/"+ errors[i]);
// 	std::ifstream errorf(error.c_str(), std::ios::ate);
// 	size_t len = errorf.tellg();
// 	errorf.seekg(0, std::ios::beg);
// 	char *buff = new char[len];
// 	errorf.read(buff, len);
// 	std::string error(buff);
// 	return (error);
// }


void Get::generate_response()
{
	//std::string errors[7] = {"404, 301, 403, 500, 409, 204, 201"};
	try 
	{
		this->match_method(int fd);
		//std::cout << this->req_path << std::endl;
		this->handle();
	}
	catch(int err)
	{
		//std::cout << e.what() << std::endl;
		this->handle_err(err);
	}
}

void Get::handle()
{
	// std::string location = "";
	// std::string location2 = "error.html";
	// std::string location3 = "20MIN.mp4";
	DIR *dir = opendir(this->req_path.c_str());
	if (dir)
	{
		if (this->req_path.at(this->req_path.length() - 1) != '/')
			throw 301;
	}
	std::ifstream bodyfile(this->req_path, std::ios::binary | std::ios::ate);
	if (!bodyfile.is_open())
		throw 404;
	this->_content_length = bodyfile.tellg();
   	bodyfile.seekg(0, std::ios::beg);
	std::vector<char> vec(this->_content_length);
	bodyfile.read(&vec[0], this->_content_length);
	//std::cout << "--- :" << vec.size() << std::endl;
	this->_head = "HTTP/1.1 200 OK\r\nContent-Type: image/png\r\nContent-Length: " + std::to_string(vec.size()) + "\r\n\r\n";
	//std::vector<char> vec2(this->_head.c_str());
	std::string res(vec.begin(), vec.end());
	this->res = this->_head + res;
	//this->_body = vec;
}
