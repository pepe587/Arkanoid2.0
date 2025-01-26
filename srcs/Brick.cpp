#include "../inc/Game.h"

Brick::Brick(short _type, Vector2 _pos, Vector2 _size) : type(_type), pos(_pos), size(_size)
{
	if (type == SIMPLE)
	{
		hits = 1;
		color = GREEN;
	}
	else if (type == RESISTANT)
	{
		hits = 3;
		color = RED;
	}
	else
	{
		hits = -1;
		color = GRAY;
	}
}

Brick::~Brick(){}

void Brick::Draw()
{
	DrawRectangle(pos.x, pos.y, size.x, size.y, color);
	DrawRectangleLines(pos.x, pos.y, size.x, size.y, BLACK);
}

bool Brick::Hit()
{
	hits--;
	if (hits)
		return true;
	return false;
}

bool Brick::ImBreakable()
{
	if (type > 1)
		return true;
	return false;
}

Vector2 Brick::getPos() const
{
	return this->pos;
}

Vector2 Brick::getSize() const
{
	return this->size;
}