#include "../includes/Channel.hpp"
#include "../includes/Client.hpp"

Channel::Channel(const std::string &name)
    : _name(name), _topic(""), _key(""), _limit(0), _inviteOnly(false)
{
    _topicRestricted = false;
}

Channel::~Channel() {}

// Getters
std::string Channel::getName() const { return _name; }

std::string Channel::getTopic() const { return _topic; }

std::string Channel::getKey() const { return _key; }

std::string Channel::getMemberNames() const
{
    std::string users;
    for (std::set<Client*>::const_iterator it = _members.begin(); it != _members.end(); ++it)
    {
        Client* c = *it;
        if (!c) continue;
        if (!users.empty()) users += " ";
        users += c->getNickname();
    }
    return users;
}

int Channel::getLimit() const { return _limit; }

// Setters
void Channel::setLimit(int limit) { _limit = limit; }

void Channel::setInviteOnly(bool v) { _inviteOnly = v; }

void Channel::setKey(const std::string &key) { _key = key; }

void Channel::setTopicRestricted(bool v) { _topicRestricted = v; }

void Channel::setTopic(const std::string &topic) { _topic = topic; }

// Members
int Channel::memberCount() const
{
    return static_cast<int>(_members.size());
}

bool Channel::addMember(Client* client)
{
    if (!client) return false;
    if (_limit > 0 && static_cast<int>(_members.size()) >= _limit)
        return false;
    _members.insert(client);
    return true;
}

void Channel::removeMember(Client* client)
{
    if (!client) return;
    _members.erase(client);
    _operators.erase(client);
}

bool Channel::hasMember(Client* client) const
{
    if (!client) return false;
    return _members.find(client) != _members.end();
}

// Broadcast
void Channel::broadcast(const std::string &msg, Client* except) const
{
    for (std::set<Client*>::const_iterator it = _members.begin(); it != _members.end(); ++it)
    {
        Client* c = *it;
        if (c && c != except)
            c->reply(msg);
    }
}

// Operators
void Channel::addOperator(Client* client)
{
    if (!client) return;
    _operators.insert(client);
}

void Channel::removeOperator(Client* client)
{
    if (!client) return;
    _operators.erase(client);
}

bool Channel::isOperator(Client* client) const
{
    if (!client) return false;
    return _operators.find(client) != _operators.end();
}

// Invites
bool Channel::isInviteOnly() const { return _inviteOnly; }
void Channel::invite(const std::string &nick) { _invites.insert(nick); }
void Channel::removeInvite(const std::string &nick) { _invites.erase(nick); }
bool Channel::isInvited(const std::string &nick) const { return _invites.find(nick) != _invites.end(); }

// Topic
bool Channel::isTopicRestricted() const { return _topicRestricted; }
