NAME		:= philo
CC			:= cc
CFLAGS		:= -Wall -Wextra -Werror -g3 # -fsanitize=address
DEPFLAGS	:= -MMD -MP

INCLUDES	:= -Iincludes
SRCS		:= $(wildcard srcs/*.c) $(wildcard srcs/*/*.c)

OBJ_DIR		:= obj
OBJS		:= $(SRCS:%.c=$(OBJ_DIR)/%.o)
DEPS		:= $(OBJS:.o=.d)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

$(OBJ_DIR)/%.o : %.c
	@mkdir -p $(dir $@)
	$(CC) $(DEPFLAGS) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

-include $(wildcard $(DEPS))

.PHONY: all clean fclean re
