#include "Config.hpp"
#include "ServerConfig.hpp"
#include "Location.hpp"

int main(int ac, char **av)
{
	if (ac != 2)
		return (1);
	try
	{
    	Config	config(av[1]);

		config.parse();

		config.printServers();
		
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
	}
    return 0;
}