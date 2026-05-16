#include "../../includes/cmds/Part.hpp"
#include "../../includes/Client.hpp"
#include "../../includes/Server.hpp"
#include "../../includes/Channel.hpp"
#include "../../includes/response.hpp"

Part::Part(Server& server) : ACommand(server) {}

Part::~Part() {}

void Part::execute(Client* client, std::vector<std::string>& args)
{
    if (args.empty())
    {
        std::string client_id = client->get_nickname().empty() ? "*" : client->get_nickname();
        client->reply(ERR_NEEDMOREPARAMS(std::string("ft_irc"), client_id, "PART"));
        return;
    }

    std::string chan_name = args[0];
    std::string reason = args.size() > 1 ? args[1] : std::string("");

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

    std::string part_msg = MSG_PART(client->get_prefix(), chan_name, reason);

    // Send the PART to the leaving client
    client->reply(part_msg);
    // Broadcast to others
    chan->broadcast(part_msg, client);

    chan->remove_member(client);

    // If channel is empty, remove it from server
    if (chan->member_count() == 0)
        _server.remove_channel(chan_name);
}
