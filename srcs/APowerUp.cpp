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

bool APowerUp::checkCollision(C_Rectangle &R, Sound &sound, std::vector<Ball> &v, t_movables *movables)
{
	pthread_mutex_lock(&movables->m_rectangle);
	if ((int)this->pos.y == (int)R.getPos().y - (int)R.getSize().y / 2 &&
		(this->pos.x >= (R.getPos().x ) && this->pos.x <= (R.getPos().x + R.getSize().x)))
	{
		PlaySound(sound);
		pthread_mutex_lock(&movables->m_balls);
		this->Boost(v);
		pthread_mutex_unlock(&movables->m_balls);
		V.y *= -1;
		pthread_mutex_unlock(&movables->m_rectangle);
		return false;
	}
	if (this->pos.y > R.getPos().y * 1.1)
	{
		pthread_mutex_unlock(&movables->m_rectangle);
		return false;
	}
	pthread_mutex_unlock(&movables->m_rectangle);
	return true;
}