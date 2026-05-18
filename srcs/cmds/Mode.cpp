#include "../../includes/cmds/Mode.hpp"
#include "../../includes/Client.hpp"
#include "../../includes/Server.hpp"
#include "../../includes/Channel.hpp"
#include "../../includes/response.hpp"

Mode::Mode(Server& server) : ACommand(server) {}

Mode::~Mode() {}

void Mode::execute(Client* client, std::vector<std::string>& args)
{
    if (args.size() < 2)
    {
        std::string clientId = client->getNickname().empty() ? "*" : client->getNickname();
        client->reply(ERR_NEEDMOREPARAMS(std::string("ft_irc"), clientId, "MODE"));
        return;
    }

    std::string target = args[0];
    std::string modes = args[1];

    // We only handle channel modes (targets starting with # or &)
    if (target.empty() || (target[0] != '#' && target[0] != '&'))
    {
        std::string clientId = client->getNickname().empty() ? "*" : client->getNickname();
        client->reply(ERR_UNKNOWNMODE(std::string("ft_irc"), clientId, modes, target));
        return;
    }

    Channel* chan = _server.getChannel(target);
    if (!chan)
    {
        std::string clientId = client->getNickname().empty() ? "*" : client->getNickname();
        client->reply(ERR_NOSUCHCHANNEL(std::string("ft_irc"), clientId, target));
        return;
    }

    // Only channel operators can change modes
    if (!chan->isOperator(client))
    {
        std::string clientId = client->getNickname().empty() ? "*" : client->getNickname();
        client->reply(ERR_CHANOPRIVSNEEDED(std::string("ft_irc"), clientId, target));
        return;
    }

    // Process mode string (e.g., +itk, -o)
    bool add = true;
    size_t paramIndex = 2; // args after channel and modes
    for (size_t i = 0; i < modes.size(); ++i)
    {
        char c = modes[i];
        if (c == '+') { add = true; continue; }
        if (c == '-') { add = false; continue; }

        switch (c)
        {
            case 'i':
                chan->setInviteOnly(add);
                break;
            case 't':
                chan->setTopicRestricted(add);
                break;
            case 'k':
                if (add)
                {
                    if (paramIndex >= args.size())
                    {
                        std::string clientId = client->getNickname().empty() ? "*" : client->getNickname();
                        client->reply(ERR_NEEDMOREPARAMS(std::string("ft_irc"), clientId, "MODE"));
                        return;
                    }
                    chan->setKey(args[paramIndex++]);
                }
                else
                {
                    chan->setKey("");
                }
                break;
            case 'l':
                if (add)
                {
                    if (paramIndex >= args.size())
                    {
                        std::string clientId = client->getNickname().empty() ? "*" : client->getNickname();
                        client->reply(ERR_NEEDMOREPARAMS(std::string("ft_irc"), clientId, "MODE"));
                        return;
                    }
                    int limit = std::atoi(args[paramIndex++].c_str());
                    chan->setLimit(limit);
                }
                else
                {
                    chan->setLimit(0);
                }
                break;
            case 'o':
                // operator mode requires a nickname parameter
                if (paramIndex >= args.size())
                {
                    std::string clientId = client->getNickname().empty() ? "*" : client->getNickname();
                    client->reply(ERR_NEEDMOREPARAMS(std::string("ft_irc"), clientId, "MODE"));
                    return;
                }
                {
                    std::string nick = args[paramIndex++];
                    Client* targetClient = _server.getClientByNickname(nick);
                    if (!targetClient)
                    {
                        std::string clientId = client->getNickname().empty() ? "*" : client->getNickname();
                        client->reply(ERR_NOSUCHNICK(std::string("ft_irc"), clientId, nick));
                        return;
                    }
                    if (add)
                        chan->addOperator(targetClient);
                    else
                        chan->removeOperator(targetClient);
                }
                break;
            default:
                {
                    std::string clientId = client->getNickname().empty() ? "*" : client->getNickname();
                    client->reply(ERR_UNKNOWNMODE(std::string("ft_irc"), clientId, std::string(1, c), target));
                    return;
                }
        }
    }

    // Announce mode change to channel members
    // Reconstruct mode change string for reply
    std::string modeMsg = ":" + client->getPrefix() + " MODE " + target + " " + modes;
    // Append parameters if any (simple approach: append remaining args from index 2)
    for (size_t i = 2; i < paramIndex && i < args.size(); ++i)
        modeMsg += std::string(" ") + args[i];
    modeMsg += "\r\n";
    chan->broadcast(modeMsg, NULL);
}
