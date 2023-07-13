#include "Response.hpp"
#include <unistd.h>
std::ifstream Response::_file(nullptr);

Response::Response(request req, const ServerConfig& config) : _req(req), _config(config), 
	_headers_status(false), _body_status(false), auto_index(false), _fd(req.get_fd()) , Allow_method(false), _dir(NULL)
{
	//std::cout << _req << std::endl;
	req_uri = _req.get_uri();
	// std::cout << _req.get_fd() << " " << req_uri << std::endl;
	_method = _req.get_method();
	status_code = _req.get_status_code();
	// std::cout << "*****" << status_code << std::endl;
	c = 0;
	//this->_res = "";
	//std::cout << "Constructor called" << std::endl;
}


// Response::Response(const Response& obj)
// {
// 	_req = request();
// 	//std::cout << "Copy Constructor called" << std::endl;
// 	*this = obj;
// }

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
			this->_head = "HTTP/1.1 " + error + "\r\nLocation: " + this->req_uri + "/";
			this->_res = this->_head;
			//std::cout << this->_res << std::endl;
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
			this->Allow_method = true;
			//std::cout << " kfdkf "<< *it2 << std::endl;
			return ;
		}
	}
	if (this->_method != "POST" && this->_method != "DELETE" && this->_method != "GET")
		throw 501;
	throw 405;
}

void	Response::index_dir(DIR *dir, std::string& path)
{
	std::ofstream file((path + "0index.html").c_str());
	if (!file.is_open())
	{
		closedir(dir);
		throw 500;
	}
	struct dirent *reader;
	file << "<!DOCTYPE html><html><body>";
	std::string name;
	while ((reader = readdir(dir)))
	{
		name = reader->d_name;
		std::cout << name << std::endl;
		if (name != "0index.html")
			file << "<a href=\"" + name + "/\">" + name + "<br></a>";
	}	
	file << "</body></html>";
	this->req_uri = path + "0index.html";
	file.close();
}

void	Response::match()
{
	const ServerConfig& config = Response::get_servconfig();
	const std::vector<Location>& Locations = config.getLocations();
	std::string path;
	std::string root;
	for (std::vector<Location>::const_iterator it = Locations.cbegin(); it != Locations.cend(); ++it)
	{
		if (it->getPath() != this->req_uri.substr(0, it->getPath().length()))
			continue;
		std::string path1 = this->req_uri;	
		root = it->getroot();
		path = this->req_uri.substr(it->getPath().length(), this->req_uri.length());
		// std::cout << "get path = " << it->getPath() << std::endl;
		// std::cout << "root = " << root << std::endl;
		// std::cout << "path = " << path << std::endl;
		// std::cout << "req sub = " << this->req_uri.substr(0, it->getPath().length()) << std::endl;
		// if (it->getroot() != this->req_uri.substr(0, it->getroot().length()))
		// 	continue;
		if (access((root + path).c_str(), F_OK) == -1)
			continue;
		this->req_uri = root + path;
		//std::cout << "final req = " << this->req_uri << std::endl;
		this->check_method(it->getMethods());
		_dir = opendir(this->req_uri.c_str());
		if (this->_method == "DELETE")
		{
			if (_dir && path1.at(path1.length() - 1) != '/')
			{
				this->req_uri = path1;
				closedir(_dir);
				throw 301;
			}	
			return ;
		}
		
		if (_dir)
		{
			if (path1.at(path1.length() - 1) != '/')
			{
				this->req_uri = path1;
				closedir(_dir);
				throw 301;
			}
			if (it->getIndex() != "")
			{
				//std::cout << "catch1" << std::endl;
				this->index = it->getroot() + it->getIndex();
				this->req_uri = this->index;
				if (access((root + path).c_str(), F_OK) == -1)
					throw 404;
				//std::cout << "req = " << this->req_uri << std::endl;
			}
			else if (it->getAutoIndex())
			{
				//std::cout << "catch2" << std::endl;
				this->index_dir(_dir, this->req_uri);
				this->auto_index = true;
			}
			else
			{
				closedir(_dir);
				throw 403;
			}
			closedir(_dir);
		}
		if (_file.is_open())
			_file.close();
		_file.open(this->req_uri, std::ifstream::binary | std::ifstream::ate);
		//std::cout << "file open =" << this->req_uri << std::endl;
		if (!_file.is_open())
			throw 403;
		else
		{	
			//std::cout << "catch3" << std::endl;
			this->_content_length = _file.tellg();
			_file.seekg(0, std::ios::beg);
			// _file.close();
			return ;	
		}
	}
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
			//std::cout << "here*****" << this->status_code << std::endl;
			throw this->status_code;
		}
		if (_headers_status == false)
		{
			c = 0;
			this->match();
			//std::cout << "here" << std::endl;
			_headers_status = true;
			this->_head = set_head() + "\r\n";
			this->_res = this->_head;
			//std::cout << this->_res << std::endl;
			if (!_file.is_open())
				throw 404;
			return;	
		}
		if (_body_status == false)
		{
			if (this->_method == "GET")
				this->handle_get();
			else if (this->_method == "DELETE")
			{
				this->handle_delete(_dir, this->req_uri);	
				throw 204;
			}
		}
	}
	catch(int err)
	{
		//_file.close();
		_req.set_status_code(err);
		//std::cout << "catch" << std::endl;
		_headers_status = true;
		_body_status = true;
		//std::cout << "error in this req_uri = " << this->req_uri << std::endl;
		this->handle_err(err);
		//std::cout << this->_res << std::endl;
	}
}

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

		if (file.substr(file.length() - it->length(), file.length()) == *it)
		{
			//std::cout << "file = "<< file << " it = " << *it << std::endl;
			return (*it2);
		}
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

void	Response::handle_delete(DIR *dir, std::string req)
{
	if (dir)
	{
		struct dirent *reader;
		std::string path;
		while ((reader = readdir(dir)))
		{
			if (reader->d_name[0] == '.' && (reader->d_name[1] == '.' || !reader->d_name[1]))
				continue;
			DIR *newdir = opendir(path.c_str());
			handle_delete(newdir, path);
		}
		closedir(dir);
		if (std::remove(req.c_str()))
			throw 403;
		return ;		
	}
	else if (std::remove(req.c_str()))
	{
		//std::cout << "req_uri in del = " << req << std::endl;
		throw 403;
	}					
}

void	Response::handle_get()
{
	// if (_headers_status == false)
	// {
	// 	std::cout << "here" << std::endl;
	// 	_headers_status = true;
	// 	this->_head = set_head() + "\r\n";
	// 	this->_res = this->_head;
	// 	std::cout << this->_res << std::endl;
	// 	if (!_file.is_open())
	// 		throw 404;
	// }
	int j = 200000;
	std::ifstream::pos_type l = j;
	// _file.open(this->req_uri, std::ios::binary);
	if (_body_status == false)
	{
		if (this->_content_length < l)
			l = this->_content_length;	
		else if (this->_content_length - c < l)
		{
			l = this->_content_length - c;
			//std::cout << "changing l = " << _file.tellg();
		}
		// std::cout << "contrnrt len = " << this->_content_length << " " << c << std::eƒndl;
		// std::cout << "9bl " << l << std::endl;
		c += l;
		std::vector<char> vec((int)l);
		_file.read(&vec[0], (int)l);
		// _file.close();
		std::string res(vec.begin(), vec.end());
		//std::cout << "b3d" << std::endl;
		this->_res = res;
		if (l < j)
		{
			//std::cout << "fd = " << _req.get_fd() << std::endl;
			// std::cout << "fd = " << _fd << " l =";
			// std::cout << l << std::endl;
			// std::cout << this->req_uri << " finished con len = " << this->_content_length << " c = " << c << std::endl; 
			this->_body_status = true;
			_file.close();
			if (this->auto_index)
				std::remove(this->req_uri.c_str());
		}
	}	
}
