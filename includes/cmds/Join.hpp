#ifndef JOIN_HPP
#define JOIN_HPP

#include "ACommand.hpp"

class Join : public ACommand {
    public:
        Join(Server& server);
        ~Join();

        void execute(Client* client, std::vector<std::string>& args);
};

#endif
