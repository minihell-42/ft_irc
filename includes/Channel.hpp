
#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <string>
#include <set>

class Client;

class Channel
{
private:
	std::string _name;
	std::string _topic;
	std::set<Client*> _members;
	std::set<Client*> _operators;
	std::set<std::string> _invites;
	bool _invite_only;
	std::string _key;
	int _limit; // 0 means unlimited
	bool _topic_restricted;

public:
	Channel(const std::string &name);
	~Channel();

	std::string get_name() const;
	std::string get_topic() const;
	void set_topic(const std::string &topic);

	bool add_member(Client* client);
	void remove_member(Client* client);
	bool has_member(Client* client) const;

	void broadcast(const std::string &msg, Client* except = NULL) const;

	int member_count() const;
	std::string get_member_names() const;

	void add_operator(Client* client);
	void remove_operator(Client* client);
	bool is_operator(Client* client) const;

	void set_invite_only(bool v);
	bool is_invite_only() const;
	void invite(const std::string &nick);
	bool is_invited(const std::string &nick) const;

	void set_key(const std::string &key);
	std::string get_key() const;

	void set_limit(int limit);
	int get_limit() const;
	void set_topic_restricted(bool v);
	bool is_topic_restricted() const;
};

#endif