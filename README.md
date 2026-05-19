*This project has been created as part of the 42 curriculum by samcasti and dgomez-a.

## Description
**ft_irc** is a custom Internet Relay Chat (IRC) server written in C++98. The goal of this project is to implement an RFC-compliant IRC server capable of handling multiple concurrent clients using a single-threaded non-blocking I/O multiplexing architecture (via `poll()`). This project focuses heavily on socket programming, networking protocols, command parsing, and robust state management.

## Instructions

### Compilation
To compile the project, run `make` at the root of the repository. This will compile the source files and generate the executable:
```bash
make
```

### Execution
The server requires a port number and a connection password to start:
```bash
./ircserv <port> <password>
```
Example:
```bash
./ircserv 6667 my_secret_password
```

### Connecting to the Server
You can connect to the server using a standard IRC client (such as `irssi`, `WeeChat`, or `kvirc`) or simply via `nc` (Netcat):
```bash
nc 127.0.0.1 6667
```
Once connected, you must authenticate using the password provided during server startup (e.g., `PASS my_secret_password`) and register your nickname and user details.

## Implemented Features

This server implements the core functionalities of an IRC server following RFC 2812. It supports multiple clients, channel management, and various commands using a flexible **Command Pattern** architecture.

### Supported Commands & Usage

Here is a list of the implemented IRC commands and how to use them:

#### Connection & Registration
- **PASS**: Set the connection password. Must be the first command sent.
  - Usage: `PASS <password>`
- **NICK**: Set or change your nickname.
  - Usage: `NICK <nickname>`
- **USER**: Specify user details (username, hostname, servername, realname).
  - Usage: `USER <username> 0 * :<real name>`
- **QUIT**: Disconnect from the server.
  - Usage: `QUIT [:<quit message>]`
- **PING**: Check if the server is responsive.
  - Usage: `PING <token>`

#### Communication
- **PRIVMSG**: Send a private message to a user or a channel.
  - Usage: `PRIVMSG <target> :<message>`
- **NOTICE**: Send a notice message (similar to PRIVMSG, but never triggers automated replies).
  - Usage: `NOTICE <target> :<message>`

#### Channel Management
- **JOIN**: Join a channel or create it if it doesn't exist.
  - Usage: `JOIN <channel> [<key>]`
- **PART**: Leave a channel.
  - Usage: `PART <channel> [:<part message>]`
- **TOPIC**: View or change the topic of a channel.
  - Usage: `TOPIC <channel> [:<new topic>]`
- **NAMES**: List the users in a channel.
  - Usage: `NAMES <channel>`
- **LIST**: List all channels on the server.
  - Usage: `LIST`
- **INVITE**: Invite a user to a channel.
  - Usage: `INVITE <nickname> <channel>`
- **KICK**: Forcibly remove a user from a channel (requires operator privileges).
  - Usage: `KICK <channel> <nickname> [:<reason>]`
- **MODE**: Change channel modes (requires operator privileges).
  - Usage: `MODE <channel> <+modes|-modes> [<mode parameters>]`
  - *Supported Modes*:
    - `i`: Invite-only channel (`+i` / `-i`)
    - `t`: Topic set restricted to channel operators (`+t` / `-t`)
    - `k`: Set/remove the channel key (password) (`+k <password>` / `-k`)
    - `o`: Give/take channel operator privilege (`+o <nickname>` / `-o <nickname>`)
    - `l`: Set/remove the user limit to channel (`+l <limit>` / `-l`)

## Resources

### References
- **RFC 2812 (Internet Relay Chat: Client Protocol):** The primary standard outlining client-server communication for IRC. [https://datatracker.ietf.org/doc/html/rfc2812](https://datatracker.ietf.org/doc/html/rfc2812)
- **Beej's Guide to Network Programming:** An essential resource for understanding sockets, `bind()`, `listen()`, `accept()`, and `poll()`. [https://beej.us/guide/bgnet/](https://beej.us/guide/bgnet/)
- **Modern IRC Client Protocol:** A more readable and modern interpretation of the IRC specifications. [https://modern.ircdocs.horse/](https://modern.ircdocs.horse/)

### AI Usage
During the development of this project, Artificial Intelligence was used in the following ways:
- **Bug Fixing and C++98 Compliance:** Assisting in identifying memory leaks, resolving template linking errors, and ensuring that modern C++ patterns were correctly adapted to strict C++98 standards.
- **Protocol Clarification:** Summarizing and clarifying specific behaviors mandated by RFC 2812, such as proper command sequences, numeric replies, and state transitions.
- **Documentation and Architecture:** Aiding in the generation of documentation files, architectural notes, and providing clear explanations of complex network system calls (e.g., `poll()` and non-blocking I/O).
