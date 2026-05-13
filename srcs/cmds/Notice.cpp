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
		Client* target_client = _server.get_client_by_nickname(target);
		if (!target_client)
			return;

		// Format: :<sender_prefix> NOTICE <target> :<message>
		std::string msg_to_send = ":" + client->get_prefix() + " NOTICE " + target + " :" + message + "\r\n";
		target_client->reply(msg_to_send);
	}
}
