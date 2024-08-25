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
    Vector2 mousePos = GetMousePosition();
    bool isHovered = CheckCollisionPointRec(mousePos, button);

    // 🎨 Definir colores estilo pixel art
    Color border = { 255, 107, 107, 255 };        // Rojo neón
    Color shadow = { 20, 20, 20, 255 };           // Sombra oscura
    Color highlight = { 255, 207, 107, 255 };     // Bisel claro (amarillento suave)
    Color darkShade = { 60, 60, 60, 255 };        // Bisel oscuro (gris oscuro)
    Color fillColor = isHovered ? wMouse : noMouse;

    // 🟫 Sombra del botón (Desplazada ligeramente hacia abajo)
    Rectangle shadowRect = { button.x + 4, button.y + 4, button.width, button.height };
    DrawRectangleRec(shadowRect, shadow);

    // 🔺 Dibujar bisel superior (Más claro)
    DrawRectangle(button.x, button.y, button.width, 4, highlight); // Línea superior

    // 🔻 Dibujar bisel inferior (Más oscuro)
    DrawRectangle(button.x, button.y + button.height - 4, button.width, 4, darkShade); // Línea inferior

    // 🟥 Dibujar borde rojo alrededor del botón
    DrawRectangleLinesEx(button, 2, border);

    // 🟦 Dibujar relleno del botón
    Rectangle innerRect = { button.x + 2, button.y + 2, button.width - 4, button.height - 4 };
    DrawRectangleRec(innerRect, fillColor);

    // ✍️ Dibujar el texto centrado
    if (!text.empty()) {
        int textWidth = MeasureText(text.c_str(), 14);
        int textX = button.x + (button.width / 2) - (textWidth / 2);
        int textY = button.y + (button.height / 2) - 7; // Centrado vertical
        DrawText(text.c_str(), textX, textY, 14, WHITE);
    }
}


bool Button::isClicked()
{
	if (IsMouseButtonDown(0) && CheckCollisionPointRec(GetMousePosition(), button))
		return true;
	else
		return false;	
}

Button::~Button(){}