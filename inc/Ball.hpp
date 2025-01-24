#pragma once
#include "Game.h"
class C_Rectangle;

class Ball
{
	private:
		Vector2 pos;
		Vector2 V;
		Vector2 Limits;
	public:
		Ball(Vector2 pos, Vector2 V);
		void draw();
		void updatePos();
		bool checkCollition(C_Rectangle &R, Sound &sound);
		Vector2 getVector() const;
		Vector2 getPos() const;
		~Ball();
};

