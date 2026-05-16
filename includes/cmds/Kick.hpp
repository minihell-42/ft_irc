#ifndef KICK_HPP
#define KICK_HPP

#include "ACommand.hpp"

class Kick : public ACommand {
public:
    Kick(Server& server);
    ~Kick();

    void execute(Client* client, std::vector<std::string>& args);
};

#endif
