#include "../../includes/cmds/Quit.hpp"
#include "../../includes/Client.hpp"
#include "../../includes/Server.hpp"

Quit::Quit(Server& server) : ACommand(server) {}

Quit::~Quit() {}

void Quit::execute(Client* client, std::vector<std::string>& args)
{
	(void)args;
	
	client->set_state(DISCONNECTED);
}