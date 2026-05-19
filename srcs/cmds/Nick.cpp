#include "../../includes/cmds/Nick.hpp"
#include "../../includes/Client.hpp"
#include "../../includes/Server.hpp"
#include "../../includes/Channel.hpp"
#include "../../includes/response.hpp"

#include <set>

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
    std::string oldNick = client->getNickname();
    std::string oldPrefix = client->getPrefix();

    if (newNick == oldNick)
        return;
    
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

    std::string nickMsg = MSG_NICK(oldPrefix, newNick);

    std::set<Client*> recipients;
    recipients.insert(client);
    const std::map<std::string, Channel*>& channels = _server.getChannels();
    for (std::map<std::string, Channel*>::const_iterator it = channels.begin(); it != channels.end(); ++it)
    {
        Channel* channel = it->second;
        if (!channel || !channel->hasMember(client))
            continue;

        const std::set<Client*>& members = channel->getMembers();
        for (std::set<Client*>::const_iterator memberIt = members.begin(); memberIt != members.end(); ++memberIt)
            recipients.insert(*memberIt);
    }

    for (std::set<Client*>::const_iterator it = recipients.begin(); it != recipients.end(); ++it)
    {
        if (*it)
            (*it)->reply(nickMsg);
    }
    
    _server.tryRegister(client);
}
