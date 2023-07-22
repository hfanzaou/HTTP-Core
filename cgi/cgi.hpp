#ifndef CGI_HPP
#define CGI_HPP

#include <string>
#include <sys/types.h>
#include <sys/wait.h>
#include "../request/request.hpp"
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <signal.h>

class Cgi
{
    private :
        std::string                         cgi_response;
        request                             &_req;
		const std::string 					_path;
        std::map<std::string, std::string>  _env;
        int                                 _status;

    public :
        Cgi(request& _req, const std::string& path);
        int                 execute_cgi(std::string filename);
        char                **getEnv();
        const std::string&  getCgiResponse() const;
};

#endif
