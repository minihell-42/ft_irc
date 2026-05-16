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
        std::string client_id = client->get_nickname().empty() ? "*" : client->get_nickname();
        client->reply(ERR_NEEDMOREPARAMS(std::string("ft_irc"), client_id, "MODE"));
        return;
    }

    std::string target = args[0];
    std::string modes = args[1];

    // We only handle channel modes (targets starting with # or &)
    if (target.empty() || (target[0] != '#' && target[0] != '&'))
    {
        std::string client_id = client->get_nickname().empty() ? "*" : client->get_nickname();
        client->reply(ERR_UNKNOWNMODE(std::string("ft_irc"), client_id, modes, target));
        return;
    }

    Channel* chan = _server.get_channel(target);
    if (!chan)
    {
        std::string client_id = client->get_nickname().empty() ? "*" : client->get_nickname();
        client->reply(ERR_NOSUCHCHANNEL(std::string("ft_irc"), client_id, target));
        return;
    }

    // Only channel operators can change modes
    if (!chan->is_operator(client))
    {
        std::string client_id = client->get_nickname().empty() ? "*" : client->get_nickname();
        client->reply(ERR_CHANOPRIVSNEEDED(std::string("ft_irc"), client_id, target));
        return;
    }

    // Process mode string (e.g., +itk, -o)
    bool add = true;
    size_t param_index = 2; // args after channel and modes
    for (size_t i = 0; i < modes.size(); ++i)
    {
        char c = modes[i];
        if (c == '+') { add = true; continue; }
        if (c == '-') { add = false; continue; }

        switch (c)
        {
            case 'i':
                chan->set_invite_only(add);
                break;
            case 't':
                chan->set_topic_restricted(add);
                break;
            case 'k':
                if (add)
                {
                    if (param_index >= args.size())
                    {
                        std::string client_id = client->get_nickname().empty() ? "*" : client->get_nickname();
                        client->reply(ERR_NEEDMOREPARAMS(std::string("ft_irc"), client_id, "MODE"));
                        return;
                    }
                    chan->set_key(args[param_index++]);
                }
                else
                {
                    chan->set_key("");
                }
                break;
            case 'l':
                if (add)
                {
                    if (param_index >= args.size())
                    {
                        std::string client_id = client->get_nickname().empty() ? "*" : client->get_nickname();
                        client->reply(ERR_NEEDMOREPARAMS(std::string("ft_irc"), client_id, "MODE"));
                        return;
                    }
                    int limit = std::atoi(args[param_index++].c_str());
                    chan->set_limit(limit);
                }
                else
                {
                    chan->set_limit(0);
                }
                break;
            case 'o':
                // operator mode requires a nickname parameter
                if (param_index >= args.size())
                {
                    std::string client_id = client->get_nickname().empty() ? "*" : client->get_nickname();
                    client->reply(ERR_NEEDMOREPARAMS(std::string("ft_irc"), client_id, "MODE"));
                    return;
                }
                {
                    std::string nick = args[param_index++];
                    Client* target_client = _server.get_client_by_nickname(nick);
                    if (!target_client)
                    {
                        std::string client_id = client->get_nickname().empty() ? "*" : client->get_nickname();
                        client->reply(ERR_NOSUCHNICK(std::string("ft_irc"), client_id, nick));
                        return;
                    }
                    if (add)
                        chan->add_operator(target_client);
                    else
                        chan->remove_operator(target_client);
                }
                break;
            default:
                {
                    std::string client_id = client->get_nickname().empty() ? "*" : client->get_nickname();
                    client->reply(ERR_UNKNOWNMODE(std::string("ft_irc"), client_id, std::string(1, c), target));
                    return;
                }
        }
    }

    // Announce mode change to channel members
    // Reconstruct mode change string for reply
    std::string mode_msg = ":" + client->get_prefix() + " MODE " + target + " " + modes;
    // Append parameters if any (simple approach: append remaining args from index 2)
    for (size_t i = 2; i < param_index && i < args.size(); ++i)
        mode_msg += std::string(" ") + args[i];
    mode_msg += "\r\n";
    chan->broadcast(mode_msg, NULL);
}
