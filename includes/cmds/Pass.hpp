#ifndef PASS_HPP
#define PASS_HPP

#include "ACommand.hpp"
#include <iostream>

class Pass : public ACommand {
public:
    Pass(Server& server);
    ~Pass();

    void execute(Client* client, std::vector<std::string>& args);
};

#endif
