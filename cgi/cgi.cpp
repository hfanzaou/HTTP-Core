#include "cgi.hpp"

Cgi::Cgi(request& _req, const std::string& _path) : req(_req), path(_path)
{}

char    **Cgi::getPyEnv()
{
    std::map<std::string, std::string>	headers = req.get_header();

    _env["HOST"] = headers["Host"];
	_env["REQUEST_METHOD"] = req.get_method();
	// if (req.get_method() == "POST")
	// {
    	_env["CONTENT_LENGTH"] = headers["Content-Length"];
		_env["CONTENT_TYPE"] = headers["Content-Type"];
	// }
    _env["HTTP_COOKIE"] = headers["Cookie"];
    std::map<std::string, std::string>::iterator it;
	char **env = new char*[_env.size() + 1];
	int i = 0;
	for(it = _env.begin(); it != _env.end(); it++)
    {
		std::string keyValue = it->first + "=" + it->second;
		env[i] = new char[keyValue.size() + 1];
		strcpy(const_cast<char*>(env[i]), keyValue.c_str());
		i++;
	}
	env[i] = NULL;
	return env;

}

<<<<<<< HEAD
char 	**Cgi::getPHPEnv()
{
	std::map<std::string, std::string>	headers = req.get_header();

	_env["HTTP_HOST"] = headers["Host"];
	_env["REQUEST_METHOD"] = req.get_method();
	_env["CONTENT_LENGTH"] = headers["Content-Length"];
	_env["CONTENT_TYPE"] = headers["Content-Type"];

	std::map<std::string, std::string>::iterator it;
	char **env = new char*[5];
	int i = 0;
	for(it = _env.begin(); it != _env.end(); it++)
    {
		std::string keyValue = it->first + "=" + it->second;
		env[i] = new char[keyValue.size() + 1];
		strcpy(const_cast<char*>(env[i]), keyValue.c_str());
		i++;
	}
	env[i] = NULL;
	return env;
}

void    Cgi::execute_cgi()
{
    int pid;
    int fd[2];
	char    **env;
	size_t pos = path.find(".");
	if	(path.substr(pos + 1) == "py")
   		env = getPyEnv();
	else if (path.substr(pos + 1) == "php")
		env = getPHPEnv();
	else
		throw 500;	
	// const char *phpP = "/usr/bin/php";
=======
void    Cgi::execute_cgi(int &status_code)
{
    int pid;
    int fd[2];
    (void)status_code;
    char    **env = getEnv();
>>>>>>> e9389aefcb0550cf371720d38d5751195fbc6148
    pipe(fd);
	int fdin = open("test", O_RDONLY);
    if ((pid = fork()) == 0)
    {
        close(fd[0]);
		dup2(fdin, 0);
        dup2(fd[1], 1);
        close(fd[1]);
<<<<<<< HEAD
		// char* constpath = new char[path.length()];
		// for (size_t i = 0; i < path.length(); i++)
		// 	constpath[i] = path[i];
        execve(path.c_str(), NULL, env);
=======
        char * const *nll = NULL;
        execve(path.c_str(), nll, env);
>>>>>>> e9389aefcb0550cf371720d38d5751195fbc6148
        perror(NULL);
        std::cerr << "Error executing script" << std::endl;
    }
    else
    {
        close(fd[1]);
<<<<<<< HEAD
        if (waitpid(-1, NULL, WNOHANG))
		{
			cgi_response = "infinite";
			close(fd[0]);
			return;
		}
		char buff[1024];
=======
        if (waitpid(-1, NULL, 0))

        char buff[1024];
>>>>>>> e9389aefcb0550cf371720d38d5751195fbc6148
        int r = 1;
        while (r != 0)
        {
            memset(buff, 0, 1024);
            r = read(fd[0], buff, 1023);
            buff[r] = 0;
            cgi_response += buff;
        }
        close(fd[0]);
    }
    std::cout << "waaaaaaa" << cgi_response << std::endl;
}

const std::string& Cgi::getCgiResponse() const
{
    return cgi_response;
}