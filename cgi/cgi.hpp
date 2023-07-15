#include <string>
#include <sys/types.h>
#include <sys/wait.h>
#include "../request/request.hpp"
#include <string.h>
#include <fstream>

class Cgi
{
    private :
        std::string                         cgi_response;
        request                       &req;
		const std::string 					path;
        std::map<std::string, std::string>  _env;

    public :
        Cgi(request& _req, const std::string& path);
        void    execute_cgi();
        char    **getEnv();
        const std::string& getCgiResponse() const;
};