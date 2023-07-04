#include "server/server.hpp"

// int	main(void)
// {
// 	server	server;

// 	server.init_server();
// 	server.start_server();
// 	server.multiplex_server();
// }

int	main(int ac, char **av)
{
	if (ac < 2)
	{
		return (0);
	}
	server	server;
	try {
		Config	config(av[1]);
		config.parse();
		config.printServers();
		server.init_server();
		server.start_server();
		server.multiplex_server(config.getServers().at(0));
	}
	catch (std::exception &e)
	{
		std::cerr << e.what() << std::endl;
	}
}
