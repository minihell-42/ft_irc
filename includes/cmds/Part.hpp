#ifndef PART_HPP
#define PART_HPP

#include "ACommand.hpp"

class Part : public ACommand {
  public:
    Part(Server& server);
    ~Part();

    void execute(Client* client, std::vector<std::string>& args);
};

#endif
