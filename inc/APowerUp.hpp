#pragma once
#include "Game.h"
#include <vector>

class Ball;

class APowerUp
{
	protected:
		Vector2 pos;
		Vector2 V;
		double radius;
		Vector2 Limits;
	public:
		APowerUp(Vector2 _pos, Vector2 _V);
		virtual ~APowerUp();
		virtual void Boost(std::vector<Ball> &v) = 0;
		virtual void Draw() = 0;
		void updatePos();
		bool checkCollision(C_Rectangle &R, Sound &sound, std::vector<Ball> &v);
};


