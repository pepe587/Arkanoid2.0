#include "../inc/Game.h"

Ball::Ball(Vector2 _pos, Vector2 _V) : pos(_pos), V(_V)
{
	Limits.x = GetMonitorWidth(0) * 0.75;
	Limits.y = GetMonitorHeight(0);
	MonitorProps.x = GetMonitorWidth(0);
	MonitorProps.y = GetMonitorHeight(0);
}

void Ball::Draw()
{
	DrawCircle(this->pos.x, this->pos.y, Limits.x / 200, RED);
}
void Ball::updatePos()
{
	this->pos.x += this->V.x;
	this->pos.y += this->V.y;
}

Ball::~Ball(){}

bool Ball::checkCollision(C_Rectangle &R, Sound &sound, std::vector<Brick> &Bricks, int &b, std::vector<APowerUp *> &PowerUps, t_movables *movables)
{
	pthread_mutex_lock(&movables->m_bricks);
	for (size_t i = 0; i < Bricks.size(); ++i)
	{
		if (CheckCollisionCircleRec(this->pos, this->Limits.x / 200, Rectangle {(float)Bricks[i].getPos().x, (float)Bricks[i].getPos().y, (float)Bricks[i].getSize().x, (float)Bricks[i].getSize().y}))
		{
			float diffX = this->pos.x - fmax(Bricks[i].getPos().x, fmin(this->pos.x, Bricks[i].getPos().x + Bricks[i].getSize().x));;
			float diffY = this->pos.y - fmax(Bricks[i].getPos().y, fmin(this->pos.y, Bricks[i].getPos().y + Bricks[i].getSize().y));;
			if (fabs(diffY) > fabs(diffX))
				V.y *= -1;
			else
				V.x *= -1;
			PlaySound(sound);
			if (!Bricks[i].Hit())
			{
				Bricks.erase(Bricks.begin() + i);
				++b;
				if (GetRandomValue(1, 10) == 5)
				{
					pthread_mutex_lock(&movables->m_powerups);
					PowerUps.push_back(new ExtraBall(Vector2 {Bricks[i].getPos().x + Bricks[i].getSize().x / 2, Bricks[i].getPos().y + Bricks[i].getSize().y / 2}, Vector2 {0, 0.2}));
					pthread_mutex_unlock(&movables->m_powerups);

				}
				else if (GetRandomValue(1, 30) == 5)
				{
					pthread_mutex_lock(&movables->m_powerups);
					PowerUps.push_back(new MultiBall(Vector2 {Bricks[i].getPos().x + Bricks[i].getSize().x / 2, Bricks[i].getPos().y + Bricks[i].getSize().y / 2}, Vector2 {0, 0.2}));
					pthread_mutex_unlock(&movables->m_powerups);
				}
			}
			pthread_mutex_unlock(&movables->m_bricks);
			return true;
		}
	}
	pthread_mutex_unlock(&movables->m_bricks);
	if (pos.x >= Limits.x || pos.x <= MonitorProps.x * 0.25)
	{
		PlaySound(sound);
		V.x *= -1;
	}
	else if (pos.y >= Limits.y || pos.y <= 0)
	{
		PlaySound(sound);
		V.y *= -1;
	}
	pthread_mutex_lock(&movables->m_rectangle);
	if ((this->pos.y >= R.getPos().y - (int)R.getSize().y / 2 && this->pos.y <= R.getPos().y) &&
		(this->pos.x >= (R.getPos().x ) && this->pos.x <= (R.getPos().x + R.getSize().x)))
	{
		PlaySound(sound);
		V.y *= -1;
		V.x = ((this->pos.x - (R.getPos().x + (R.getSize().x / 2))) / (R.getSize().x / 2)) / 10;
	}
	if (this->pos.y > R.getPos().y * 1.1) {
		pthread_mutex_unlock(&movables->m_rectangle); return false; }
	pthread_mutex_unlock(&movables->m_rectangle);
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
