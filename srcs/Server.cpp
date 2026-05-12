#include "../includes/Server.hpp"
#include "../includes/cmds/Pass.hpp"
#include "../includes/cmds/Nick.hpp"
#include "../includes/cmds/User.hpp"
#include "../includes/response.hpp"
#include "../includes/cmds/User.hpp"

bool Server::_running = true;

// CONSTRUCTOR AND DESTRUCTOR
Server::Server(char **av) : _name("irc.miservidor.com"), _pass(av[2])
{
	if (_pass.empty())
		throw InvalidPassword();
	_socket_fd = -1;

	char *end;
	long port = std::strtol(av[1], &end, 10);
	if (*end != '\0' || end == av[1] || port < 1 || port > 65535)
    	throw IncorrectPort();
	_port = static_cast<int>(port);

	time_t now = time(0);
	std::string time_str = ctime(&now);
	_created_at = time_str.substr(0, time_str.size() - 1);

	_commands["PASS"] = new Pass(*this);
	_commands["NICK"] = new Nick(*this);
	_commands["USER"] = new User(*this);
}

Server::~Server()
{
    close(_socket_fd);
    for (std::map<int, Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it)
        delete it->second;
    for (std::map<std::string, Channel*>::iterator it = _channels.begin(); it != _channels.end(); ++it)
        delete it->second;
    for (std::map<std::string, ACommand*>::iterator it = _commands.begin(); it != _commands.end(); ++it)
        delete it->second;
}

//GETTERS AND SETTERS
std::string Server::get_password() const
{
	return _pass;
}

// SERVER RUN & INIT 
void Server::init_socket()
{
	struct sockaddr_in  serv_addr = {};

	_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (_socket_fd == -1)
    	throw CreateSocketError();

	// forcefully attacing socket to the port by making it reusable
    int optval = 1;
    if (setsockopt(_socket_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) == -1)
        throw SetSocketOptionError();

    // making the socket NON-BLOCKING
    if (fcntl(_socket_fd, F_SETFL, O_NONBLOCK) == -1)
        throw NonBlockingSocketError();

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(_port);
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	// bind socket to an IP address on selected port
    if (bind(_socket_fd, (sockaddr *) &serv_addr, sizeof(serv_addr)) == -1)
        throw BindSocketError();

    // listening for requests
    if (listen(_socket_fd, SOMAXCONN) == -1)
        throw ListenSocketError();
}

void Server::run()
{
	init_socket();

	// Register signal handlers for graceful shutdown
	struct sigaction sa;
	sa.sa_handler = signal_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);

	pollfd srv = {_socket_fd, POLLIN, 0};
	_pfds.push_back(srv);

	std::cout << "Server is listening on port: " << _port << std::endl;

	while (_running)
	{
		if (poll(_pfds.begin().base(), _pfds.size(), -1) < 0)
		{
			if (errno == EINTR)
				continue;
			throw PollFailedError();
		}

		for (std::vector<pollfd>::iterator it = _pfds.begin(); it != _pfds.end(); it++) {
			try{
				if (it->revents & POLLIN)
				{
					if (it->fd == _socket_fd)
					{
						client_connect();
						break;
					}
					else
						client_request(it->fd);
				}
				else if (it->revents & (POLLHUP | POLLERR))
				{
					client_disconnect(it->fd);
					break;
				}
			} catch (std::exception& e) {
				std::cerr << "Error: " << e.what() << std::endl;
			}
		}
	}
}

// UTILS
bool Server::is_nickname_in_use(const std::string& nick) const
{
	for (std::map<int, Client*>::const_iterator it = _clients.begin(); it != _clients.end(); ++it)
	{
		if (it->second->get_nickname() == nick)
			return true;
	}
	return false;
}

void Server::signal_handler(int sig)
{
	(void)sig;
	_running = false;
}

// CLIENT EVENTS
void Server::client_connect()
{
	sockaddr_in  addr = {};
	socklen_t    len = sizeof(addr);

	int client_fd = accept(_socket_fd, (sockaddr *)&addr, &len);
	if (client_fd == -1)
    	throw AcceptSocketError();
	// make it non blocking
	if (fcntl(client_fd, F_SETFL, O_NONBLOCK) == -1)
	{
    	close(client_fd);
    	throw NonBlockingSocketError();
	}
	// add the poll for monitoring
	pollfd pfd = {client_fd, POLLIN, 0};
	_pfds.push_back(pfd);

	std::string hostname = inet_ntoa(addr.sin_addr);
	_clients[client_fd] = new Client(client_fd, hostname);

	std::cout << hostname << " connected on fd " << client_fd << std::endl;

}

void Server::client_disconnect(int fd)
{
	// remove from poll 
	for (std::vector<pollfd>::iterator it = _pfds.begin(); it != _pfds.end(); ++it)
	{
    	if (it->fd == fd)
    	{
       	 	_pfds.erase(it);
        	break;
    	}
	}
	// delete client
	std::map<int, Client*>::iterator it = _clients.find(fd);
	if (it != _clients.end())
	{
    	std::cout << "Client disconnected on fd " << fd << std::endl;
    	delete it->second;
    	_clients.erase(it);
	}
	close(fd);
}

void Server::client_request(int fd)
{
	char buf[512];
	int bytes = recv(fd, buf, sizeof(buf) - 1, 0);
	if (bytes <= 0)
	{
    	client_disconnect(fd);
    	return;
	}
	buf[bytes] = '\0';
	Client* client = _clients[fd];
	client->append_buffer(buf);

	std::string &buffer = client->get_buffer();
	size_t pos;
	while ((pos = buffer.find("\r\n")) != std::string::npos) {
		std::string line = buffer.substr(0, pos);
		buffer.erase(0, pos + 2);
		if (!line.empty())
		{
			std::cout << "Received: " << line << std::endl;
        	parse_command(client, line);
		}
	}
}

// PARSING & ROUTING COMMAND
void Server::parse_command(Client *client, const std::string &line)
{
	std::string command;
	std::vector<std::string> args;

	std::istringstream stream(line);
	stream >> command;

	std::string arg;
	while (stream >> arg)
	{
		if (arg[0] == ':')
		{
			std::string trailing;
			std::getline(stream, trailing);
			arg = arg.substr(1) + trailing;
			args.push_back(arg);
			break;
		}
		args.push_back(arg);
	}

	for (size_t i = 0; i < command.size(); i++)
		command[i] = std::toupper(command[i]);

	route_command(client, command, args);
}

void Server::route_command(Client *client, const std::string &command, std::vector<std::string> &args)
{
	std::map<std::string, ACommand*>::iterator it = _commands.find(command);
	if (it != _commands.end())
		it->second->execute(client, args);
	else
		std::cout << "Unknown command: " << command << std::endl;
}

// CLIENT REGISTRATION
void Server::try_register(Client *client)
{
	if (client->get_state() == REGISTERED)
		return;

	if (client->get_state() != LOGIN)
		return;

	if (client->get_nickname().empty() || client->get_nickname() == "*")
		return;

	if (client->get_username().empty())
		return;

	client->set_state(REGISTERED);

	client->reply(RPL_WELCOME(_name, client->get_nickname()));
	client->reply(RPL_YOURHOST(_name, client->get_nickname(), std::string("1.0")));
	client->reply(RPL_CREATED(_name, client->get_nickname(), _created_at));

	std::cout << "Client on fd " << client->get_fd() << " is now fully registered as " << client->get_nickname() << "!" << std::endl;
}