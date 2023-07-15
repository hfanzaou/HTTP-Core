#include "Response.hpp"

std::string get_type(std::string file)
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
			return (*it2);
		++it2;	
	}
	return ("application/octet-stream");
}

std::string ft_time()
{
	std::time_t Time = std::time(NULL);
	std::istringstream date(std::ctime(&Time));
	std::string day;
	std::string daym;
	std::string month;
	std::string year;
	std::string time;
	date >> day >> month >> daym >> time >> year;
	return (day + ", " + daym + " " + month + " " + year + " " + time + " GMT");
}

std::string generate_error(short error)
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
		case BAD_REQUEST:
			return ("400 Bad Request");
		case LONG_URI:
			return ("414 URI Too Long");
		case NOT_IMPLEM:
			return ("501 Not Implemented");
		case FOUND:
			return ("302 Found");	
	}
	return ("200 OK");
}