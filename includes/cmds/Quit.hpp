#ifndef QUIT_HPP
#define QUIT_HPP

#include "ACommand.hpp"
#include <iostream>

class Quit : public ACommand {
    public:
        Quit(Server& server);
        ~Quit();

        void execute(Client* client, std::vector<std::string>& args);
};

#endif