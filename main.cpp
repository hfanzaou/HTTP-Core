#include "server/server.hpp"

int main(int ac, char **av)
{
	if (ac != 2)
		return (1);
	try
	{
    	Config	config(av[1]);
		server	server(config);

		config.parse();
		config.printServers();
		for (size_t i = 0; i < config.getServers().size(); i++)
		{
			server.init_server(config.getServers()[i]);
			server.start_server();
		}
		server.multiplex_server();
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
	}
}
