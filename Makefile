NAME = avm

CLANG = clang++

FLAGS = -std=c++11 -Wall -Wextra -Werror -Wno-deprecated-register -MMD

HEADERS = includes

DIR_SRCS = sources

DIR_OBJS = tmp

SRCS =	main.cpp \
		Lexer.cpp \
		Parser.cpp \
		OpFactory.cpp \
		AVMException.cpp \

DPDS = $(addprefix $(DIR_OBJS)/, $(SRCS:.cpp=.d))

OBJS = $(addprefix $(DIR_OBJS)/, $(SRCS:.cpp=.o))

opti:
		@make -j all

all: $(NAME)

$(NAME): $(OBJS)
		@$(CLANG) $(FLAGS) -o $@ $(OBJS)
		@echo "\033[35;1m[abstractVM] compilation SUCCESS"

$(DIR_OBJS)/%.o: $(DIR_SRCS)/%.cpp
		@mkdir -p tmp
		@$(CLANG) $(FLAGS) -I $(HEADERS) -c -o $@ $<

clean:
		@rm -rf $(DIR_OBJS)
		@echo "cleaning object files (.o)"

fclean: clean
		@rm -f $(NAME)
		@echo "cleaning executable 'avm'"

re:
		@make fclean
		@make opti

.PHONY: all clean fclean re

-include $(DPDS)
