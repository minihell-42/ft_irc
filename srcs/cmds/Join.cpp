#include "../../includes/cmds/Join.hpp"
#include "../../includes/Client.hpp"
#include "../../includes/Server.hpp"
#include "../../includes/Channel.hpp"
#include "../../includes/response.hpp"

Join::Join(Server& server) : ACommand(server) {}

Join::~Join() {}

// TODO: it shows in the channel that a user join multiple times to the channel
void Join::execute(Client* client, std::vector<std::string>& args)
{
    if (args.empty())
    {
        std::string clientId = client->getNickname().empty() ? "*" : client->getNickname();
        client->reply(ERR_NEEDMOREPARAMS(std::string("ft_irc"), clientId, "JOIN"));
        return;
    }

    std::string chanName = args[0];
    std::string key = args.size() > 1 ? args[1] : std::string("");

    Channel* chan = _server.getChannel(chanName);
    if (!chan)
    {
        chan = _server.createChannel(chanName);
    }

    // Check invite-only
    if (chan->isInviteOnly() && !chan->isInvited(client->getNickname()))
    {
        std::string clientId = client->getNickname().empty() ? "*" : client->getNickname();
        client->reply(ERR_INVITEONLYCHAN(std::string("ft_irc"), clientId, chanName));
        return;
    }

    // Check key
    if (!chan->getKey().empty() && chan->getKey() != key)
    {
        std::string clientId = client->getNickname().empty() ? "*" : client->getNickname();
        client->reply(ERR_BADCHANNELKEY(std::string("ft_irc"), clientId, chanName));
        return;
    }

    // try to add member
    if (!chan->addMember(client))
    {
        std::string clientId = client->getNickname().empty() ? "*" : client->getNickname();
        client->reply(ERR_CHANNELISFULL(std::string("ft_irc"), clientId, chanName));
        return;
    }

    // If first member, make operator
    if (chan->memberCount() == 1)
        chan->addOperator(client);

    // Clear any outstanding invite for this nick
    chan->removeInvite(client->getNickname());

    // Broadcast join to other members
    std::string joinMsg = MSG_JOIN(client->getPrefix(), chanName);
    chan->broadcast(joinMsg, client);

    if (chan->getTopic().empty())
        client->reply(RPL_NOTOPIC(std::string("ft_irc"), client->getNickname(), chanName));
    else
        client->reply(RPL_TOPIC(std::string("ft_irc"), client->getNickname(), chanName, chan->getTopic()));

    // Send NAMES reply to joining client
    // Build users list
    std::string users = chan->getMemberNames();
    client->reply(RPL_NAMREPLY(std::string("ft_irc"), client->getNickname(), "=", chanName, users));
    client->reply(RPL_ENDOFNAMES(std::string("ft_irc"), client->getNickname(), chanName));
}
