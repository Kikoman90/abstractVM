NAME = avm

CLANG = clang++

FLAGS = -std=c++11 -Wall -Wextra -Werror -Wno-deprecated-register

HEADERS = includes

DIR_LEX = flex

DIR_SRCS = sources

DIR_OBJS = tmp

SRCS =	main.cpp \
		tokens.cpp \
		Lexer.cpp \
		Parser.cpp \
		OpFactory.cpp \
		AVMException.cpp \

LEX = tokens.flex

LEXR = $(addprefix $(DIR_SRCS)/, $(LEX:.flex=.cpp))

OBJS = $(addprefix $(DIR_OBJS)/, $(SRCS:.cpp=.o))

all: $(NAME)

$(NAME): $(LEXR) $(OBJS)
		@$(CLANG) $(FLAGS) -o $@ $(OBJS)
		@echo "\033[35;1m[abstractVM] compilation SUCCESS"

$(DIR_SRCS)/%.cpp: $(DIR_LEX)/%.flex
		@flex -o $@ $<

$(DIR_OBJS)/%.o: $(DIR_SRCS)/%.cpp
		@mkdir -p tmp
		@$(CLANG) $(FLAGS) -I $(HEADERS) -c -o $@ $<

clean:
		@rm -rf $(DIR_OBJS)
		@echo "cleaning object files (.o)"

fclean: clean
		@rm -f $(NAME)
		@echo "cleaning executable abstractVM"

re: fclean all

.PHONY: clean fclean re
