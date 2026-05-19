#include "../../includes/cmds/Invite.hpp"
#include "../../includes/Client.hpp"
#include "../../includes/Server.hpp"
#include "../../includes/Channel.hpp"
#include "../../includes/response.hpp"

Invite::Invite(Server& server) : ACommand(server) {}

Invite::~Invite() {}

void Invite::execute(Client* client, std::vector<std::string>& args)
{
    // INVITE <nick> <channel>
    if (args.size() < 2)
    {
        std::string clientId = client->getNickname().empty() ? "*" : client->getNickname();
        client->reply(ERR_NEEDMOREPARAMS(std::string("ft_irc"), clientId, "INVITE"));
        return;
    }

    std::string targetNick = args[0];
    std::string chanName = args[1];

    Channel* chan = _server.getChannel(chanName);
    if (!chan)
    {
        std::string clientId = client->getNickname().empty() ? "*" : client->getNickname();
        client->reply(ERR_NOSUCHCHANNEL(std::string("ft_irc"), clientId, chanName));
        return;
    }

    // Only operators can invite
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

    if (chan->hasMember(target))
    {
        std::string clientId = client->getNickname().empty() ? "*" : client->getNickname();
        client->reply(ERR_USERONCHANNEL(std::string("ft_irc"), clientId, targetNick, chanName));
        return;
    }

    // Add to invite list and notify
    chan->invite(targetNick);
    client->reply(RPL_INVITING(std::string("ft_irc"), client->getNickname(), targetNick, chanName));

    // Send the invite message to the target
    std::string invite_msg = ":" + client->getPrefix() + " INVITE " + targetNick + " :" + chanName + "\r\n";
    target->reply(invite_msg);
}
