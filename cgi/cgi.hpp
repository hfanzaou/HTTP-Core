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
<<<<<<< HEAD
        Cgi(request& _req, const std::string& path);
        void    execute_cgi();
        char    **getPyEnv();
		char	**getPHPEnv();
=======
        Cgi(request& _req, std::string& path);
        void    execute_cgi(int &status_code);
        char    **getEnv();
>>>>>>> e9389aefcb0550cf371720d38d5751195fbc6148
        const std::string& getCgiResponse() const;
};