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
        std::string nick = client->get_nickname().empty() ? "*" : client->get_nickname();
        client->reply(ERR_NONICKNAMEGIVEN(std::string("ft_irc"), nick));
        return;
    }
    
    std::string new_nick = args[0];
    
    // Check if the nick is already in use by another client
    if (_server.is_nickname_in_use(new_nick))
    {
        std::cout << "NICK: Nickname '" << new_nick << "' is already in use." << std::endl;
        std::string nick = client->get_nickname().empty() ? "*" : client->get_nickname();
        client->reply(ERR_NICKNAMEINUSE(std::string("ft_irc"), nick, new_nick));
        return;
    }
    
    client->set_nickname(new_nick);
    std::cout << "Nick for fd " << client->get_fd() << " set to: " << new_nick << std::endl;
}
