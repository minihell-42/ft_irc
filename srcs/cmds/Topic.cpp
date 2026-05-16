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
        std::string client_id = client->get_nickname().empty() ? "*" : client->get_nickname();
        client->reply(ERR_NEEDMOREPARAMS(std::string("ft_irc"), client_id, "TOPIC"));
        return;
    }

    std::string chan_name = args[0];
    Channel* chan = _server.get_channel(chan_name);
    if (!chan)
    {
        std::string client_id = client->get_nickname().empty() ? "*" : client->get_nickname();
        client->reply(ERR_NOSUCHCHANNEL(std::string("ft_irc"), client_id, chan_name));
        return;
    }

    if (!chan->has_member(client))
    {
        std::string client_id = client->get_nickname().empty() ? "*" : client->get_nickname();
        client->reply(ERR_NOTONCHANNEL(std::string("ft_irc"), client_id, chan_name));
        return;
    }

    // If only channel argument -> show topic
    if (args.size() == 1)
    {
        if (chan->get_topic().empty())
            client->reply(RPL_NOTOPIC(std::string("ft_irc"), client->get_nickname(), chan_name));
        else
            client->reply(RPL_TOPIC(std::string("ft_irc"), client->get_nickname(), chan_name, chan->get_topic()));
        return;
    }

    // Set topic (args[1] contains trailing topic due to parser)
    std::string new_topic = args[1];
    chan->set_topic(new_topic);

    // Notify channel members of topic change
    std::string topic_msg = ":" + client->get_prefix() + " TOPIC " + chan_name + " :" + new_topic + "\r\n";
    chan->broadcast(topic_msg, NULL);

    // Reply to setter with RPL_TOPIC
    client->reply(RPL_TOPIC(std::string("ft_irc"), client->get_nickname(), chan_name, new_topic));
}
