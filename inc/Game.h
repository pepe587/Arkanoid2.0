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

float	getRandom(float min, float max);
void	free_and_close(t_movables *movables, pthread_t *thread);
bool	checkExtension(std::string filename);
bool	thereAreUtil(t_movables *movables);
int		GetFps(double &time, int &fps_counter);
void	thread_sleep(struct timeval &start, struct timeval &end);