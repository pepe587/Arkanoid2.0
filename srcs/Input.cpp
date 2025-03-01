#include "../inc/Game.h"

Input::Input(Vector2 _pos, Vector2 _size) : pos(_pos), size(_size)
{}

Input::~Input() {}

void Input::Draw(Color c)
{
	DrawRectangleV(pos, size, c);
	DrawRectangle(pos.x + size.x * 0.15, pos.y + size.y * 0.5, size.x * 0.7, size.y * 0.25, WHITE);
	DrawText(input.c_str(), pos.x + size.x * 0.15, pos.y + size.y * 0.5, 20, BLACK);
}

bool mayus()
{
	if (IsKeyDown(((KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)) && !IsKeyDown(KEY_CAPS_LOCK))
		|| IsKeyDown(KEY_CAPS_LOCK))
		return true;
	return false;
}

void fillDictionaries(std::vector<std::pair<int, int> > &normalDict, std::vector<std::pair<int, int> > &mayusDict);
{
	normalDict.push_back(std::make_pair(KEY_SPACE, 32));
	for (int i = 'A'; i <= 'Z'; ++i)
		normalDict.push_back(std::make_pair(i, i));
	for (int i = '0'; i <= '9'; ++i)
		normalDict.push_back(std::make_pair(i, i));
}

void Input::listenKeys()
{
	static std::vector<std::pair<int, int> > normalDict;
	static std::vector<std::pair<int, int> > mayusDict;

	if (!normalDict.size())
		fillDictionaries(normalDict, mayusDict);
	for (size_t i = 0; i < normalDict.size(); ++i)
	{
		if (IsKeyPressed(normalDict[i].first))
		{
			if (mayus())
				input.push_back(mayusDict[i].second);
			else
				input.push_back(normalDict[i].second);
		}
	}
}

std::string Input::getValue() const
{
	return this->input;
}