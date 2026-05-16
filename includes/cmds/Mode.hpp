#ifndef MODE_HPP
#define MODE_HPP

#include "ACommand.hpp"

class Mode : public ACommand {
public:
    Mode(Server& server);
    ~Mode();

    void execute(Client* client, std::vector<std::string>& args);
};

#endif
