#ifndef PING_HPP
#define PING_HPP

#include "ACommand.hpp"

class Ping : public ACommand {
    public:
        Ping(Server& server);
        ~Ping();

        void execute(Client* client, std::vector<std::string>& args);
};

#endif
