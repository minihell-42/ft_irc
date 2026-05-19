#include "../includes/Server.hpp"
#include "../includes/cmds/Pass.hpp"
#include "../includes/cmds/Nick.hpp"
#include "../includes/cmds/User.hpp"
#include "../includes/cmds/Quit.hpp"
#include "../includes/cmds/Ping.hpp"
#include "../includes/cmds/Privmsg.hpp"
#include "../includes/cmds/Notice.hpp"
#include "../includes/cmds/Join.hpp"
#include "../includes/cmds/Part.hpp"
#include "../includes/cmds/Topic.hpp"
#include "../includes/cmds/Mode.hpp"
#include "../includes/cmds/Kick.hpp"
#include "../includes/cmds/Invite.hpp"
#include "../includes/response.hpp"
#include "../includes/cmds/List.hpp"
#include "../includes/cmds/Names.hpp"

bool Server::_running = true;

// CONSTRUCTOR AND DESTRUCTOR
Server::Server(char **av) : _name("irc.miservidor.com"), _pass(av[2])
{
	if (_pass.empty())
		throw InvalidPassword();
	_socketFd = -1;

	char *end;
	long port = std::strtol(av[1], &end, 10);
	if (*end != '\0' || end == av[1] || port < 1 || port > 65535)
    	throw IncorrectPort();
	_port = static_cast<int>(port);

	time_t now = time(0);
	std::string timeStr = ctime(&now);
	_createdAt = timeStr.substr(0, timeStr.size() - 1);

	_commands["PASS"] = new Pass(*this);
	_commands["NICK"] = new Nick(*this);
	_commands["USER"] = new User(*this);
	_commands["JOIN"] = new Join(*this);
	_commands["PART"] = new Part(*this);
	_commands["TOPIC"] = new Topic(*this);
	_commands["MODE"] = new Mode(*this);
	_commands["KICK"] = new Kick(*this);
	_commands["INVITE"] = new Invite(*this);
	_commands["LIST"] = new List(*this);
	_commands["NAMES"] = new Names(*this);
	_commands["QUIT"] = new Quit(*this);
	_commands["PING"] = new Ping(*this);
	_commands["PRIVMSG"] = new Privmsg(*this);
	_commands["NOTICE"] = new Notice(*this);
}

Server::~Server()
{
    close(_socketFd);
    for (std::map<int, Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it)
        delete it->second;
    for (std::map<std::string, Channel*>::iterator it = _channels.begin(); it != _channels.end(); ++it)
        delete it->second;
    for (std::map<std::string, ACommand*>::iterator it = _commands.begin(); it != _commands.end(); ++it)
        delete it->second;
}

//GETTERS AND SETTERS
std::string Server::getPassword() const
{
	return _pass;
}

std::string Server::getName() const
{
	return _name;
}

// SERVER RUN & INIT 
void Server::initSocket()
{
	struct sockaddr_in  servAddr = {};

	_socketFd = socket(AF_INET, SOCK_STREAM, 0);
	if (_socketFd == -1)
    	throw CreateSocketError();

	// forcefully attacing socket to the port by making it reusable
  int optval = 1;
  if (setsockopt(_socketFd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) == -1)
    throw SetSocketOptionError();

  // making the socket NON-BLOCKING
  if (fcntl(_socketFd, F_SETFL, O_NONBLOCK) == -1)
    throw NonBlockingSocketError();

	servAddr.sin_family = AF_INET;
	servAddr.sin_port = htons(_port);
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	// bind socket to an IP address on selected port
  if (bind(_socketFd, (sockaddr *) &servAddr, sizeof(servAddr)) == -1)
    throw BindSocketError();

  // listening for requests
  if (listen(_socketFd, SOMAXCONN) == -1)
    throw ListenSocketError();
}

void Server::run()
{
	initSocket();

	// Register signal handlers for graceful shutdown
	struct sigaction sa;
	sa.sa_handler = signalHandler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);

	pollfd srv = {_socketFd, POLLIN, 0};
	_pfds.push_back(srv);

	std::cout << "Server is listening on port: " << _port << std::endl;

	while (_running)
	{
		// Check which clients have data to send and enable POLLOUT
		for (size_t i = 1; i < _pfds.size(); i++) {
			if (_pfds[i].fd != -1) {
				Client* client = _clients[_pfds[i].fd];
				if (client && client->hasOutput())
					_pfds[i].events |= POLLOUT;
				else
					_pfds[i].events &= ~POLLOUT;
			}
		}

		if (poll(&_pfds[0], _pfds.size(), -1) < 0)
		{
			if (!_running)
				break;
			throw PollFailedError();
		}

		for (size_t i = 0; i < _pfds.size(); i++) {
			try{
				if (_pfds[i].revents & POLLIN)
				{
					if (_pfds[i].fd == _socketFd)
						clientConnect();
					else
						clientRequest(_pfds[i].fd);
				}
				
				if (_pfds[i].fd != -1 && (_pfds[i].revents & POLLOUT))
				{
					Client* client = _clients[_pfds[i].fd];
					if (client)
						client->sendPendingOutput();
				}

				if (_pfds[i].revents & (POLLHUP | POLLERR))
					clientDisconnect(_pfds[i].fd);
			} catch (std::exception& e) {
				std::cerr << "Error: " << e.what() << std::endl;
				if (_pfds[i].fd != -1 && _pfds[i].fd != _socket_fd)
					client_disconnect(_pfds[i].fd);
			}
		}

		// Clean up disconnected clients from the poll vector safely
		for (std::vector<pollfd>::iterator it = _pfds.begin(); it != _pfds.end(); ) {
			if (it->fd == -1)
				it = _pfds.erase(it);
			else
				++it;
		}
	}
}

// UTILS
bool Server::isNicknameInUse(const std::string& nick) const
{
	for (std::map<int, Client*>::const_iterator it = _clients.begin(); it != _clients.end(); ++it)
	{
		if (it->second->getNickname() == nick)
			return true;
	}
	return false;
}

Client* Server::getClientByNickname(const std::string& nick) const
{
	for (std::map<int, Client*>::const_iterator it = _clients.begin(); it != _clients.end(); ++it)
	{
		if (it->second->getNickname() == nick)
			return it->second;
	}
	return NULL;
}

void Server::signalHandler(int sig)
{
	(void)sig;
	_running = false;
}

// CHANNEL MANAGEMENT
Channel* Server::getChannel(const std::string &name) const
{
	std::map<std::string, Channel*>::const_iterator it = _channels.find(name);
	if (it != _channels.end())
		return it->second;
	return NULL;
}

Channel* Server::createChannel(const std::string &name)
{
	Channel* ch = new Channel(name);
	_channels[name] = ch;
	return ch;
}

void Server::removeChannel(const std::string &name)
{
	std::map<std::string, Channel*>::iterator it = _channels.find(name);
	if (it != _channels.end())
	{
		delete it->second;
		_channels.erase(it);
	}
}

const std::map<std::string, Channel*>& Server::getChannels() const
{
	return _channels;
}

// CLIENT EVENTS
void Server::clientConnect()
{
	sockaddr_in  addr = {};
	socklen_t    len = sizeof(addr);

	int clientFd = accept(_socketFd, (sockaddr *)&addr, &len);
	if (clientFd == -1)
    	throw AcceptSocketError();
	// make it non blocking
	if (fcntl(clientFd, F_SETFL, O_NONBLOCK) == -1)
	{
    	close(clientFd);
    	throw NonBlockingSocketError();
	}
	// add the poll for monitoring
	pollfd pfd = {clientFd, POLLIN, 0};
	_pfds.push_back(pfd);

	std::string hostname = inet_ntoa(addr.sin_addr);
	_clients[clientFd] = new Client(clientFd, hostname);

	std::cout << hostname << " connected on fd " << clientFd << std::endl;

}

void Server::clientDisconnect(int fd)
{
	for (std::vector<pollfd>::iterator it = _pfds.begin(); it != _pfds.end(); ++it)
	{
    	if (it->fd == fd)
    	{
       	 	it->fd = -1;
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

void Server::clientRequest(int fd)
{
	char buf[512];
	int bytes = recv(fd, buf, sizeof(buf) - 1, 0);
	if (bytes <= 0)
	{
    	clientDisconnect(fd);
    	return;
	}
	buf[bytes] = '\0';
	Client* client = _clients[fd];
	client->appendBuffer(buf);

	std::string &buffer = client->getBuffer();

	if (buffer.size() > 2048)
	{
		std::cout << "Client on fd " << fd << " exceeded buffer limit. Disconnecting..." << std::endl;
		clientDisconnect(fd);
		return;
	}

	size_t pos;
	while ((pos = buffer.find('\n')) != std::string::npos) {
		std::string line = buffer.substr(0, pos);
		buffer.erase(0, pos + 1);
		
		if (!line.empty() && line[line.length() - 1] == '\r')
			line.erase(line.length() - 1);
			
		if (!line.empty())
		{
			std::cout << "Received: " << line << std::endl;
        	parseCommand(client, line);
			
			if (client->getState() == DISCONNECTED)
			{
				clientDisconnect(fd);
				return;
			}
		}
	}
}

// PARSING & ROUTING COMMAND
void Server::parseCommand(Client *client, const std::string &line)
{
	std::string command;
	std::vector<std::string> args;

	std::istringstream stream(line);
	stream >> command;

	if (command.empty())
		return;

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

	routeCommand(client, command, args);
}

void Server::routeCommand(Client *client, const std::string &command, std::vector<std::string> &args)
{
	if (client->getState() != REGISTERED)
	{
		if (command != "PASS" && command != "NICK" && command != "USER" && command != "QUIT" && command != "CAP")
		{
			std::string nick = client->getNickname().empty() ? "*" : client->getNickname();
			client->reply(ERR_NOTREGISTERED(std::string("ft_irc"), nick));
			return;
		}
	}

	std::map<std::string, ACommand*>::iterator it = _commands.find(command);
	if (it != _commands.end())
		it->second->execute(client, args);
	else
		std::cout << "Unknown command: " << command << std::endl;
}

// CLIENT REGISTRATION
void Server::tryRegister(Client *client)
{
	if (client->getState() == REGISTERED)
		return;

	if (client->getState() != LOGIN)
		return;

	if (client->getNickname().empty() || client->getNickname() == "*")
		return;

	if (client->getUsername().empty())
		return;

	client->setState(REGISTERED);

	client->reply(RPL_WELCOME(_name, client->getNickname()));
	client->reply(RPL_YOURHOST(_name, client->getNickname(), std::string("1.0")));
	client->reply(RPL_CREATED(_name, client->getNickname(), _createdAt));

	std::cout << "Client on fd " << client->getFd() << " is now fully registered as " << client->getNickname() << "!" << std::endl;
}