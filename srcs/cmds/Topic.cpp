#include "../../includes/cmds/Topic.hpp"
#include "../../includes/Client.hpp"
#include "../../includes/Server.hpp"
#include "../../includes/Channel.hpp"
#include "../../includes/response.hpp"

Topic::Topic(Server& server) : ACommand(server) {}

Topic::~Topic() {}

void Topic::execute(Client* client, std::vector<std::string>& args)
{
    if (args.empty())
    {
        std::string clientId = client->getNickname().empty() ? "*" : client->getNickname();
        client->reply(ERR_NEEDMOREPARAMS(std::string("ft_irc"), clientId, "TOPIC"));
        return;
    }

    std::string chanName = args[0];
    Channel* chan = _server.getChannel(chanName);
    if (!chan)
    {
        std::string clientId = client->getNickname().empty() ? "*" : client->getNickname();
        client->reply(ERR_NOSUCHCHANNEL(std::string("ft_irc"), clientId, chanName));
        return;
    }

    if (!chan->hasMember(client))
    {
        std::string clientId = client->getNickname().empty() ? "*" : client->getNickname();
        client->reply(ERR_NOTONCHANNEL(std::string("ft_irc"), clientId, chanName));
        return;
    }

    // If only channel argument -> show topic
    if (args.size() == 1)
    {
        if (chan->getTopic().empty())
            client->reply(RPL_NOTOPIC(std::string("ft_irc"), client->getNickname(), chanName));
        else
            client->reply(RPL_TOPIC(std::string("ft_irc"), client->getNickname(), chanName, chan->getTopic()));
        return;
    }

    // Set topic (args[1] contains trailing topic due to parser)
    std::string newTopic = args[1];

    // If channel has +t (topic restricted), only operators can set it
    if (chan->isTopicRestricted() && !chan->isOperator(client))
    {
        std::string clientId = client->getNickname().empty() ? "*" : client->getNickname();
        client->reply(ERR_CHANOPRIVSNEEDED(std::string("ft_irc"), clientId, chanName));
        return;
    }

    chan->setTopic(newTopic);

    // Notify channel members of topic change
    std::string topicMsg = ":" + client->getPrefix() + " TOPIC " + chanName + " :" + newTopic + "\r\n";
    chan->broadcast(topicMsg, NULL);

    // Reply to setter with RPL_TOPIC
    client->reply(RPL_TOPIC(std::string("ft_irc"), client->getNickname(), chanName, newTopic));
}
