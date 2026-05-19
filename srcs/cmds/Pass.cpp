#include "../../includes/cmds/Pass.hpp"
#include "../../includes/Client.hpp"
#include "../../includes/Server.hpp"
#include "../../includes/response.hpp"

Pass::Pass(Server& server) : ACommand(server) {}

Pass::~Pass() {}

void Pass::execute(Client* client, std::vector<std::string>& args)
{
    std::string nick = client->getNickname().empty() ? "*" : client->getNickname();

    if (client->getState() == REGISTERED)
    {
        std::cout << "Already registered." << std::endl;
        client->reply(ERR_ALREADYREGISTRED(std::string("ft_irc"), nick));
        return;
    }
    if (args.empty())
    {
        std::cout << "PASS: Not enough parameters." << std::endl;
        client->reply(ERR_NEEDMOREPARAMS(std::string("ft_irc"), nick, std::string("PASS")));
        return;
    }
    if (args[0] != _server.getPassword())
    {
        std::cout << "PASS: Password mismatch." << std::endl;
        client->reply(ERR_PASSWDMISMATCH(std::string("ft_irc"), nick));
        return;
    }
    
    std::cout << "PASS accepted for fd " << client->getFd() << std::endl;
    client->setState(LOGIN);
    
    _server.tryRegister(client);
}
