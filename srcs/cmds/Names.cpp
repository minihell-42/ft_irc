#include "../../includes/cmds/Names.hpp"
#include "../../includes/Client.hpp"
#include "../../includes/Server.hpp"
#include "../../includes/Channel.hpp"
#include "../../includes/response.hpp"

Names::Names(Server& server) : ACommand(server) {}
Names::~Names() {}

static std::vector<std::string> splitChannels(const std::string &s)
{
    std::vector<std::string> res;
    std::string cur;
    for (size_t i = 0; i < s.size(); ++i)
    {
        if (s[i] == ',') { if (!cur.empty()) res.push_back(cur); cur.clear(); }
        else cur.push_back(s[i]);
    }
    if (!cur.empty()) res.push_back(cur);
    return res;
}

void Names::execute(Client* client, std::vector<std::string>& args)
{
    std::string nick = client->getNickname().empty() ? "*" : client->getNickname();

    if (args.empty())
    {
        const std::map<std::string, Channel*>& channels = _server.getChannels();
        for (std::map<std::string, Channel*>::const_iterator it = channels.begin(); it != channels.end(); ++it)
        {
            Channel* ch = it->second;
            if (!ch) continue;
            std::string users = ch->getMemberNames();
            client->reply(RPL_NAMREPLY(_server.getName(), nick, "=", ch->getName(), users));
            client->reply(RPL_ENDOFNAMES(_server.getName(), nick, ch->getName()));
        }
        return;
    }

    // args[0] may contain comma-separated channels
    std::vector<std::string> chans = splitChannels(args[0]);
    for (size_t i = 0; i < chans.size(); ++i)
    {
        Channel* ch = _server.getChannel(chans[i]);
        if (!ch)
        {
            client->reply(RPL_ENDOFNAMES(_server.getName(), nick, chans[i]));
            continue;
        }
        std::string users = ch->getMemberNames();
        client->reply(RPL_NAMREPLY(_server.getName(), nick, "=", ch->getName(), users));
        client->reply(RPL_ENDOFNAMES(_server.getName(), nick, ch->getName()));
    }
}
