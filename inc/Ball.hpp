#pragma once
#include "Game.h"
#include <iostream>
#include <vector>
class C_Rectangle;
class Brick;
class APowerUp;

class Ball
{
	private:
		Vector2 pos;
		Vector2 V;
		Vector2 Limits;
		Vector2 MonitorProps;
	public:
		Ball(Vector2 pos, Vector2 V);
		void Draw();
		void updatePos();
		bool checkCollision(C_Rectangle &R, Sound &sound, std::vector<Brick> &Brick, int &b, std::vector<APowerUp *> &PowerUps);
		Vector2 getVector() const;
		Vector2 getPos() const;
		~Ball();
};

