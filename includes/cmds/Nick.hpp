#ifndef NICK_HPP
#define NICK_HPP

#include "ACommand.hpp"

class Nick : public ACommand {
    public:
        Nick(Server& server);
        ~Nick();

        void execute(Client* client, std::vector<std::string>& args);
};

#endif
