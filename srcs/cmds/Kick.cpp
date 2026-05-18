#include "../../includes/cmds/Kick.hpp"
#include "../../includes/Client.hpp"
#include "../../includes/Server.hpp"
#include "../../includes/Channel.hpp"
#include "../../includes/response.hpp"

Kick::Kick(Server& server) : ACommand(server) {}

Kick::~Kick() {}

void Kick::execute(Client* client, std::vector<std::string>& args)
{
    // KICK <channel> <user> [reason]
    if (args.size() < 2)
    {
        std::string clientId = client->getNickname().empty() ? "*" : client->getNickname();
        client->reply(ERR_NEEDMOREPARAMS(std::string("ft_irc"), clientId, "KICK"));
        return;
    }

    std::string chanName = args[0];
    std::string targetNick = args[1];
    std::string reason = args.size() > 2 ? args[2] : std::string("");

    Channel* chan = _server.getChannel(chanName);
    if (!chan)
    {
        std::string clientId = client->getNickname().empty() ? "*" : client->getNickname();
        client->reply(ERR_NOSUCHCHANNEL(std::string("ft_irc"), clientId, chanName));
        return;
    }

    // Only channel operators can kick
    if (!chan->isOperator(client))
    {
        std::string clientId = client->getNickname().empty() ? "*" : client->getNickname();
        client->reply(ERR_CHANOPRIVSNEEDED(std::string("ft_irc"), clientId, chanName));
        return;
    }

    Client* target = _server.getClientByNickname(targetNick);
    if (!target)
    {
        std::string clientId = client->getNickname().empty() ? "*" : client->getNickname();
        client->reply(ERR_NOSUCHNICK(std::string("ft_irc"), clientId, targetNick));
        return;
    }

    if (!chan->hasMember(target))
    {
        std::string clientId = client->getNickname().empty() ? "*" : client->getNickname();
        client->reply(ERR_USERNOTINCHANNEL(std::string("ft_irc"), clientId, targetNick, chanName));
        return;
    }

    // Build KICK message and send to all members
    std::string kickMsg = MSG_KICK(client->getPrefix(), chanName, targetNick, reason);

    // Send to kicked user and broadcast to others
    target->reply(kickMsg);
    chan->broadcast(kickMsg, target);

    // Remove the user from channel
    chan->removeMember(target);

    // If channel empty, remove channel from server
    if (chan->memberCount() == 0)
        _server.removeChannel(chanName);
}
