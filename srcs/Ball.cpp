#include "../inc/Game.h"

Ball::Ball(int _x, int _y, Vector2 _V) : x(_x), y(_y), V(_V)
{
	Limits.x = GetMonitorWidth(0);
	Limits.y = GetMonitorHeight(0);
}

void Ball::draw()
{
	DrawCircle(this->x, this->y, 7.5, RED);
}
void Ball::updatePos()
{
	this->x += this->V.x;
	this->y += this->V.y;
}

Ball::~Ball()
{
}

bool Ball::checkCollition(C_Rectangle &R)
{
	if (x >= Limits.x || x <= 0)
		V.x *= -1;
	else if (y >= Limits.y || y <= 0)
		V.y *= -1;
	if ((int)this->y == (int)R.getPos().y - (int)R.getSize().y / 2 &&
		(this->x >= (R.getPos().x ) && this->x <= (R.getPos().x + R.getSize().x)))
	{
		V.y *= -1;
	}
	if (this->y > R.getPos().y * 1.1)
		return false;
	return true;
}


