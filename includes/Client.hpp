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

class Client {
	private:
		std::string _nickname;
		std::string _username;
		std::string _realname;

		int 				_fd;
		std::string _hostname;

		std::string _buffer;
		std::string _writeBuffer;
		ClientState _state;

	public:
		Client(int fd, const std::string &hostname);
		~Client();

		// getters
		std::string getNickname() const;
		std::string getUsername() const;
		std::string getRealname() const;
		std::string getHostname() const;
		std::string getPrefix() const;
		std::string &getBuffer();
		ClientState getState() const;
		int 				getFd() const;

		// setters
		void setNickname(const std::string &nickname);
		void setUsername(const std::string &nickname);
		void setRealname(const std::string &nickname);
		void setState(ClientState state);

		//buffer methods
		void appendBuffer(const std::string &data);
		
		// communication
		bool hasOutput() const;
		void sendPendingOutput();
		void reply(const std::string &msg);
};

#endif