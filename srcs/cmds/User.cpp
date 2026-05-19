#include "../../includes/cmds/User.hpp"
#include "../../includes/response.hpp"

User::User(Server& server) : ACommand(server) {}

User::~User() {}

void User::execute(Client* client, std::vector<std::string>& args)
{
    std::string clientId = client->getNickname().empty() ? "*" : client->getNickname();

    if (!client->getUsername().empty()) 
    {
        client->reply(ERR_ALREADYREGISTRED(std::string("ft_irc"), clientId));
        return;
    }
    // USER <username> <hostname> <servername> <realname>
    if (args.size() < 4)
    {
        client->reply(ERR_NEEDMOREPARAMS(std::string("ft_irc"), clientId, "USER"));
        return;
    }
    
    std::string newUsername = args[0];
    std::string realname = args[3];
    
    client->setUsername(newUsername);
    client->setRealname(realname);
    
    std::cout << "Username for fd " << client->getFd() << " set to: " << newUsername << std::endl 
    << "Realname set to: " << realname << std::endl;

    _server.tryRegister(client);
}
