#include "../../includes/cmds/Privmsg.hpp"
#include "../../includes/Client.hpp"
#include "../../includes/Server.hpp"
#include "../../includes/response.hpp"

Privmsg::Privmsg(Server& server) : ACommand(server) {}

Privmsg::~Privmsg() {}

void Privmsg::execute(Client* client, std::vector<std::string>& args)
{
	if (args.size() < 2 || args[0].empty() || args[1].empty())
	{
    	std::string client_id = client->get_nickname().empty() ? "*" : client->get_nickname();
		client->reply(ERR_NEEDMOREPARAMS(std::string("ft_irc"), client_id, "PRIVMSG"));
		return;
	} 

	std::string target = args[0];
	std::string message = args[1];

	// Determine if target is a channel (starts with # or &)
	if (target[0] == '#' || target[0] == '&')
	{
		Channel* chan = _server.get_channel(target);
		if (!chan)
		{
			std::string client_id = client->get_nickname().empty() ? "*" : client->get_nickname();
			client->reply(ERR_NOSUCHCHANNEL(std::string("ft_irc"), client_id, target));
			return;
		}
		// Check membership
		if (!chan->hasMember(client))
		{
			std::string client_id = client->get_nickname().empty() ? "*" : client->get_nickname();
			client->reply(ERR_CANNOTSENDTOCHAN(std::string("ft_irc"), client_id, target));
			return;
		}
		// Broadcast to channel members except sender
		std::string msg = ":" + client->get_prefix() + " PRIVMSG " + target + " :" + message + "\r\n";
		chan->broadcast(msg, client);
	}
	else
	{
		// Target is a user
		Client* target_client = _server.get_client_by_nickname(target);
		if (!target_client)
		{
			std::string client_id = client->get_nickname().empty() ? "*" : client->get_nickname();
			client->reply(ERR_NOSUCHNICK(std::string("ft_irc"), client_id, target));
			return;
		}

		// Send message to target user
		// Format: :<sender_prefix> PRIVMSG <target> :<message>
		std::string msg_to_send = ":" + client->get_prefix() + " PRIVMSG " + target + " :" + message + "\r\n";
		target_client->reply(msg_to_send);
	}
}
