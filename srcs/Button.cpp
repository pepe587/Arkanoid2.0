#include "../inc/Game.h"

Button::Button(Vector2 _pos, Vector2 _size, std::string _text, Color _ctext) : pos(_pos), size(_size), text(_text), ctext(_ctext)
{
	button = {pos.x, pos.y, size.x, size.y};
}
Button::Button(Vector2 _pos, Vector2 _size) : pos(_pos), size(_size)
{
	button = {pos.x, pos.y, size.x, size.y};
}

void Button::Draw(Color noMouse, Color wMouse)
{
	if (CheckCollisionPointRec(GetMousePosition(), button))
		DrawRectangleRec(button, wMouse);
	else
		DrawRectangleRec(button, noMouse);
	if (!text.empty())
		DrawText(text.c_str(), pos.x + size.x * 0.10, pos.y + size.y * 0.5, 2, ctext);
}
bool Button::isClicked()
{
	if (IsMouseButtonDown(0) && CheckCollisionPointRec(GetMousePosition(), button))
		return true;
	else
		return false;	
}

Button::~Button(){}