#include "cgi.hpp"

std::string    getHeaderValue(std::map<std::string, std::string>& headers, const std::string& key)
{
    if (headers.find(key) != headers.end())
        return headers[key];
    return "";
}

Cgi::Cgi(request& req, const std::string& path) : _req(req), _path(path), _status(0)
{}

char    **Cgi::getEnv()
{
    std::map<std::string, std::string>	headers = _req.get_header();

    _env["SERVER_NAME"] = getHeaderValue(headers, "Host");
    _env["SERVER_SOFTWARE"] = "WEBSERV/1.1";
    _env["GATEWAY_INTERFACE"] = "CGI/1.1";
    _env["QUERY_STRING"] = _req.get_query();
    _env["HTTP_COOKIE"] = getHeaderValue(headers, "Cookie");
    _env["PATH_INFO"] = _path;
	_env["REQUEST_METHOD"] = _req.get_method();
	if (_req.get_method() == "POST")
	{
    	_env["CONTENT_LENGTH"] = headers["Content-Length"];
		_env["CONTENT_TYPE"] = headers["Content-Type"];
	}

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

int Cgi::execute_cgi(std::string filename)
{
    int pid;
    int fd[2];

    if (pipe(fd) == -1)
    {
        std::cerr << "pipe failed!" << std::endl;
        return 500;
    }
    if ((pid = fork()) == -1)
    {
        std::cerr << "Fork failed!" << std::endl;
        return 500;
    }
    else if (pid == 0)
    {
        char    **env = getEnv();
	    int fdin = open(filename.c_str(), O_RDONLY);
        close(fd[0]);
		dup2(fdin, 0);
        dup2(fd[1], 1);
        close(fd[1]);

        char * const * nll = NULL;
        execve(_path.c_str(), nll, env);
        std::cerr << strerror(errno) << std::endl;
        exit(EXIT_FAILURE);
    }
    else
    {
        close(fd[1]);
        time_t startTime = time(NULL);
        while (true)
        {
            pid_t result = waitpid(pid, &_status, WNOHANG);
            if (result == -1)
                return 500;
            else if (result == 0)
            {
                time_t  currentTime = time(NULL);
                if (currentTime - startTime > 2)
                {
                    kill(pid, SIGKILL);
                    return 502;
                }
            }
            else
            {
                if (WEXITSTATUS(_status) != EXIT_SUCCESS)
                    return 500;
                break;
            }
            usleep(10000);
        }
        char buff[1024];
        int rbytes = 1;
        while (rbytes != 0)
        {
            memset(buff, 0, 1024);
            rbytes = read(fd[0], buff, 1023);
            buff[rbytes] = 0;
            cgi_response += buff;
        }
        close(fd[0]);
    }
    // for (size_t i = 0; i < _env.size() + 1; i++)
    //     delete env[i];
    // delete env;
   // std::cout << "Cgi Response:\n" << cgi_response << std::endl;
    return 200;
}

const std::string& Cgi::getCgiResponse() const { return cgi_response; }