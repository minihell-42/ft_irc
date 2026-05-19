#ifndef LIST_HPP
#define LIST_HPP

#include "ACommand.hpp"

class List : public ACommand {
public:
    List(Server& server);
    ~List();

    void execute(Client* client, std::vector<std::string>& args);
};

#endif
