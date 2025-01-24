#include "../inc/Game.h"

ExtraBall::ExtraBall(Vector2 _pos, Vector2 _V) : APowerUp(_pos, _V)
{}

ExtraBall::~ExtraBall(){}

void ExtraBall::Boost(std::vector<Ball> &v)
{
	v.push_back(Ball((this->pos), Vector2 {(float)(GetRandomValue(-20, 20) / 10), (float)GetRandomValue(-5, -20) / 10}));
}

void ExtraBall::Draw()
{
	DrawCircle(this->pos.x, this->pos.y, this->radius, ORANGE);
}
