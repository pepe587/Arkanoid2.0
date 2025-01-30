#pragma once

#include "../raylib/src/raylib.h"
#include "Ball.hpp"
#include "Rectangle.hpp"
#include "Brick.hpp"
#include "APowerUp.hpp"
#include "ExtraBall.hpp"
#include "MultiBall.hpp"
#include "GLFW/glfw3.h" 
#include <iostream>
#include <vector>
#include <unistd.h>
#include <pthread.h>

typedef struct s_movables
{
    std::vector<Ball> &Balls;
    C_Rectangle &R;
    std::vector<Brick> &Bricks;
    std::vector<APowerUp *> &PowerUps;
    int &BricksBreaks;
    pthread_mutex_t &m_balls;
    pthread_mutex_t &m_rectangle;
    pthread_mutex_t &m_bricks;
    pthread_mutex_t &m_powerups;
} t_movables;