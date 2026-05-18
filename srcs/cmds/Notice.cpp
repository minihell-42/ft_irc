#include "../../includes/cmds/Notice.hpp"
#include "../../includes/Client.hpp"
#include "../../includes/Server.hpp"

Notice::Notice(Server& server) : ACommand(server) {}

Notice::~Notice() {}

void Notice::execute(Client* client, std::vector<std::string>& args)
{
	if (args.size() < 2 || args[0].empty() || args[1].empty())
		return;
	
	std::string target = args[0];
	std::string message = args[1];

	if (target[0] == '#' || target[0] == '&')
	{
		// TODO: Channel messaging implementation
		// If channel doesn't exist or client can't send, silently ignore
	}
	else
	{
		Client* targetClient = _server.getClientByNickname(target);
		if (!targetClient)
			return;

		// Format: :<sender_prefix> NOTICE <target> :<message>
		std::string msgToSend = ":" + client->getPrefix() + " NOTICE " + target + " :" + message + "\r\n";
		targetClient->reply(msgToSend);
	}
}
