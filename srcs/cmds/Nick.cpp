#include "../../includes/cmds/Nick.hpp"
#include "../../includes/Client.hpp"
#include "../../includes/Server.hpp"
#include "../../includes/response.hpp"

Nick::Nick(Server& server) : ACommand(server) {}

Nick::~Nick() {}

void Nick::execute(Client* client, std::vector<std::string>& args)
{
    if (args.empty())
    {
        std::cout << "NICK: No nick given." << std::endl;
        std::string nick = client->getNickname().empty() ? "*" : client->getNickname();
        client->reply(ERR_NONICKNAMEGIVEN(std::string("ft_irc"), nick));
        return;
    }
    
    std::string newNick = args[0];
    
    // Check if the nick is already in use by another client
    if (_server.isNicknameInUse(newNick))
    {
        std::cout << "NICK: Nickname '" << newNick << "' is already in use." << std::endl;
        std::string nick = client->getNickname().empty() ? "*" : client->getNickname();
        client->reply(ERR_NICKNAMEINUSE(std::string("ft_irc"), nick, newNick));
        return;
    }
    
    client->setNickname(newNick);
    std::cout << "Nick for fd " << client->getFd() << " set to: " << newNick << std::endl;
    
    _server.tryRegister(client);
}
