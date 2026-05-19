#include "../includes/Client.hpp"
#include "../includes/Server.hpp"

Client::Client(int fd, const std::string &hostname)
	: _fd(fd), _hostname(hostname), _state(HANDSHAKE) {}

Client::~Client() {}

// GETTERS
std::string Client::getNickname() const { return _nickname; }

std::string Client::getUsername() const { return _username; }

std::string Client::getRealname() const { return _realname; }

std::string Client::getHostname() const { return _hostname; }

std::string Client::getPrefix() const 
{ 
	return _nickname + "!" + _username + "@" + _hostname;
}

std::string& Client::getBuffer() { return _buffer; }

ClientState Client::getState() const { return _state; }

int Client::getFd() const { return _fd; }

// SETTERS
void Client::setNickname(const std::string &nickname) { _nickname = nickname; }

void Client::setUsername(const std::string &username) { _username = username; }

void Client::setRealname(const std::string &realname) { _realname = realname; }

void Client::setState(ClientState state) { _state = state; }

// BUFFER
void Client::appendBuffer(const std::string &data) { _buffer += data; }

// COMMUNICATION
void Client::reply(const std::string &msg)
{
	_writeBuffer += msg;
}

bool Client::hasOutput() const
{
	return !_writeBuffer.empty();
}

void Client::sendPendingOutput()
{
	if (_writeBuffer.empty())
		return;

	int bytes_sent = send(_fd, _write_buffer.c_str(), _write_buffer.length(), 0);
	if (bytes_sent > 0)
		_write_buffer.erase(0, bytes_sent);
	else if (bytes_sent == -1)
		throw Server::SendError();
}
