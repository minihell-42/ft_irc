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
        std::string client_id = client->get_nickname().empty() ? "*" : client->get_nickname();
        client->reply(ERR_NEEDMOREPARAMS(std::string("ft_irc"), client_id, "KICK"));
        return;
    }

    std::string chan_name = args[0];
    std::string target_nick = args[1];
    std::string reason = args.size() > 2 ? args[2] : std::string("");

    Channel* chan = _server.get_channel(chan_name);
    if (!chan)
    {
        std::string client_id = client->get_nickname().empty() ? "*" : client->get_nickname();
        client->reply(ERR_NOSUCHCHANNEL(std::string("ft_irc"), client_id, chan_name));
        return;
    }

    // Only channel operators can kick
    if (!chan->isOperator(client))
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

    if (!chan->hasMember(target))
    {
        std::string client_id = client->get_nickname().empty() ? "*" : client->get_nickname();
        client->reply(ERR_USERNOTINCHANNEL(std::string("ft_irc"), client_id, target_nick, chan_name));
        return;
    }

    // Build KICK message and send to all members
    std::string kick_msg = MSG_KICK(client->get_prefix(), chan_name, target_nick, reason);

    // Send to kicked user and broadcast to others
    target->reply(kick_msg);
    chan->broadcast(kick_msg, target);

    // Remove the user from channel
    chan->removeMember(target);

    // If channel empty, remove channel from server
    if (chan->memberCount() == 0)
        _server.remove_channel(chan_name);
}
