#ifndef NOTICE_HPP
#define NOTICE_HPP

#include "ACommand.hpp"

class Notice : public ACommand {
public:
    Notice(Server& server);
    ~Notice();

    void execute(Client* client, std::vector<std::string>& args);
};

#endif
