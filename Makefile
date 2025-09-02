UNAME_S := $(shell uname -s)

CC          = g++
RM          = rm -f

NAME        = project

CXXFLAGS    = -std=c++23 -Wall -Wextra -Wno-deprecated-declarations

ifeq ($(UNAME_S), Darwin)
    LDFLAGS     = -lm -lassimp -lglfw -framework OpenGL -L/opt/homebrew/lib
	CPPFLAGS    = -iquote ./include -I ./glad -I ./imgui -I/opt/homebrew/include
else
    LDFLAGS     = -lm -lassimp -lglfw -lGL
	CPPFLAGS    = -iquote ./include -I ./glad -I ./imgui
endif

SRC              = $(wildcard source/**/**/*.cpp) \
                   $(wildcard source/**/*.cpp) \
                   main.cpp

SRC_DEPENDENCIES = $(wildcard glad/*.cpp) \
                   $(wildcard imgui/*.cpp)

OBJ              = $(SRC:.cpp=.o)

OBJ_DEPENDENCIES = $(SRC_DEPENDENCIES:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJ) $(OBJ_DEPENDENCIES)
	@$(CC) $(OBJ) $(OBJ_DEPENDENCIES) -o $(NAME) $(LDFLAGS)
	@echo -e '\e[1m\e[32m👌 $(NAME) compiled\e[0m'

%.o: %.cpp
	@echo -e '\e[2mCompiling $@\e[0m'
	@$(CC) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

clean:
	@echo -e '\e[1m\e[91m🗑 Deleting compilation files\e[0m'
	@$(RM) $(OBJ) $(OBJ_DEPENDENCIES)
	@$(RM) profile.txt gmon.out
	@find . -type f -name '*.gcda' -delete
	@find . -type f -name '*.gcno' -delete
	@find . -type f -name '*.gcov' -delete
	@find . -type f -name 'vgcore.*' -delete

flush:
	@echo -e '\e[1m\e[91m🗑 Flushing source .o files (skipping dependencies)\e[0m'
	@$(RM) $(OBJ)

fclean: clean
	@echo -e '\e[1m\e[91m🗑 Deleting binaries\e[0m\n'
	@$(RM) $(NAME)
	@$(RM) $(NAME)_debug
	@$(RM) $(wildcard shaders/*.spv)

re: fclean all

debug:
	@echo -e '\e[1m\e[94m🚀 Compile debug binary\e[0m'
	@$(CC) -o $(NAME)_debug -g -g3 $(CXXFLAGS) $(CPPFLAGS) $(LDFLAGS) $(SRC) $(SRC_DEPENDENCIES)

gdb: debug
	@echo -e '\e[1m\e[94m🚀 Run gdb\e[0m\n'
	@gdb --quiet $(NAME)_debug $(ARGS)

valgrind: debug
	@echo -e '\e[1m\e[94m🚀 Run valgrind\e[0m\n'
	@valgrind ./$(NAME)_debug $(ARGS)

leak: debug
	@echo -e '\e[1m\e[94m🚀 Run valgrind (\e[34mleak\e[94m)\e[0m\n'
	@valgrind --leak-check=full ./$(NAME)_debug $(ARGS)

origins: debug
	@echo -e '\e[1m\e[94m🚀 Run valgrind (\e[34mtrack-origins\e[94m)\e[0m\n'
	@valgrind --track-origins=yes ./$(NAME)_debug $(ARGS)

profile:
	@echo -e '\e[1m\e[94m🚀 Profile binary\e[0m\n'
	@$(CC) -o $(NAME)_profile -pg $(CXXFLAGS) $(CPPFLAGS) $(SRC)
	@./$(NAME)_profile $(ARGS)
	@gprof $(NAME) gmon.out > profile.txt
	@echo -e '\e[1m\e[90m⚠️  Profiling saved in profile.txt\e[0m'

.PHONY: all clean flush fclean re debug gdb valgrind leak origins profile
