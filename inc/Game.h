#pragma once

#include "../raylib/src/raylib.h"
#include "Ball.hpp"
#include "Rectangle.hpp"
#include "Brick.hpp"
#include "APowerUp.hpp"
#include "ExtraBall.hpp"
#include "MultiBall.hpp"
#include "GLFW/glfw3.h"
#include "Button.hpp" 
#include "Input.hpp"
#include <iostream>
#include <vector>
#include <unistd.h>
#include <pthread.h>
#include <math.h>
#include <fstream>
#include <sys/time.h>
#include <sys/types.h>
#include <dirent.h>
#include <utility>
#include <string.h>

typedef struct s_movables
{
    std::vector<Ball> Balls;
    C_Rectangle R;
    std::vector<Brick> Bricks;
    std::vector<APowerUp *> PowerUps;
    int BricksBreaks;
    pthread_mutex_t m_balls;
    pthread_mutex_t m_rectangle;
    pthread_mutex_t m_bricks;
    pthread_mutex_t m_powerups;
} t_movables;

typedef struct s_settings
{
	unsigned int	maxfps;
	unsigned short	volume;
} t_settings;

typedef struct s_quadrant
{
	Rectangle r;
	std::vector<Ball> Balls;
	std::vector<Brick> Bricks;
} t_quadrant;


float	getRandom(float min, float max);
void	free_and_close(t_movables *movables, pthread_t *thread, Texture2D b);
bool	checkExtension(std::string filename);
bool	thereAreUtil(t_movables *movables);
int		GetFps(double &time, int &fps_counter);
void	thread_sleep(struct timeval &start, struct timeval &end);
bool	canPutBrick(void);
Vector2 roundPos(Vector2 pos);
void	deleteByPos(std::vector<Brick> &b);
bool	IsTaken(std::vector<Brick> &b, Brick &newB);
void	BricksToFile(std::string filename, std::vector<Brick> &b);
void	DrawMenuBackground();