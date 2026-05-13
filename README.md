# ft_irc

This project is a custom implementation of an Internet Relay Chat (IRC) server in C++98, built according to RFC 2812. It is designed to handle multiple concurrent client connections using an asynchronous, event-driven architecture.

## Current Features & Architecture

### Core Networking & Event Loop
- **Non-Blocking I/O:** All server and client sockets operate in non-blocking mode (`O_NONBLOCK`).
- **Event-Driven Polling:** Uses the `poll()` system call to monitor state changes across all connected file descriptors efficiently.
- **Robust Buffer Management:** 
  - **Read Buffers:** Each client has a dedicated read buffer that re-assembles fragmented TCP packets until a complete command (ending in `\r\n`) is received.
  - **Write Buffers:** Prevents `send()` blocking issues by queueing outbound messages into a write buffer. The server waits for a `POLLOUT` event before attempting to flush the buffer to the socket, preventing lost messages on slow connections.
- **Security & Stability:** Incorporates protections against buffer overflow attacks by forcibly disconnecting malicious clients that send excessive data without valid line terminations.

### Command Routing & Authentication
- **Command Pattern:** Utilizes an object-oriented Command Pattern (`ACommand`) to route, parse, and execute incoming IRC messages cleanly.
- **Strict Registration Flow:** Enforces the standard IRC connection sequence. Clients must successfully authenticate before they are allowed to interact with the server:
  1. `PASS` (Server password verification)
  2. `NICK` (Unique nickname selection)
  3. `USER` (Username and realname definition)
- **Connection Management:** Implements the `QUIT` command for graceful disconnection and the `PING`/`PONG` commands to keep client connections alive.
- **Unauthorized Command Protection:** Any attempt to execute operational commands (like `JOIN` or `PRIVMSG`) prior to completing the registration sequence is intercepted and rejected with an `ERR_NOTREGISTERED` response.
- **Automatic Registration:** The server intelligently detects when a client has fulfilled all authentication criteria (regardless of the order they send it) and automatically upgrades their state to `REGISTERED`, firing off the standard IRC Welcome messages (`001`, `002`, `003`, `004`).

### Messaging
- **Direct Messaging (`PRIVMSG`):** Users can send private messages to other users by nickname. The server looks up the target client and routes the message appropriately.
- **Automated Notices (`NOTICE`):** Fully supports the `NOTICE` command, behaving identically to `PRIVMSG` but strictly adhering to RFC 2812 by never sending automatic error replies (preventing infinite loops between bots).

## Next Steps

1. **Channel Management:**
   - **`JOIN` & `PART`:** Allow users to create, enter, and leave channels.
   - **Broadcasting:** Implement the logic to broadcast messages to all users within a specific channel.
   - **`TOPIC`:** Allow users to view and change channel topics.

2. **Channel Operators & Modes:**
   - Implement channel operator privileges (`+o`) and mode management (`MODE` command).
   - Implement operator-only commands such as `KICK` and `INVITE`.