#include "../inc/Game.h"

Ball::Ball(Vector2 _pos, Vector2 _V) : pos(_pos), V(_V)
{
	Limits.x = GetMonitorWidth(0) * 0.75;
	Limits.y = GetMonitorHeight(0);
}

void Ball::draw()
{
	DrawCircle(this->pos.x, this->pos.y, Limits.x / 200, RED);
}
void Ball::updatePos()
{
	this->pos.x += this->V.x;
	this->pos.y += this->V.y;
}

Ball::~Ball()
{
}

bool Ball::checkCollition(C_Rectangle &R, Sound &sound)
{
	if (pos.x >= Limits.x || pos.x <= GetMonitorWidth(0) * 0.25)
	{
		PlaySound(sound);
		V.x *= -1;
	}
	else if (pos.y >= Limits.y || pos.y <= 0)
	{
		PlaySound(sound);
		V.y *= -1;
	}
	if ((int)this->pos.y == (int)R.getPos().y - (int)R.getSize().y / 2 &&
		(this->pos.x >= (R.getPos().x ) && this->pos.x <= (R.getPos().x + R.getSize().x)))
	{
		PlaySound(sound);
		V.y *= -1;
	}
	if (this->pos.y > R.getPos().y * 1.1)
		return false;
	return true;
}

Vector2 Ball::getVector() const
{
	return this->V;
}

Vector2 Ball::getPos() const
{
	return this->pos;
}
