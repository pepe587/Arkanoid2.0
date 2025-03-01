#pragma once
#include "Game.h"

class Input
{
private:
	std::string input;
	Vector2 pos;
	Vector2 size;
public:
	Input(Vector2 _pos, Vector2 _size);
	~Input();
	void Draw(Color c);
	void listenKeys();
	std::string getValue() const;
};



