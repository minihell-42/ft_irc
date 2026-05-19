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
    	std::string clientId = client->getNickname().empty() ? "*" : client->getNickname();
		client->reply(ERR_NEEDMOREPARAMS(std::string("ft_irc"), clientId, "PRIVMSG"));
		return;
	} 

	std::string target = args[0];
	std::string message = args[1];

	// Determine if target is a channel (starts with # or &)
	if (target[0] == '#' || target[0] == '&')
	{
		Channel* chan = _server.getChannel(target);
		if (!chan)
		{
			std::string clientId = client->getNickname().empty() ? "*" : client->getNickname();
			client->reply(ERR_NOSUCHCHANNEL(std::string("ft_irc"), clientId, target));
			return;
		}
		// Check membership
		if (!chan->hasMember(client))
		{
			std::string clientId = client->getNickname().empty() ? "*" : client->getNickname();
			client->reply(ERR_CANNOTSENDTOCHAN(std::string("ft_irc"), clientId, target));
			return;
		}
		// Broadcast to channel members except sender
		std::string msg = ":" + client->getPrefix() + " PRIVMSG " + target + " :" + message + "\r\n";
		chan->broadcast(msg, client);
	}
	else
	{
		// Target is a user
		Client* targetClient = _server.getClientByNickname(target);
		if (!targetClient)
		{
			std::string clientId = client->getNickname().empty() ? "*" : client->getNickname();
			client->reply(ERR_NOSUCHNICK(std::string("ft_irc"), clientId, target));
			return;
		}

		// Send message to target user
		// Format: :<sender_prefix> PRIVMSG <target> :<message>
		std::string msgToSend = ":" + client->getPrefix() + " PRIVMSG " + target + " :" + message + "\r\n";
		targetClient->reply(msgToSend);
	}
}
