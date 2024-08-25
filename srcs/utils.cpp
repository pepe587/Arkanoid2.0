#include "../inc/Game.h"

float	getRandom(float min, float max)
{
	float rand = 0;

	while (rand == 0)
		rand = (float)GetRandomValue((int)(min * 10), (int)(max * 10)) / 10;
	return rand;
}

void free_and_close(t_movables *movables, pthread_t *thread, Texture2D b)
{
	pthread_cancel(*thread);
	CloseAudioDevice();
	pthread_mutex_lock(&movables->m_balls);
	for (unsigned long i = 0; i < movables->PowerUps.size(); ++i)
		delete movables->PowerUps[i];
	pthread_mutex_unlock(&movables->m_balls);
	delete thread;
	delete movables;
	UnloadTexture(b);
}

bool checkExtension(std::string filename)
{
	size_t i = filename.size();

	if (i <= 5 || filename[i - 1] != 'a'
	|| filename[i - 2] != 't'
	|| filename[i - 3] != 'a'
	|| filename[i - 4] != 'd'
	|| filename[i - 5] != '.')
		return false;
	return true;
}

bool thereAreUtil(t_movables *movables)
{
	pthread_mutex_lock(&(movables->m_powerups));
	for (unsigned long i = 0; i < movables->PowerUps.size(); ++i)
		if (dynamic_cast<ExtraBall *>(movables->PowerUps[i]))
		{
			pthread_mutex_unlock(&(movables->m_powerups));
			return true;
		}
	pthread_mutex_unlock(&(movables->m_powerups));
	return false;
}

int GetFps(double &time, int &fps_counter)
{
	int ret;
	if (GetTime() - time >= 1)
	{
		time = GetTime();
		ret = fps_counter;
		fps_counter = 0;
		return ret;
	}
	throw 1;
	return -1;
}

void	thread_sleep(struct timeval &start, struct timeval &end)
{	
	(void)start;
	(void)end;
	//double time_sleep = 150 - ((end.tv_usec) - (start.tv_usec));
	//if (time_sleep >= 0)
	//	usleep(time_sleep);
	usleep(100);
}

bool canPutBrick(void)
{
	Vector2 pos = roundPos(GetMousePosition());

	if (pos.x < GetMonitorWidth(0) * 0.25 || pos.x >= GetMonitorWidth(0) * 0.74
		|| pos.y > GetMonitorHeight(0) * 0.5)
		return false;
	return true;
}

Vector2 roundPos(Vector2 pos)
{
	Vector2 size = {(float)GetMonitorWidth(0) / 100, (float)GetMonitorHeight(0) / 90};
	float x = GetMonitorWidth(0) * 0.25;
	float y = 0;

	while (x + size.x < pos.x)
		x += size.x;
	while (y + size.y < pos.y)
		y += size.y;
	return Vector2 {x, y};
}

void deleteByPos(std::vector<Brick> &b)
{
	Vector2 MousePos = roundPos(GetMousePosition());

	for (size_t i = 0; i < b.size(); i++)
		if (b[i].getPos().x == MousePos.x && b[i].getPos().y == MousePos.y)
		{
			b.erase(b.begin() + i);
			return ;
		}
}

bool IsTaken(std::vector<Brick> &b, Brick &newB)
{
	for (size_t i = 0; i < b.size(); i++)
		if (b[i].getPos().x == newB.getPos().x
			&& b[i].getPos().y == newB.getPos().y)
			return true;
	return false;
}

void BricksToFile(std::string filename, std::vector<Brick> &b)
{
	std::ofstream new_level("levels/" + filename + ".data");
	std::string arrlevel[50];
	Vector2 size = {(float)(GetMonitorWidth(0) / 100), (float)(GetMonitorHeight(0) / 90)};

	for (int i = 0; i <= 49; ++i)
		arrlevel[i] = std::string("000000000000000000000000000000000000000000000000000");

	for (size_t i = 3; i < b.size(); ++i)
		arrlevel[(int)(b[i].getPos().y / size.y)][(int)(b[i].getPos().x / size.x) - 26] = b[i].getType() + '0';
	for (int i = 0; i <= 49; ++i)
	{
		for (size_t x = 0; x < arrlevel[i].size(); ++x)
			new_level << arrlevel[i][x];
		if (i != 49)
			new_level << '\n';
	}

	new_level.close();
}

void DrawMenuBackground()
{
    // 🎮 Colores base para el fondo
    Color bgStart = { 20, 20, 30, 255 };   // Color de inicio (más oscuro)
    Color bgEnd = { 40, 40, 60, 255 };     // Color final del gradiente (más claro)

    // 🌌 Fondo con gradiente suave de arriba hacia abajo
    for (int i = 0; i < GetScreenHeight(); i++) {
        Color currentColor = ColorLerp(bgStart, bgEnd, (float)i / GetScreenHeight());
        DrawLine(0, i, GetScreenWidth(), i, currentColor);  // Gradiente suave
    }

    // 🔲 Líneas de rejilla sutiles (más suave que antes)
    Color gridColor = { 255, 255, 255, 40 };  // Líneas sutiles
    for (int i = 0; i < GetScreenWidth(); i += 50) {
        DrawLine(i, 0, i, GetScreenHeight(), gridColor);  // Líneas verticales
    }
    for (int i = 0; i < GetScreenHeight(); i += 50) {
        DrawLine(0, i, GetScreenWidth(), i, gridColor);  // Líneas horizontales
    }

    // 🏁 Borde simple en blanco para el menú
    Color borderColor = { 255, 255, 255, 100 };  // Borde blanco translúcido
    DrawRectangleLines(0, 0, GetScreenWidth(), GetScreenHeight(), borderColor);
}