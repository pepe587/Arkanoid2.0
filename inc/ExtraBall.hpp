#pragma once
#include "Game.h"
#include <vector>

class Ball;

class ExtraBall : public APowerUp
{
	public:
		ExtraBall(Vector2 _pos, Vector2 _V);
		~ExtraBall();
		void Draw();
		void Boost(std::vector<Ball> &v);
};

