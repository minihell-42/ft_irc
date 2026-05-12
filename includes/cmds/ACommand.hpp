#ifndef ACOMMAND_HPP
#define ACOMMAND_HPP

#include <string>
#include <vector>

class Client;
class Server;

class ACommand {
protected:
  Server &_server;

public:
  ACommand(Server &server);
  virtual ~ACommand();
  virtual void execute(Client *client, std::vector<std::string> &args) = 0;
};
#endif