#ifndef SERVER_HPP
#define SERVER_HPP

#include <algorithm>
#include <arpa/inet.h>
#include <ctime>
#include <errno.h>
#include <fcntl.h>
#include <iostream>
#include <map>
#include <netdb.h>
#include <netinet/in.h>
#include <poll.h>
#include <signal.h>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <utility>

#include "Channel.hpp"
#include "Client.hpp"
#include "cmds/ACommand.hpp"


class Server {

private:
  static bool _running;
  const std::string _name;
  const std::string _pass;
  int _port;
  int _socket_fd;

  std::string _created_at;

  std::vector<pollfd>     _pfds;
  std::map<int, Client*> _clients;
  std::map<std::string, Channel*> _channels;
  std::map<std::string, ACommand*> _commands;

  Server(const Server& other);
  Server& operator=(const Server& other);

public:
  Server(char **av);
  ~Server();

  void run();
  void init_socket();
  
  // client events
  void client_connect();
  void client_disconnect(int fd);
  void client_request(int fd);
  void try_register(Client *client);

  // getters and setters
  std::string get_password() const;
  std::string get_name() const;

  // Utils
  bool is_nickname_in_use(const std::string& nick) const;
  Client* get_client_by_nickname(const std::string& nick) const;
  static void signal_handler(int sig);

  // Commands
  void parse_command(Client *client, const std::string &line);
  void route_command(Client *client, const std::string &command, std::vector<std::string> &args);

  // Channels
  Channel* get_channel(const std::string &name) const;
  Channel* create_channel(const std::string &name);
  void remove_channel(const std::string &name);

  // EXCEPTIONS
  class IncorrectPort : public std::exception {
    const char *what() const throw() {
      return "Error: Incorrect Port";
    }
  };

  class InvalidPassword : public std::exception {
    const char *what() const throw() {
      return "Error: Invalid password.";
    }
  };

  class CreateSocketError : public std::exception {
    const char *what() const throw() {
      return "Error: Creating socket failed.";
    }
  };

  class SetSocketOptionError : public std::exception {
    const char *what() const throw() {
      return "Error: Setting socket options failed.";
    }
  };  
  
  class NonBlockingSocketError : public std::exception {
    const char *what() const throw() {
      return "Error while setting socket to NON-BLOCKING!";
    }
  };

  class BindSocketError : public std::exception {
    const char *what() const throw() {
      return "Error: Binding failed.";
    }
  };

  class ListenSocketError : public std::exception {
    const char *what() const throw() { 
      return "Error: Listening failed."; 
    }
  };

  class AcceptSocketError : public std::exception {
    const char *what() const throw() { 
      return "Error: Accepting failed."; 
    }
  };

  class PollFailedError: public std::exception {
		const char * what() const throw() {
			return "Error: The poll() function failed.";
		}
	};
	class ConnectionError: public std::exception {
		const char * what() const throw() {
			return "Error: Connection failed";
		}
	};

  class SendError: public std::exception {
		const char * what() const throw() {
			return "Error: sending message failed";
		}
	};
};

#endif