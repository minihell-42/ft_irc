#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <string>

// --- 001-099: Connection Registration ---
#define RPL_WELCOME(source, client)					":" + source + " 001 " + client + " :Welcome to the Internet Relay Network " + client + "\r\n"
#define RPL_YOURHOST(source, client, ver)			":" + source + " 002 " + client + " :Your host is " + source + ", running version " + ver + "\r\n"
#define RPL_CREATED(source, client, date)			":" + source + " 003 " + client + " :This server was created " + date + "\r\n"

// --- 300-399: Command Responses ---
#define RPL_LISTSTART(source, client)               ":" + source + " 321 " + client + " Channel :Users  Name\r\n"
#define RPL_LIST(source, client, chan, count, topic) ":" + source + " 322 " + client + " " + chan + " " + count + " :" + topic + "\r\n"
#define RPL_LISTEND(source, client)                 ":" + source + " 323 " + client + " :End of /LIST\r\n"
#define RPL_NOTOPIC(source, client, chan)           ":" + source + " 331 " + client + " " + chan + " :No topic is set\r\n"
#define RPL_TOPIC(source, client, chan, topic)      ":" + source + " 332 " + client + " " + chan + " :" + topic + "\r\n"
#define RPL_INVITING(source, client, target, chan)  ":" + source + " 341 " + client + " " + target + " " + chan + "\r\n"
#define RPL_NAMREPLY(source, client, symbol, chan, users) ":" + source + " 353 " + client + " " + symbol + " " + chan + " :" + users + "\r\n"
#define RPL_ENDOFNAMES(source, client, chan)        ":" + source + " 366 " + client + " " + chan + " :End of /NAMES list\r\n"

// --- 400-599: Error Replies ---
#define ERR_NOSUCHNICK(source, client, nick)		":" + source + " 401 " + client + " " + nick + " :No such nick/channel\r\n"
#define ERR_NOSUCHCHANNEL(source, client, chan)		":" + source + " 403 " + client + " " + chan + " :No such channel\r\n"
#define ERR_CANNOTSENDTOCHAN(source, client, chan)  ":" + source + " 404 " + client + " " + chan + " :Cannot send to channel\r\n"
#define ERR_NONICKNAMEGIVEN(source, client)			":" + source + " 431 " + client + " :No nickname given\r\n"
#define ERR_NICKNAMEINUSE(source, client, nick)		":" + source + " 433 " + client + " " + nick + " :Nickname is already in use\r\n"
#define ERR_USERNOTINCHANNEL(source, client, target, chan) ":" + source + " 441 " + client + " " + target + " " + chan + " :They aren't on that channel\r\n"
#define ERR_NOTONCHANNEL(source, client, chan)      ":" + source + " 442 " + client + " " + chan + " :You're not on that channel\r\n"
#define ERR_USERONCHANNEL(source, client, target, chan) ":" + source + " 443 " + client + " " + target + " " + chan + " :is already on channel\r\n"
#define ERR_NOTREGISTERED(source, client)			":" + source + " 451 " + client + " :You have not registered\r\n"
#define ERR_NEEDMOREPARAMS(source, client, cmd)		":" + source + " 461 " + client + " " + cmd + " :Not enough parameters\r\n"
#define ERR_ALREADYREGISTRED(source, client)		":" + source + " 462 " + client + " :Unauthorized command (already registered)\r\n"
#define ERR_PASSWDMISMATCH(source, client)			":" + source + " 464 " + client + " :Password incorrect\r\n"
#define ERR_CHANNELISFULL(source, client, chan)     ":" + source + " 471 " + client + " " + chan + " :Cannot join channel (+l)\r\n"
#define ERR_UNKNOWNMODE(source, client, mode, chan) ":" + source + " 472 " + client + " " + mode + " :is unknown mode char to me for " + chan + "\r\n"
#define ERR_INVITEONLYCHAN(source, client, chan)    ":" + source + " 473 " + client + " " + chan + " :Cannot join channel (+i)\r\n"
#define ERR_BADCHANNELKEY(source, client, chan)     ":" + source + " 475 " + client + " " + chan + " :Cannot join channel (+k)\r\n"
#define ERR_CHANOPRIVSNEEDED(source, client, chan)  ":" + source + " 482 " + client + " " + chan + " :You're not channel operator\r\n"

// --- Custom Messages ---
#define MSG_JOIN(source, chan)                      ":" + source + " JOIN :" + chan + "\r\n"
#define MSG_PART(source, chan, reason)              ":" + source + " PART " + chan + " :" + reason + "\r\n"
#define MSG_KICK(source, chan, target, reason)      ":" + source + " KICK " + chan + " " + target + " :" + reason + "\r\n"
#define MSG_NICK(source, newnick)                   ":" + source + " NICK :" + newnick + "\r\n"

#endif // RESPONSE_HPP
