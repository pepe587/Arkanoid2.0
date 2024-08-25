NAME=game
FLAGS=-Wall -Werror -Wextra -O3 #-g -fsanitize=address
SRCS=srcs/main.cpp srcs/Rectangle.cpp srcs/Ball.cpp srcs/APowerUp.cpp srcs/ExtraBall.cpp \
	srcs/MultiBall.cpp srcs/Brick.cpp srcs/utils.cpp srcs/Button.cpp srcs/Input.cpp
OBJS=$(SRCS:.cpp=.o)
RAYLIB=raylib/raylib/libraylib.a

all: $(NAME)

$(RAYLIB):
	@(cd raylib/ \
	&& cmake . -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=OFF \
	&& make) || echo -e "\033[31mError with the dependencies, try executing make install\033[0m"

%.o: %.cpp
	@c++ $(FLAGS) -c $< -o $@

$(NAME):$(OBJS) $(RAYLIB)
	@c++ $(FLAGS) $(OBJS) $(RAYLIB) -lm -o $(NAME)

clean:
	@rm -fr $(OBJS)

fclean: clean
	@rm -fr $(NAME)

flclean: flclean
	@rm -fr $(RAYLIB)

#install dependencies
install:
	@sudo apt update
	@sudo apt install -y \
    build-essential \
    cmake \
    git \
    libsdl2-dev \
    libglfw3-dev \
    libopenal-dev \
    libfreetype6-dev \
    libjpeg-dev \
    libpng-dev \
    libtiff-dev \
    libz-dev \
    libx11-dev \
    libxrandr-dev \
    libxi-dev \
    libxxf86vm-dev \
    libxss-dev \
    libasound2-dev \
    libsndfile1-dev \
    libogg-dev \
    libvorbis-dev \
    libflac-dev \
    libmpg123-dev
	$(MAKE) all


re: fclean all

fre: flclean all