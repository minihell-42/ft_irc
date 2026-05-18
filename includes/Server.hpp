#ifndef SERVER_HPP
#define SERVER_HPP

#include <arpa/inet.h>
#include <ctime>
#include <fcntl.h>
#include <map>
#include <netdb.h>
#include <netinet/in.h>
#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include "Channel.hpp"
#include "Client.hpp"
#include "cmds/ACommand.hpp"
#include <iostream>

class Server {
  private:
    int                               _port;
    int                               _socketFd;
    static bool                       _running;
    std::string                       _createdAt;
    const std::string                 _pass;
    const std::string                 _name;
    std::vector<pollfd>               _pfds;
    std::map<int, Client*>            _clients;
    std::map<std::string, Channel*>   _channels;
    std::map<std::string, ACommand*>  _commands;

    Server(const Server& other);
    Server& operator=(const Server& other);

  public:
    Server(char **av);
    ~Server();

    void run();
    void initSocket();
    
    // client events
    void clientConnect();
    void clientDisconnect(int fd);
    void clientRequest(int fd);
    void tryRegister(Client *client);

    // getters and setters
    std::string getPassword() const;
    std::string getName() const;

    // Utils
    bool isNicknameInUse(const std::string& nick) const;
    Client* getClientByNickname(const std::string& nick) const;
    static void signalHandler(int sig);

    // Commands
    void parseCommand(Client *client, const std::string &line);
    void routeCommand(Client *client, const std::string &command, std::vector<std::string> &args);

    // Channels
    Channel* getChannel(const std::string &name) const;
    Channel* createChannel(const std::string &name);
    void removeChannel(const std::string &name);

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