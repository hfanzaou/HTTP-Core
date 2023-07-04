#include "Response.hpp"
#include <unistd.h>
std::ifstream Response::_file(nullptr);

Response::Response()
{
	//std::cout << "Default Constructor called" << std::endl;
}

Response::Response(request* req, const ServerConfig& config) : _req(req), _config(config), _headers_status(false), _body_status(false), _dir(false), auto_index(false)
{
	std::cout << _req << std::endl;
	req_uri = _req->get_uri();
	std::cout << "3 " << req_uri << std::endl;
	_method = _req->get_method();
	status_code = _req->get_status_code();
	std::cout << "*****" << status_code << std::endl;
	this->_res = "";
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


std::string& Response::get_body()
{
			return (this->_body);
}

std::string& Response::get_res()
{
	return (this->_res);
}

bool		Response::get_send_status()
{
	return (this->_body_status);
}

std::string Response::generate_error(short error)
{
	//std::string ret = "";
	switch(error)
	{
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
		case 400:
			return ("400 Bad Request");
		case 414:
			return ("414 URI Too Long");
		case 501:
			return ("501 Not Implemented");
	}
	return ("200 OK");
}

void	Response::handle_err(int err)
{
		std::string error(this->generate_error(err));
		if (err == 301)
		{
			this->_head = "HTTP/1.1" + error.substr(0, 3) + "\r\nLocation: " + this->req_uri + "/";
			this->_res = this->_head;
			return ;
		}
		std::string error_page = "<!DOCTYPE html><html><body><center><h1>ERROR</h1><h1><font size=\"100\">" 
		+ error.substr(0, 3) + "</font></h1><h1>" + error.substr(3, error.length()) + "</h1></center></body></html>";
		this->_head = "HTTP/1.1 " + error.substr(0, 3) + "\r\nContent-Type: text/html\r\nContent-Length: " + std::to_string(error_page.size()) + "\r\n\r\n" + error_page;
		this->_res = this->_head;
		//this->_body = error_page;
}

void	Response::check_method(const std::vector<std::string>& methods)
{
	for (std::vector<std::string>::const_iterator it2 = methods.cbegin(); it2 != methods.cend(); ++it2)
	{
		if (this->_method == *it2)
		{
			std::cout << " kfdkf "<< *it2 << std::endl;
			return ;
		}
	}
	throw 403;
}

void	Response::index_dir(DIR *dir, std::string& path)
{
	std::ofstream file((path + "/index.html").c_str());
	if (!file.is_open())
		throw 500;
	struct dirent *reader;
	file << "<!DOCTYPE html><html><body>";
	std::string name;
	while ((reader = readdir(dir)))
	{
		name = reader->d_name;
		file << "<a href=\"" + name + "\">" + name + "<br></a>";
	}
	file << "</body></html>";
	this->req_uri = path + "/index.html";
	file.close();
}

void	Response::match()
{
	const ServerConfig& config = Response::get_servconfig();
	const std::vector<Location>& Locations = config.getLocations();
	std::string path;
	for (std::vector<Location>::const_iterator it = Locations.cbegin(); it != Locations.cend(); ++it)
	{
		path = it->getroot();
		this->req_uri = path + this->req_uri;
		std::cout << "55" << this->req_uri << std::endl;
		DIR *dir = opendir(this->req_uri.c_str());
		if (dir)
		{
			if (this->req_uri.at(this->req_uri.length() - 1) != '/')
				throw 301;
			if (it->getIndex() != "")
			{
				std::cout << "catch1" << std::endl;
				this->index = it->getroot() + it->getIndex();
				this->req_uri = this->index;
				std::cout << "req = " << this->req_uri << std::endl;
			}
			else if (it->getAutoIndex())
			{
				std::cout << "catch2" << std::endl;
				this->index_dir(dir, this->req_uri);
				this->auto_index = true;
			}
			else
			{
				closedir(dir);
				throw 403;
			}
			closedir(dir);
		}
		_file.open(this->req_uri, std::ios::binary | std::ios::ate);
		if (_file.is_open())
		{
			std::cout << "catch3" << std::endl;
		//this->req_uri = path;
			this->_content_length = _file.tellg();
			_file.seekg(0, std::ios::beg);
			// std::cout << "1" << it->getroot() << std::endl;
			// std::cout << "2" << this->req_uri << std::endl;
			this->check_method(it->getMethods());
			//std::cout << "heheh" << std::endl;
			return ;	
		}
	}
	std::cout << "heheh" << std::endl;
	throw 404;
}


void Response::generate_response()
{
	//std::string errors[7] = {"404, 301, 403, 500, 409, 204, 201"};
	try 
	{
		if (status_code != 200)
		{
			_headers_status = true;
			_body_status = true;
			std::cout << "here*****" << this->status_code << std::endl;
			throw this->status_code;
		}
		//std::cout << "hello" << std::endl;
		
		if (_headers_status == false)
		{
			this->match();
		}
		//std::cout << this->req_uri << std::endl;
		if (_body_status == false)
			this->handle();
	}
	catch(int err)
	{
		_file.close();
		std::cout << "catch" << std::endl;
		_headers_status = true;
		_body_status = true;
		this->handle_err(err);
	}
}

// std::ifstream& Response::get_file()
// {
// 	this->_file.open(this->_req->get_uri())
// 	if (!this->_file.is_open())
// 		throw 404;
// 	return (this->_file);
// }

// int	Response::match_fd()
// {
// 	if (this->_client.find(this->_fd) == this->_client.end())
// 	{
// 		std::pair<std::ifstream, size_t> pair(get_file(), 0);
// 		this->_client.insert(std::pair<int, std::pair<std::ifstream, size_t> >(this->_fd, (get_file(), 0)));
// 	}
// 	else

// }
// void	Response::send_chunked(int fd)
// {
// 	//size_t chunk = 1000;

// 	if (this->_body.size() < 1000)
// 	{
// 		//send(i, &this->_body[0], this->_body.size(), 0);
// 		return ;
// 	}

// }

std::string get_file_type(std::string file)
{
	std::vector<std::string> types;
	types.push_back("html");
	types.push_back("htm");
	types.push_back("shtml");
	types.push_back("css");
	types.push_back("text");
	types.push_back("jpeg");
	types.push_back("jpg");
	types.push_back("png");
	types.push_back("js");
	types.push_back("json");
	types.push_back("mp4");
	types.push_back("webm");
	std::vector<std::string> cont_types;
	cont_types.push_back("text/html");
	cont_types.push_back("text/html");
	cont_types.push_back("text/html");
	cont_types.push_back("text/css");
	cont_types.push_back("text/plain");
	cont_types.push_back("image/jpeg");
	cont_types.push_back("image/jpeg");
	cont_types.push_back("image/png");
	cont_types.push_back("application/javascript");
	cont_types.push_back("application/json");
	cont_types.push_back("video/mp4");
	cont_types.push_back("video/webm");
	std::vector<std::string>::iterator it2 = cont_types.begin();
	for (std::vector<std::string>::iterator it = types.begin(); it != types.end(); ++it)
	{

		if (file.find(*it) != std::string::npos)
			return (*it2);
		++it2;	
	}
	return ("text/html");
}

std::string Response::set_head()
{
	std::string head = "";
	head += "HTTP/1.1 200 OK\r\n";
	head += "Content-Type: " + get_file_type(this->req_uri) + "\r\n";
	head += "Content-Length: " + std::to_string(this->_content_length) + "\r\n";
	//head += "Connection: close\r\n";
	return (head);
}
void Response::handle()
{
	// std::string location = "";
	// std::string location2 = "error.html";
	// std::string location3 = "20MIN.mp4";
	// DIR *dir = opendir(this->req_uri.c_str());
	// if (dir)
	// {
	// 	_file.close();
	// 	if (this->req_uri.at(this->req_uri.length() - 1) != '/')
	// 	{
	// 		this->req_uri += "/";
	// 		closedir(dir);
	// 		throw 301;
	// 	}
	// 	else if (this->index != "")
	// 	{
			
	// 	}	
	// }
	if (_headers_status == false)
	{
		std::cout << "here" << std::endl;
		//_file.open(this->req_uri, std::ios::binary | std::ios::ate);
		// if (!_file.is_open())
		// {
		// 	std::cout << this->req_uri << "----" << std::endl;
		// 	throw 404;
		// }
		_headers_status = true;
		this->_res = set_head() + "\r\n";
		std::cout << this->_res << std::endl;
		if (!_file.is_open())
			throw 404;
		//std::cout << this->_res << std::endl;
		return;
	}
	//std::cout << "hdhdhhd" << std::endl;
	if (_body_status == false)
	{
		int l = 255;
		if (this->_content_length - _file.tellg() < 255)
			l = this->_content_length - _file.tellg() + 1;
		std::vector<char> vec(l);
		_file.read(&vec[0], l);
		//std::cout << "--- :" << vec.size() << std::endl;
		//std::vector<char> vec2(this->_head.c_str());
		std::string res(vec.begin(), vec.end());
		this->_res = res;
		if (_file.eof())
		{
			this->_body_status = true;
			_file.close();
			if (this->auto_index)
				std::remove(this->req_uri.c_str());
		}
	}	
	//this->_body = vec;
}
