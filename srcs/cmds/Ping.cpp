#include "../../includes/cmds/Ping.hpp"
#include "../../includes/Client.hpp"
#include "../../includes/Server.hpp"

Ping::Ping(Server& server) : ACommand(server) {}

Ping::~Ping() {}

void Ping::execute(Client* client, std::vector<std::string>& args)
{
    if (args.empty())
    {
        std::cout << "PING: Not enough parameters." << std::endl;
        return;
    }
    
    std::string token = args[0];
    
    std::string reply = "PONG " + _server.getName() + " :" + token + "\r\n";
    client->reply(reply);
    std::cout << "Queued PONG for fd " << client->getFd() << std::endl;
}