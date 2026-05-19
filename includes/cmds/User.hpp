#ifndef USER_HPP
#define USER_HPP

#include "ACommand.hpp"
#include "../../includes/Client.hpp"
#include "../../includes/Server.hpp"

class User : public ACommand {
    public:
        User(Server& server);
        ~User();

        void execute(Client* client, std::vector<std::string>& args);
};

#endif
