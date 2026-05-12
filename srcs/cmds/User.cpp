#include "../../includes/cmds/User.hpp"

User::User(Server& server) : ACommand(server) {}

User::~User() {}

void User::execute(Client* client, std::vector<std::string>& args)
{

}
