#pragma once
#include "Game.h"

class Button
{
private:
	Vector2 pos;
	Vector2	size;
	Rectangle button;
	std::string text;
	Color ctext;
public:
	Button(Vector2 _pos, Vector2 _size, std::string _text, Color _ctext);
	Button(Vector2 _pos, Vector2 _size);
	void Draw(Color noMouse, Color wMouse);
	bool isClicked();
	~Button();
};

