NAME=game
FLAGS=-Wall -Werror -Wextra
SRCS=srcs/main.cpp srcs/Rectangle.cpp srcs/Ball.cpp
OBJS=$(SRCS:.cpp=.o)
RAYLIB=raylib/raylib/libraylib.a

all: $(NAME)

$(RAYLIB):
	@cd raylib/ \
	&& cmake . -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=OFF \
	&& make

%.o: %.cpp
	@c++ $(FLAGS) -c $< -o $@

$(NAME):$(OBJS) $(RAYLIB)
	@c++ $(FLAGS) $(OBJS) $(RAYLIB) -lm -o $(NAME)

clean:
	@rm -fr $(OBJS)

fclean: clean
	@rm -fr $(NAME)
	@rm -fr $(RAYLIB)

re: fclean all