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
        std::string client_id = client->get_nickname().empty() ? "*" : client->get_nickname();
        client->reply(ERR_NEEDMOREPARAMS(std::string("ft_irc"), client_id, "INVITE"));
        return;
    }

    std::string target_nick = args[0];
    std::string chan_name = args[1];

    Channel* chan = _server.get_channel(chan_name);
    if (!chan)
    {
        std::string client_id = client->get_nickname().empty() ? "*" : client->get_nickname();
        client->reply(ERR_NOSUCHCHANNEL(std::string("ft_irc"), client_id, chan_name));
        return;
    }

    // Only operators can invite (project requirement)
    if (!chan->is_operator(client))
    {
        std::string client_id = client->get_nickname().empty() ? "*" : client->get_nickname();
        client->reply(ERR_CHANOPRIVSNEEDED(std::string("ft_irc"), client_id, chan_name));
        return;
    }

    Client* target = _server.get_client_by_nickname(target_nick);
    if (!target)
    {
        std::string client_id = client->get_nickname().empty() ? "*" : client->get_nickname();
        client->reply(ERR_NOSUCHNICK(std::string("ft_irc"), client_id, target_nick));
        return;
    }

    if (chan->has_member(target))
    {
        std::string client_id = client->get_nickname().empty() ? "*" : client->get_nickname();
        client->reply(ERR_USERONCHANNEL(std::string("ft_irc"), client_id, target_nick, chan_name));
        return;
    }

    // Add to invite list and notify
    chan->invite(target_nick);
    client->reply(RPL_INVITING(std::string("ft_irc"), client->get_nickname(), target_nick, chan_name));

    // Send the invite message to the target
    std::string invite_msg = ":" + client->get_prefix() + " INVITE " + target_nick + " :" + chan_name + "\r\n";
    target->reply(invite_msg);
}
