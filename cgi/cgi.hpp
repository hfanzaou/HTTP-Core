#include <string>
#include <sys/types.h>
#include <sys/wait.h>
#include "../request/request.hpp"
#include <string.h>
#include <fstream>
#include <fcntl.h>
#include <unistd.h>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>

class Cgi
{
    private :
        std::string                         cgi_response;
        request                             &req;
		const std::string 					path;
        std::map<std::string, std::string>  _env;

    public :
        Cgi(request& _req, std::string& path);
        void    execute_cgi(int &status_code);
        char    **getEnv();
        const std::string& getCgiResponse() const;
};