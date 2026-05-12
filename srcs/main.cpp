# include "../includes/Server.hpp"
#include <exception>
#include <stdexcept>

int main(int ac, char **av)
{
	if (ac != 3)
		throw std::invalid_argument("Usage: ./ircserv <port> <password>");
	try {
		Server server(av);
		server.run();
	} catch (std::exception& e) {
			std::cerr << "\033[0;31m" << e.what() << "\033[0m" << '\n';
			return 1;
	}
	return (0);
}