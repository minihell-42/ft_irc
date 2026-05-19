#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <string>

// --- 001-099: Connection Registration ---
#define RPL_WELCOME(source, client)					":" + source + " 001 " + client + " :Welcome to the Internet Relay Network " + client + "\r\n"
#define RPL_YOURHOST(source, client, ver)			":" + source + " 002 " + client + " :Your host is " + source + ", running version " + ver + "\r\n"
#define RPL_CREATED(source, client, date)			":" + source + " 003 " + client + " :This server was created " + date + "\r\n"


// --- 400-599: Error Replies ---
#define ERR_NOSUCHNICK(source, client, nick)		":" + source + " 401 " + client + " " + nick + " :No such nick/channel\r\n"
#define ERR_NOSUCHCHANNEL(source, client, chan)		":" + source + " 403 " + client + " " + chan + " :No such channel\r\n"
#define ERR_NONICKNAMEGIVEN(source, client)			":" + source + " 431 " + client + " :No nickname given\r\n"
#define ERR_NICKNAMEINUSE(source, client, nick)		":" + source + " 433 " + client + " " + nick + " :Nickname is already in use\r\n"
#define ERR_NOTREGISTERED(source, client)			":" + source + " 451 " + client + " :You have not registered\r\n"
#define ERR_NEEDMOREPARAMS(source, client, cmd)		":" + source + " 461 " + client + " " + cmd + " :Not enough parameters\r\n"
#define ERR_ALREADYREGISTRED(source, client)		":" + source + " 462 " + client + " :Unauthorized command (already registered)\r\n"
#define ERR_PASSWDMISMATCH(source, client)			":" + source + " 464 " + client + " :Password incorrect\r\n"

#endif // RESPONSE_HPP
