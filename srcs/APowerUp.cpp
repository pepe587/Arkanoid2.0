#include "../inc/Game.h"

APowerUp::APowerUp(Vector2 _pos, Vector2 _V) : pos(_pos), V(_V)
{
	this->radius = GetMonitorWidth(0) / 150;
	Limits.x = GetMonitorWidth(0) * 0.75;
	Limits.y = GetMonitorHeight(0);
}

APowerUp::~APowerUp() {}

void APowerUp::updatePos()
{
	this->pos.x += this->V.x;
	this->pos.y += this->V.y;
}

bool APowerUp::checkCollition(C_Rectangle &R, Sound &sound, std::vector<Ball> &v)
{
	if ((int)this->pos.y == (int)R.getPos().y - (int)R.getSize().y / 2 &&
		(this->pos.x >= (R.getPos().x ) && this->pos.x <= (R.getPos().x + R.getSize().x)))
	{
		PlaySound(sound);
		this->Boost(v);
		V.y *= -1;
		return false;
	}
	if (this->pos.y > R.getPos().y * 1.1)
		return false;
	return true;
}