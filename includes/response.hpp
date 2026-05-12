#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <string>

/*
 * IRC Numeric Replies and Error Codes (RFC 1459 / RFC 2812)
 *
 * Source: Typically the server name (e.g., "irc.local")
 * Client: The nickname of the user receiving the message (or "*" if not yet registered)
 */

// --- 001-099: Connection Registration ---
#define RPL_WELCOME(source, client)					":" + source + " 001 " + client + " :Welcome to the Internet Relay Network " + client + "\r\n"
#define RPL_YOURHOST(source, client, ver)			":" + source + " 002 " + client + " :Your host is " + source + ", running version " + ver + "\r\n"
#define RPL_CREATED(source, client, date)			":" + source + " 003 " + client + " :This server was created " + date + "\r\n"
#define RPL_MYINFO(source, client, ver, umodes, cmodes) ":" + source + " 004 " + client + " " + source + " " + ver + " " + umodes + " " + cmodes + "\r\n"
#define RPL_ISUPPORT(source, client, tokens)		":" + source + " 005 " + client + " " + tokens + " :are supported by this server\r\n"

// --- 200-399: Command Responses ---
#define RPL_UMODEIS(source, client, modes)			":" + source + " 221 " + client + " " + modes + "\r\n"
#define RPL_LUSERCLIENT(source, client, users, inv, serv) ":" + source + " 251 " + client + " :There are " + users + " users and " + inv + " invisible on " + serv + " servers\r\n"
#define RPL_LUSEROP(source, client, ops)			":" + source + " 252 " + client + " " + ops + " :operator(s) online\r\n"
#define RPL_LUSERUNKNOWN(source, client, unknown)	":" + source + " 253 " + client + " " + unknown + " :unknown connection(s)\r\n"
#define RPL_LUSERCHANNELS(source, client, chans)	":" + source + " 254 " + client + " " + chans + " :channels formed\r\n"
#define RPL_LUSERME(source, client, users, serv)	":" + source + " 255 " + client + " :I have " + users + " clients and " + serv + " servers\r\n"
#define RPL_AWAY(source, client, nick, msg)			":" + source + " 301 " + client + " " + nick + " :" + msg + "\r\n"
#define RPL_UNAWAY(source, client)					":" + source + " 305 " + client + " :You are no longer marked as being away\r\n"
#define RPL_NOWAWAY(source, client)					":" + source + " 306 " + client + " :You have been marked as being away\r\n"
#define RPL_WHOISUSER(source, client, nick, user, host, real) ":" + source + " 311 " + client + " " + nick + " " + user + " " + host + " * :" + real + "\r\n"
#define RPL_WHOISSERVER(source, client, nick, serv, info) ":" + source + " 312 " + client + " " + nick + " " + serv + " :" + info + "\r\n"
#define RPL_WHOISOPERATOR(source, client, nick)		":" + source + " 313 " + client + " " + nick + " :is an IRC operator\r\n"
#define RPL_ENDOFWHO(source, client, mask)			":" + source + " 315 " + client + " " + mask + " :End of WHO list\r\n"
#define RPL_ENDOFWHOIS(source, client, nick)		":" + source + " 318 " + client + " " + nick + " :End of WHOIS list\r\n"
#define RPL_WHOISCHANNELS(source, client, nick, chans) ":" + source + " 319 " + client + " " + nick + " :" + chans + "\r\n"
#define RPL_LISTSTART(source, client)				":" + source + " 321 " + client + " Channel :Users  Name\r\n"
#define RPL_LIST(source, client, chan, count, topic) ":" + source + " 322 " + client + " " + chan + " " + count + " :" + topic + "\r\n"
#define RPL_LISTEND(source, client)					":" + source + " 323 " + client + " :End of LIST\r\n"
#define RPL_CHANNELMODEIS(source, client, chan, modes, args) ":" + source + " 324 " + client + " " + chan + " " + modes + " " + args + "\r\n"
#define RPL_NOTOPIC(source, client, chan)			":" + source + " 331 " + client + " " + chan + " :No topic is set\r\n"
#define RPL_TOPIC(source, client, chan, topic)		":" + source + " 332 " + client + " " + chan + " :" + topic + "\r\n"
#define RPL_INVITING(source, client, nick, chan)	":" + source + " 341 " + client + " " + nick + " " + chan + "\r\n"
#define RPL_NAMREPLY(source, client, symbol, chan, users) ":" + source + " 353 " + client + " " + symbol + " " + chan + " :" + users + "\r\n"
#define RPL_ENDOFNAMES(source, client, chan)		":" + source + " 366 " + client + " " + chan + " :End of NAMES list\r\n"
#define RPL_MOTD(source, client, text)				":" + source + " 372 " + client + " :- " + text + "\r\n"
#define RPL_MOTDSTART(source, client)				":" + source + " 375 " + client + " :- " + source + " Message of the day - \r\n"
#define RPL_ENDOFMOTD(source, client)				":" + source + " 376 " + client + " :End of MOTD command\r\n"
#define RPL_YOUREOPER(source, client)				":" + source + " 381 " + client + " :You are now an IRC operator\r\n"
#define RPL_TIME(source, client, serv, time)		":" + source + " 391 " + client + " " + serv + " :" + time + "\r\n"

// --- 400-599: Error Replies ---
#define ERR_NOSUCHNICK(source, client, nick)		":" + source + " 401 " + client + " " + nick + " :No such nick/channel\r\n"
#define ERR_NOSUCHSERVER(source, client, serv)		":" + source + " 402 " + client + " " + serv + " :No such server\r\n"
#define ERR_NOSUCHCHANNEL(source, client, chan)		":" + source + " 403 " + client + " " + chan + " :No such channel\r\n"
#define ERR_CANNOTSENDTOCHAN(source, client, chan)	":" + source + " 404 " + client + " " + chan + " :Cannot send to channel\r\n"
#define ERR_TOOMANYCHANNELS(source, client, chan)	":" + source + " 405 " + client + " " + chan + " :You have joined too many channels\r\n"
#define ERR_WASNOSUCHNICK(source, client, nick)		":" + source + " 406 " + client + " " + nick + " :There was no such nickname\r\n"
#define ERR_TOOMANYTARGETS(source, client, target)	":" + source + " 407 " + client + " " + target + " :Duplicate recipients. No message delivered\r\n"
#define ERR_NOORIGIN(source, client)				":" + source + " 409 " + client + " :No origin specified\r\n"
#define ERR_NORECIPIENT(source, client, cmd)		":" + source + " 411 " + client + " :No recipient given (" + cmd + ")\r\n"
#define ERR_NOTEXTTOSEND(source, client)			":" + source + " 412 " + client + " :No text to send\r\n"
#define ERR_UNKNOWNCOMMAND(source, client, cmd)		":" + source + " 421 " + client + " " + cmd + " :Unknown command\r\n"
#define ERR_NOMOTD(source, client)					":" + source + " 422 " + client + " :MOTD File is missing\r\n"
#define ERR_NONICKNAMEGIVEN(source, client)			":" + source + " 431 " + client + " :No nickname given\r\n"
#define ERR_ERRONEUSNICKNAME(source, client, nick)	":" + source + " 432 " + client + " " + nick + " :Erroneous nickname\r\n"
#define ERR_NICKNAMEINUSE(source, client, nick)		":" + source + " 433 " + client + " " + nick + " :Nickname is already in use\r\n"
#define ERR_USERNOTINCHANNEL(source, client, nick, chan) ":" + source + " 441 " + client + " " + nick + " " + chan + " :They aren't on that channel\r\n"
#define ERR_NOTONCHANNEL(source, client, chan)		":" + source + " 442 " + client + " " + chan + " :You're not on that channel\r\n"
#define ERR_USERONCHANNEL(source, client, nick, chan) ":" + source + " 443 " + client + " " + nick + " " + chan + " :is already on channel\r\n"
#define ERR_NOLOGIN(source, client, user)			":" + source + " 444 " + client + " " + user + " :User not logged in\r\n"
#define ERR_NOTREGISTERED(source, client)			":" + source + " 451 " + client + " :You have not registered\r\n"
#define ERR_NEEDMOREPARAMS(source, client, cmd)		":" + source + " 461 " + client + " " + cmd + " :Not enough parameters\r\n"
#define ERR_ALREADYREGISTRED(source, client)		":" + source + " 462 " + client + " :Unauthorized command (already registered)\r\n"
#define ERR_PASSWDMISMATCH(source, client)			":" + source + " 464 " + client + " :Password incorrect\r\n"
#define ERR_YOUREBANNEDCREEP(source, client)		":" + source + " 465 " + client + " :You are banned from this server\r\n"
#define ERR_KEYSET(source, client, chan)			":" + source + " 467 " + client + " " + chan + " :Channel key already set\r\n"
#define ERR_CHANNELISFULL(source, client, chan)		":" + source + " 471 " + client + " " + chan + " :Cannot join channel (+l)\r\n"
#define ERR_UNKNOWNMODE(source, client, mode, chan)	":" + source + " 472 " + client + " " + mode + " :is unknown mode char to me for " + chan + "\r\n"
#define ERR_INVITEONLYCHAN(source, client, chan)	":" + source + " 473 " + client + " " + chan + " :Cannot join channel (+i)\r\n"
#define ERR_BANNEDFROMCHAN(source, client, chan)	":" + source + " 474 " + client + " " + chan + " :Cannot join channel (+b)\r\n"
#define ERR_BADCHANNELKEY(source, client, chan)		":" + source + " 475 " + client + " " + chan + " :Cannot join channel (+k)\r\n"
#define ERR_BADCHANMASK(source, client, chan)		":" + source + " 476 " + client + " " + chan + " :Bad Channel Mask\r\n"
#define ERR_NOCHANMODES(source, client, chan)		":" + source + " 477 " + client + " " + chan + " :Channel doesn't support modes\r\n"
#define ERR_BANLISTFULL(source, client, chan, mode)	":" + source + " 478 " + client + " " + chan + " " + mode + " :Channel list is full\r\n"
#define ERR_NOPRIVILEGES(source, client)			":" + source + " 481 " + client + " :Permission Denied- You're not an IRC operator\r\n"
#define ERR_CHANOPRIVSNEEDED(source, client, chan)	":" + source + " 482 " + client + " " + chan + " :You're not channel operator\r\n"
#define ERR_CANTKILLSERVER(source, client)			":" + source + " 483 " + client + " :You can't kill a server!\r\n"
#define ERR_RESTRICTED(source, client)				":" + source + " 484 " + client + " :Your connection is restricted!\r\n"
#define ERR_NOOPERHOST(source, client)				":" + source + " 491 " + client + " :No O-lines for your host\r\n"
#define ERR_UMODEUNKNOWNFLAG(source, client)		":" + source + " 501 " + client + " :Unknown MODE flag\r\n"
#define ERR_USERSDONTMATCH(source, client)			":" + source + " 502 " + client + " :Cannot change mode for other users\r\n"

// --- Custom Messages ---
#define MSG_JOIN(prefix, chan)						":" + prefix + " JOIN :" + chan + "\r\n"
#define MSG_PART(prefix, chan, reason)				":" + prefix + " PART " + chan + (reason.empty() ? "" : " :" + reason) + "\r\n"
#define MSG_QUIT(prefix, reason)					":" + prefix + " QUIT :Quit: " + reason + "\r\n"
#define MSG_NICK(prefix, newnick)					":" + prefix + " NICK :" + newnick + "\r\n"
#define MSG_PRIVMSG(prefix, target, msg)			":" + prefix + " PRIVMSG " + target + " :" + msg + "\r\n"
#define MSG_NOTICE(prefix, target, msg)				":" + prefix + " NOTICE " + target + " :" + msg + "\r\n"
#define MSG_KICK(prefix, chan, target, reason)		":" + prefix + " KICK " + chan + " " + target + " :" + reason + "\r\n"
#define MSG_MODE(prefix, target, modes, args)		":" + prefix + " MODE " + target + " " + modes + (args.empty() ? "" : " " + args) + "\r\n"

#endif // RESPONSE_HPP
