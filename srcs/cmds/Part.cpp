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
        std::string clientId = client->getNickname().empty() ? "*" : client->getNickname();
        client->reply(ERR_NEEDMOREPARAMS(std::string("ft_irc"), clientId, "PART"));
        return;
    }

    std::string chanName = args[0];
    std::string reason = args.size() > 1 ? args[1] : std::string("");

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

    std::string partMsg = MSG_PART(client->getPrefix(), chanName, reason);

    // Send the PART to the leaving client
    client->reply(partMsg);
    // Broadcast to others
    chan->broadcast(partMsg, client);

    chan->removeMember(client);

    // If channel is empty, remove it from server
    if (chan->memberCount() == 0)
        _server.removeChannel(chanName);
}
