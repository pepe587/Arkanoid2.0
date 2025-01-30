#include "../inc/Game.h"

C_Rectangle::C_Rectangle()
{
	this->height = (GetMonitorHeight(0) * 0.02);
	this->y = (GetMonitorHeight(0) * 0.75);
	this->width = (GetMonitorWidth(0) * 0.125);
	this->x = (GetMonitorWidth(0) / 2) - (this->width / 2);
	MonitorProps.x = GetMonitorWidth(0);
	MonitorProps.y = GetMonitorHeight(0);
}

C_Rectangle::~C_Rectangle()
{
}

void C_Rectangle::move(int n)
{
	if ((this->x <= MonitorProps.x * 0.25 && n < 0)
		|| (this->x >= MonitorProps.x * 0.75 - this->width && n > 0))
		return ;
	this->x += n;
}

void C_Rectangle::draw()
{
	DrawRectangleRounded(Rectangle {(float)this->x, (float)this->y, (float)this->width, (float)this->height}, 1, 10, BLACK);
}

Vector2 C_Rectangle::getPos() const
{
	return Vector2 {static_cast<float>(this->x), static_cast<float>(this->y)};
}

Vector2 C_Rectangle::getSize() const
{
	return Vector2 {static_cast<float>(this->width), static_cast<float>(this->height)};
}