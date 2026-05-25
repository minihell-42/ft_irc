NAME = ircserv

CC = c++
FLAGS = -Wall -Wextra -Werror -std=c++98
INCLUDES = -I ./includes

SRCS_DIR = srcs
OBJ_DIR = obj

SRCS = \
	$(SRCS_DIR)/main.cpp \
	$(SRCS_DIR)/Channel.cpp \
	$(SRCS_DIR)/Client.cpp \
	$(SRCS_DIR)/Server.cpp \
	$(SRCS_DIR)/cmds/ACommand.cpp \
	$(SRCS_DIR)/cmds/Invite.cpp \
	$(SRCS_DIR)/cmds/Join.cpp \
	$(SRCS_DIR)/cmds/Kick.cpp \
	$(SRCS_DIR)/cmds/List.cpp \
	$(SRCS_DIR)/cmds/Mode.cpp \
	$(SRCS_DIR)/cmds/Names.cpp \
	$(SRCS_DIR)/cmds/Nick.cpp \
	$(SRCS_DIR)/cmds/Notice.cpp \
	$(SRCS_DIR)/cmds/Part.cpp \
	$(SRCS_DIR)/cmds/Pass.cpp \
	$(SRCS_DIR)/cmds/Ping.cpp \
	$(SRCS_DIR)/cmds/Privmsg.cpp \
	$(SRCS_DIR)/cmds/Quit.cpp \
	$(SRCS_DIR)/cmds/Topic.cpp \
	$(SRCS_DIR)/cmds/User.cpp
OBJS = $(patsubst $(SRCS_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRCS))

RM = rm -rf

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(FLAGS) $(INCLUDES) $(OBJS) -o $(NAME)

$(OBJ_DIR)/%.o: $(SRCS_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CC) $(FLAGS) $(INCLUDES) -c $< -o $@

clean:
	$(RM) $(OBJ_DIR)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
