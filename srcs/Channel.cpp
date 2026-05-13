#include "../includes/Channel.hpp"
#include "../includes/Client.hpp"

Channel::Channel(const std::string &name)
    : _name(name), _topic(""), _invite_only(false), _key("") , _limit(0)
{
}

Channel::~Channel()
{
}

std::string Channel::get_name() const { return _name; }
std::string Channel::get_topic() const { return _topic; }
void Channel::set_topic(const std::string &topic) { _topic = topic; }

bool Channel::add_member(Client* client)
{
    if (!client) return false;
    if (_limit > 0 && static_cast<int>(_members.size()) >= _limit)
        return false;
    _members.insert(client);
    return true;
}

void Channel::remove_member(Client* client)
{
    if (!client) return;
    _members.erase(client);
    _operators.erase(client);
}

bool Channel::has_member(Client* client) const
{
    if (!client) return false;
    return _members.find(client) != _members.end();
}

void Channel::broadcast(const std::string &msg, Client* except) const
{
    for (std::set<Client*>::const_iterator it = _members.begin(); it != _members.end(); ++it)
    {
        Client* c = *it;
        if (c && c != except)
            c->reply(msg);
    }
}

int Channel::member_count() const
{
    return static_cast<int>(_members.size());
}

std::string Channel::get_member_names() const
{
    std::string users;
    for (std::set<Client*>::const_iterator it = _members.begin(); it != _members.end(); ++it)
    {
        Client* c = *it;
        if (!c) continue;
        if (!users.empty()) users += " ";
        users += c->get_nickname();
    }
    return users;
}

void Channel::add_operator(Client* client)
{
    if (!client) return;
    _operators.insert(client);
}

void Channel::remove_operator(Client* client)
{
    if (!client) return;
    _operators.erase(client);
}

bool Channel::is_operator(Client* client) const
{
    if (!client) return false;
    return _operators.find(client) != _operators.end();
}

void Channel::set_invite_only(bool v) { _invite_only = v; }
bool Channel::is_invite_only() const { return _invite_only; }
void Channel::invite(const std::string &nick) { _invites.insert(nick); }
bool Channel::is_invited(const std::string &nick) const { return _invites.find(nick) != _invites.end(); }

void Channel::set_key(const std::string &key) { _key = key; }
std::string Channel::get_key() const { return _key; }

void Channel::set_limit(int limit) { _limit = limit; }
int Channel::get_limit() const { return _limit; }
