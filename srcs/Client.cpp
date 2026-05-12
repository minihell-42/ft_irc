#include "../includes/Client.hpp"
#include "../includes/Server.hpp"

Client::Client(int fd, const std::string &hostname)
	: _fd(fd), _hostname(hostname), _state(HANDSHAKE)
{
}

Client::~Client()
{
}

// GETTERS
int Client::get_fd() const { return _fd; }
std::string Client::get_nickname() const { return _nickname; }
std::string Client::get_username() const { return _username; }
std::string Client::get_realname() const { return _realname; }
std::string Client::get_hostname() const { return _hostname; }
ClientState Client::get_state() const { return _state; }
std::string Client::get_prefix() const { return _nickname + "!" + _username + "@" + _hostname; }

// SETTERS
void Client::set_nickname(const std::string &nickname) { _nickname = nickname; }
void Client::set_username(const std::string &username) { _username = username; }
void Client::set_realname(const std::string &realname) { _realname = realname; }
void Client::set_state(ClientState state) { _state = state; }

// BUFFER
void Client::append_buffer(const std::string &data) { _buffer += data; }
std::string& Client::get_buffer() { return _buffer; }

// COMMUNICATION
void Client::reply(const std::string &msg)
{
	if (send(_fd, msg.c_str(), msg.length(), 0) < 0)
		throw Server::SendError();
}
