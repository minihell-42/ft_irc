#ifndef NAMES_HPP
#define NAMES_HPP

#include "ACommand.hpp"

class Names : public ACommand {
public:
    Names(Server& server);
    ~Names();

    void execute(Client* client, std::vector<std::string>& args);
};

#endif
