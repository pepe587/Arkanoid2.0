#include "../inc/Game.h"

Input::Input(Vector2 _pos, Vector2 _size) : pos(_pos), size(_size)
{}

Input::~Input() {}

void Input::Draw(Color c) {
    // 🎨 Definir colores estilo pixel art
    (void)c;
	Color border = { 255, 107, 107, 255 };        // Rojo neón
    Color shadow = { 20, 20, 20, 255 };           // Sombra oscura
    Color highlight = { 255, 207, 107, 255 };     // Bisel claro (amarillento suave)
    Color darkShade = { 60, 60, 60, 255 };        // Bisel oscuro (gris oscuro)

    // 🟫 Sombra del input (Desplazada ligeramente hacia abajo)
    Rectangle shadowRect = { pos.x + 4, pos.y + 4, size.x, size.y };
    DrawRectangleRec(shadowRect, shadow);

    // 🔺 Dibujar bisel superior (Más claro)
    DrawRectangle(pos.x, pos.y, size.x, 4, highlight); // Línea superior

    // 🔻 Dibujar bisel inferior (Más oscuro)
    DrawRectangle(pos.x, pos.y + size.y - 4, size.x, 4, darkShade); // Línea inferior

    // 🟥 Dibujar borde rojo alrededor del input
    DrawRectangleLinesEx({ pos.x, pos.y, size.x, size.y }, 2, border);

    // 🟦 Dibujar relleno del input
    Rectangle innerRect = { pos.x + 2, pos.y + 2, size.x - 4, size.y - 4 };
    DrawRectangleRec(innerRect, WHITE);

    // ✍️ Dibujar el texto centrado
    if (!input.empty()) {
        int textWidth = MeasureText(input.c_str(), 20);
        int textX = pos.x + (size.x / 2) - (textWidth / 2);
        int textY = pos.y + (size.y / 2) - 10; // Centrado vertical
        DrawText(input.c_str(), textX, textY, 20, BLACK);
    }
}


bool mayus()
{
	if (IsKeyDown(((KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)) && !IsKeyDown(KEY_CAPS_LOCK))
		|| IsKeyDown(KEY_CAPS_LOCK))
		return true;
	return false;
}

void fillDictionaries(std::vector<std::pair<int, int> > &normalDict, std::vector<std::pair<int, int> > &mayusDict)
{
	normalDict.push_back(std::make_pair(KEY_SPACE, 32));
	mayusDict.push_back(std::make_pair(KEY_SPACE, 32));
	for (int i = 'A'; i <= 'Z'; ++i)
	{
		normalDict.push_back(std::make_pair(i, i + 32));
		mayusDict.push_back(std::make_pair(i, i));
	}
		
	for (int i = '0'; i <= '9'; ++i)
	{
		normalDict.push_back(std::make_pair(i, i));
		mayusDict.push_back(std::make_pair(i - 15, i - 15));		
	}
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
	if (IsKeyPressed(KEY_BACKSPACE) && input.size())
		input.pop_back();
}

std::string Input::getValue() const
{
	return this->input;
}