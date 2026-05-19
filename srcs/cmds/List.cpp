#include "../../includes/cmds/List.hpp"
#include "../../includes/Client.hpp"
#include "../../includes/Server.hpp"
#include "../../includes/Channel.hpp"
#include "../../includes/response.hpp"

List::List(Server& server) : ACommand(server) {}
List::~List() {}

void List::execute(Client* client, std::vector<std::string>& args)
{
    (void)args;
    std::string nick = client->getNickname().empty() ? "*" : client->getNickname();

    client->reply(RPL_LISTSTART(_server.getName(), nick));

    const std::map<std::string, Channel*>& channels = _server.getChannels();
    for (std::map<std::string, Channel*>::const_iterator it = channels.begin(); it != channels.end(); ++it)
    {
        Channel* ch = it->second;
        if (!ch) continue;
        std::string count = std::to_string(ch->memberCount());
        std::string topic = ch->getTopic();
        client->reply(RPL_LIST(_server.getName(), nick, ch->getName(), count, topic));
    }

    client->reply(RPL_LISTEND(_server.getName(), nick));
}
