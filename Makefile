NAME		= no_castle_no_title

CXX			= g++
RM			= rm -rf

LIBS		= -lsfml-graphics -lsfml-audio -lsfml-window -lsfml-system
INCPATH		= include/
CPPFLAGS	=  -I $(INCPATH)
CXXFLAGS	= -W -Wall -Werror -Wextra -std=c++11 $(LIBS)

SRCPATH		= src
SRC			= $(wildcard $(SRCPATH)/*.cpp)
OBJ			= $(SRC:.cpp=.o)


all: $(NAME)

$(NAME): $(OBJ)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -o $@ $^ $(LIBS)

src/%.o: src/%.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c -o $@ $<

clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY:: all clean fclean re
