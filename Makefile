NAME=game
FLAGS=-Wall -Werror -Wextra
SRCS=srcs/main.cpp srcs/Rectangle.cpp srcs/Ball.cpp
OBJS=$(SRCS:.cpp=.o)
RAYLIB=raylib/raylib/libraylib.a

all: $(NAME)

$(RAYLIB):
	@cmake raylib/ -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=OFF
	@make -C raylib/ install

%.o: %.cpp
	@c++ $(FLAGS) -c $< -o $@

$(NAME):$(OBJS)
	@c++ $(FLAGS) $(OBJS) $(RAYLIB) -lm -o $(NAME)

clean:
	@rm -fr $(OBJS)

fclean: clean
	@rm -fr $(NAME)

re: fclean all