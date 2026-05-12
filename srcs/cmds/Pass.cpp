#include "../../includes/cmds/Pass.hpp"
#include "../../includes/Client.hpp"
#include "../../includes/Server.hpp"
#include "../../includes/response.hpp"

Pass::Pass(Server& server) : ACommand(server) {}

Pass::~Pass() {}

void Pass::execute(Client* client, std::vector<std::string>& args)
{
    std::string nick = client->get_nickname().empty() ? "*" : client->get_nickname();

    if (client->get_state() == REGISTERED)
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
    if (args[0] != _server.get_password())
    {
        std::cout << "PASS: Password mismatch." << std::endl;
        client->reply(ERR_PASSWDMISMATCH(std::string("ft_irc"), nick));
        return;
    }
    
    std::cout << "PASS accepted for fd " << client->get_fd() << std::endl;
    client->set_state(LOGIN);
    
    _server.try_register(client);
}
