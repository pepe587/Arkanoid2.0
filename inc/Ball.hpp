#pragma once

class C_Rectangle;

class Ball
{
private:
	float x;
	float y;
	Vector2 V;
	Vector2 Limits;
public:
	Ball(int _x, int _y, Vector2 V);
	void draw();
	void updatePos();
	bool checkCollition(C_Rectangle &R);
	~Ball();
};

