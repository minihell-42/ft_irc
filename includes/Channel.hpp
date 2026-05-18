#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <string>
#include <set>

class Client;

class Channel {
	private:
		std::string						_name;
		std::string						_topic;
		std::string						_key;
		std::set<Client*>			_members;
		std::set<Client*> 		_operators;
		std::set<std::string>	_invites;
		int 									_limit; // 0 means unlimited
		bool 									_inviteOnly;
		bool 									_topicRestricted;

	public:
		Channel(const std::string &name);
		~Channel();

		std::string getName() const;
		std::string getTopic() const;
		std::string getKey() const;
		std::string getMemberNames() const;
		int 				getLimit() const;
		
		void 				setLimit(int limit);
		void 				setInviteOnly(bool v);
		void 				setTopicRestricted(bool v);
		void 				setKey(const std::string &key);
		void 				setTopic(const std::string &topic);
		
		int 				memberCount() const;
		bool 				addMember(Client* client);
		void 				removeMember(Client* client);
		bool 				hasMember(Client* client) const;

		void 				broadcast(const std::string &msg, Client* except = NULL) const;

		void 				addOperator(Client* client);
		void 				removeOperator(Client* client);
		bool 				isOperator(Client* client) const;

		bool 				isInviteOnly() const;
		void 				invite(const std::string &nick);
		void 				removeInvite(const std::string &nick);
		bool 				isInvited(const std::string &nick) const;

		bool 				isTopicRestricted() const;
};

#endif