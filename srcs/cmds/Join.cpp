#include "../../includes/cmds/Join.hpp"
#include "../../includes/Client.hpp"
#include "../../includes/Server.hpp"
#include "../../includes/Channel.hpp"
#include "../../includes/response.hpp"

Join::Join(Server& server) : ACommand(server) {}

Join::~Join() {}

void Join::execute(Client* client, std::vector<std::string>& args)
{
    if (args.empty())
    {
        std::string client_id = client->get_nickname().empty() ? "*" : client->get_nickname();
        client->reply(ERR_NEEDMOREPARAMS(std::string("ft_irc"), client_id, "JOIN"));
        return;
    }

    std::string chan_name = args[0];
    std::string key = args.size() > 1 ? args[1] : std::string("");

    Channel* chan = _server.get_channel(chan_name);
    if (!chan)
    {
        chan = _server.create_channel(chan_name);
    }

    // Check invite-only
    if (chan->is_invite_only() && !chan->is_invited(client->get_nickname()))
    {
        std::string client_id = client->get_nickname().empty() ? "*" : client->get_nickname();
        client->reply(ERR_INVITEONLYCHAN(std::string("ft_irc"), client_id, chan_name));
        return;
    }

    // Check key
    if (!chan->get_key().empty() && chan->get_key() != key)
    {
        std::string client_id = client->get_nickname().empty() ? "*" : client->get_nickname();
        client->reply(ERR_BADCHANNELKEY(std::string("ft_irc"), client_id, chan_name));
        return;
    }

    // try to add member
    if (!chan->add_member(client))
    {
        std::string client_id = client->get_nickname().empty() ? "*" : client->get_nickname();
        client->reply(ERR_CHANNELISFULL(std::string("ft_irc"), client_id, chan_name));
        return;
    }

    // If first member, make operator
    if (chan->member_count() == 1)
        chan->add_operator(client);

    // Broadcast join to other members
    std::string join_msg = MSG_JOIN(client->get_prefix(), chan_name);
    chan->broadcast(join_msg, client);

    // Send NAMES reply to joining client
    // Build users list
    std::string users = chan->get_member_names();
    client->reply(RPL_NAMREPLY(std::string("ft_irc"), client->get_nickname(), "=", chan_name, users));
    client->reply(RPL_ENDOFNAMES(std::string("ft_irc"), client->get_nickname(), chan_name));
}
