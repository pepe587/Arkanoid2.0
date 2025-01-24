#pragma once
#include "Game.h"

class Ball;

class MultiBall : public APowerUp
{
	public:
		MultiBall(Vector2 _pos, Vector2 _V);
		~MultiBall();
		void Draw();
		void Boost(std::vector<Ball> &v);
};


