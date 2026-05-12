#include "../../includes/cmds/User.hpp"
#include "../../includes/response.hpp"

User::User(Server& server) : ACommand(server) {}

User::~User() {}

void User::execute(Client* client, std::vector<std::string>& args)
{
    std::string client_id = client->get_nickname().empty() ? "*" : client->get_nickname();

    if (!client->get_username().empty()) 
    {
        client->reply(ERR_ALREADYREGISTRED(std::string("ft_irc"), client_id));
        return;
    }
    // USER <username> <hostname> <servername> <realname>
    if (args.size() < 4)
    {
        client->reply(ERR_NEEDMOREPARAMS(std::string("ft_irc"), client_id, "USER"));
        return;
    }
    
    std::string new_username = args[0];
    std::string realname = args[3];
    
    client->set_username(new_username);
    client->set_realname(realname);
    
    std::cout << "Username for fd " << client->get_fd() << " set to: " << new_username << std::endl 
    << "Realname set to: " << realname << std::endl;

    _server.try_register(client);
}
