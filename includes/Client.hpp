#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>
#include <sys/socket.h>

enum ClientState
{
    HANDSHAKE,
    LOGIN,
    REGISTERED,
    DISCONNECTED
};

class Client
{
	private:
		std::string _nickname;
		std::string _username;
		std::string _realname;

		int _fd;
		std::string _hostname;

		std::string _buffer;
		ClientState     _state;

	public:
		Client(int fd, const std::string &hostname);
		~Client();

	// getters
	int 		get_fd() const;
	std::string get_nickname() const;
	std::string get_username() const;
	std::string get_realname() const;
	std::string get_hostname() const;
	ClientState get_state() const;
	std::string get_prefix() const;
	// setters
	void set_nickname(const std::string &nickname);
	void set_username(const std::string &nickname);
	void set_realname(const std::string &nickname);
	void set_state(ClientState state);
	//buffer methods
	void append_buffer(const std::string &data);
	std::string &get_buffer();
	
	// communication
	void reply(const std::string &msg);
};

#endif