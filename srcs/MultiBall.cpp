#include "../inc/Game.h"

MultiBall::MultiBall(Vector2 _pos, Vector2 _V) : APowerUp(_pos, _V)
{}

MultiBall::~MultiBall(){}

void MultiBall::Boost(std::vector<Ball> &v)
{
	unsigned long size = v.size(); //evita que haya un bucle infinito
	for (unsigned long i = 0; i < size; ++i)
	{	
		v.push_back(Ball((v[i].getPos()), Vector2 {getRandom(-0.2, 0.2), getRandom(-0.2, 0.2)}));
		v.push_back(Ball((v[i].getPos()), Vector2 {getRandom(-0.2, 0.2), getRandom(-0.2, 0.2)}));
	}
	
}

void MultiBall::Draw()
{
	DrawCircle(this->pos.x, this->pos.y, this->radius, GREEN);
}
