#include "../inc/Game.h"

/*To do List:
	-Add all the Sounds
	-Add more Power Ups + (IDEA) add power-downs
	-Fix the bug of the collision (if the collision is with the x axis the ball goes to a bad direction)
	-Find solution to the problem of FPS (if FPS > speed >) IMPORTANT (raycasting can be a solution)
	-Fix the bug of the percent
	-Change the method of the install the dependencies (idk if this its a good idea), search for AppImage for linux and a .exe installer for windows
	-Improve FPS
	-Change all the geeting of the monitor size to a method
*/

void *refresh_movables(void *_movables)
{
	t_movables movables = *(t_movables *)_movables;

	InitAudioDevice();
	Sound sound = LoadSound("sounds/ball-bounce.wav");
	/*Rectangle movement*/
	while (!WindowShouldClose())
	{
		if (IsKeyDown(KEY_RIGHT))
		{
			pthread_mutex_lock(&(movables.m_rectangle));
			movables.R.move(2);
			pthread_mutex_unlock(&(movables.m_rectangle));
		}
		if (IsKeyDown(KEY_LEFT))
		{
			pthread_mutex_lock(&(movables.m_rectangle));
			movables.R.move(-2);
			pthread_mutex_unlock(&(movables.m_rectangle));
		}
		/*Ball movement & check collision*/
		pthread_mutex_lock(&(movables.m_balls));
		for (unsigned long i = 0; i < movables.Balls.size(); ++i)
		{
			movables.Balls[i].updatePos();
			if (!movables.Balls[i].checkCollision(movables.R, sound, movables.Bricks, movables.BricksBreaks, movables.PowerUps))
			{
				movables.Balls.erase(movables.Balls.begin() + i);
				--i;
			}
		}
		pthread_mutex_unlock(&(movables.m_balls));
		/*Power Ups movement & check collision + Boost*/
		pthread_mutex_lock(&(movables.m_powerups));
		for (unsigned long i = 0; i < movables.PowerUps.size(); ++i)
		{
			movables.PowerUps[i]->updatePos();
			if (!movables.PowerUps[i]->checkCollision(movables.R, sound, movables.Balls))
			{
				delete movables.PowerUps[i];
				movables.PowerUps.erase(movables.PowerUps.begin() + i);
				--i;
			}
		}
		pthread_mutex_unlock(&(movables.m_powerups));
	}
	return (NULL);
}

bool thereAreUtil(std::vector<APowerUp *> &v, t_movables &movables)
{
	pthread_mutex_lock(&(movables.m_powerups));
	for (unsigned long i = 0; i < v.size(); ++i)
		if (dynamic_cast<ExtraBall *>(v[i]))
		{
			pthread_mutex_unlock(&(movables.m_powerups));
			return true;
		}
	pthread_mutex_unlock(&(movables.m_powerups));
	return false;
}

int brickeableBricks(std::vector<Brick> &bricks)
{
	int i = 0;

	for (unsigned long z = 0; z < bricks.size(); z++)
		if (bricks[z].ImBreakable())
			++i;
	return i;
}

std::vector<Brick> intToBrick()
{
	std::string map[50] = {
		"00000000000000000000000000000000000000000000000000", // Línea 1
		"00111111110000000000000000000000001111111100000000", // Línea 2
		"00222222220000000011111111110000002222222200000000", // Línea 3
		"00333333330000001122222222221000003333333300000000", // Línea 4
		"00000000000000003333333333333000000000000000000000", // Línea 5
		"00100000000010000000000000000000000000000100000010", // Línea 6
		"00111111111111110000000000000001111111111111110000", // Línea 7
		"00200000000000220000000000000022000000000000220000", // Línea 8
		"00333333333333333300000000003333333333333333330000", // Línea 9
		"00000000000000000000000000000000000000000000000000", // Línea 10
		"00112233113322110000000000000011223311332211000000", // Línea 11
		"00112233113322111111111111111111223311332211000000", // Línea 12
		"00200000000000222222222222222222200000000000220000", // Línea 13
		"00333333333333333333333333333333333333333333330000", // Línea 14
		"00000000000000000000000000000000000000000000000000", // Línea 15
		"00001111222233330000000000000003333222211110000000", // Línea 16
		"00000000000000001111000000001111000000000000000000", // Línea 17
		"00000000000000000022000000220000000000000000000000", // Línea 18
		"00000000000000000000033333000000000000000000000000", // Línea 19
		"00000000000000000000000000000000000000000000000000", // Línea 20
		"00001111110000000000000000000000000000111111000000", // Línea 21
		"00002222220000000000000000000000000000222222000000", // Línea 22
		"00003333330000000000000000000000000000333333000000", // Línea 23
		"00000000000000000000000000000000000000000000000000", // Línea 24
		"00001111222233331111111111111111333322221111000000", // Línea 25
		"00000000000000001111222233332222111100000000000000", // Línea 26
		"00000000000000000000000000000000000000000000000000", // Línea 27
		"00001111222200000000000000000000222211110000000000", // Línea 28
		"00000000000000000000000000000000000000000000000000", // Línea 29
		"00111111111111111111111111111111111111111111110000", // Línea 30
		"00222222222222222222222222222222222222222222220000", // Línea 31
		"00333333333333333333333333333333333333333333330000", // Línea 32
		"00000000000000000000000000000000000000000000000000", // Línea 33
		"00001111222233331111111111111111333322221111000000", // Línea 34
		"00001111222200000000000000000000222211110000000000", // Línea 35
		"00000000000000000000000000000000000000000000000000", // Línea 36
		"00000000000000001111111111111111000000000000000000", // Línea 37
		"00000000000000000000000000000000000000000000000000", // Línea 38
		"00001122331111113322110000001122331111113322110000", // Línea 39
		"00001111222233331111111111111111333322221111000000", // Línea 40
		"00000000000000000000000000000000000000000000000000", // Línea 41
		"00111111111111111111111111111111111111111111110000", // Línea 42
		"00222222222222222222222222222222222222222222220000", // Línea 43
		"00333333333333333333333333333333333333333333330000", // Línea 44
		"00000000000000000000000000000000000000000000000000", // Línea 45
		"00001111222233331111111111111111333322221111000000", // Línea 46
		"00000000000000000000000000000000000000000000000000", // Línea 47
		"00111111111111111111111111111111111111111111110000", // Línea 48
		"00000000000000000000000000000000000000000000000000", // Línea 49
		"00003333333333333333333333333333333333333333330000", // Línea 50
	};

	std::vector<Brick> Bricks;
	Vector2 size;
	size.x = GetMonitorWidth(0) / 100;
	size.y = GetMonitorHeight(0) / 90;
	for (unsigned long x = 0, y = 0; y < 50 && x < 50; ++y)
	{
		while (x < map[y].size())
		{
			if (map[y][x] != '0')
				Bricks.push_back(Brick(map[y][x] - '0', Vector2 {(float)((x * size.x) + (GetMonitorWidth(0) * 0.25)), (float)(y * size.y)}, size));
			++x;
		}
		x = 0;
	}
	return Bricks;
}

int main(void)
{
	std::vector<Ball> Balls;
	std::vector<APowerUp *> PowerUps;
	InitWindow(1, 1, "Game");
	SetWindowSize(GetMonitorWidth(0), GetMonitorHeight(0));
	ToggleFullscreen();
	std::vector<Brick> Bricks = intToBrick();
	int	BricksBreakables = brickeableBricks(Bricks);
	int BricksBreaks = 0;

	/*TEST FOR POWERUPS*/
	//PowerUps.push_back(new ExtraBall(Vector2 {(float)GetMonitorWidth(0) / 2, (float)GetMonitorHeight(0) / 3}, Vector2 {0, 0.5}));
	//PowerUps.push_back(new MultiBall(Vector2 {(float)GetMonitorWidth(0) / 2, 0}, Vector2 {0, 0.5}));
	/*END OF THE TEST*/

	/*First Ball of the Game (HARDCODED VALUES)*/
	Balls.push_back(Ball(Vector2 {(float)GetMonitorWidth(0) / 2, (float)(GetMonitorHeight(0) / 1.5)}, Vector2 {0.5, 1}));
	
	C_Rectangle R;
	double time = 0;
	int fps_counter = 0;
	int actual_fps = 0;
	pthread_mutex_t mutexes[4];
	pthread_mutex_init(&(mutexes[0]), NULL);
	pthread_mutex_init(&(mutexes[1]), NULL);
	pthread_mutex_init(&(mutexes[2]), NULL);
	pthread_mutex_init(&(mutexes[3]), NULL);
	t_movables movables = {Balls, R, Bricks, PowerUps, BricksBreaks, mutexes[0], mutexes[1], mutexes[2], mutexes[3]};
	pthread_t *thread = new pthread_t;
	pthread_create(thread, NULL, refresh_movables, &movables);
	/*Render Thread*/
	while (!WindowShouldClose())
	{
		/*Calculate FPS*/
		if (GetTime() - time >= 1)
		{
			actual_fps = fps_counter;
			time = GetTime();
			fps_counter = 0;
		}
		/*No balls = No party*/
		pthread_mutex_lock(&movables.m_balls);
		if (!Balls.size() && !thereAreUtil(PowerUps, movables))
		{
			break ;
			pthread_mutex_unlock(&movables.m_balls);
		}
		pthread_mutex_unlock(&movables.m_balls);

		/*Draw Scope*/
		BeginDrawing();
		ClearBackground(BLACK);
		DrawText(("FPS: " + std::to_string(actual_fps)).c_str(), 10, 10, 50, WHITE);
		DrawText(((std::to_string((((float)BricksBreaks / (float)BricksBreakables)) * 100) + " %").c_str()), 10, 100, 50, WHITE);
		DrawRectangle(GetMonitorWidth(0) * 0.25, 0, GetMonitorWidth(0) * 0.5, GetMonitorHeight(0), BLUE);
		
		pthread_mutex_lock(&movables.m_rectangle);
		R.draw();
		pthread_mutex_unlock(&movables.m_rectangle);

		pthread_mutex_lock(&movables.m_bricks);
		for (unsigned long i = 0; i < Bricks.size(); ++i)
			Bricks[i].Draw();
		pthread_mutex_unlock(&movables.m_bricks);

		pthread_mutex_lock(&movables.m_balls);
		for (unsigned long i = 0; i < Balls.size(); ++i)
			Balls[i].Draw();
		pthread_mutex_unlock(&movables.m_balls);
		
		pthread_mutex_lock(&movables.m_powerups);
		for (unsigned long i = 0; i < PowerUps.size(); ++i)
			PowerUps[i]->Draw();
		pthread_mutex_unlock(&movables.m_powerups);
        
		EndDrawing();
		++fps_counter;
	}
	pthread_mutex_lock(&movables.m_balls);
	for (unsigned long i = 0; i < PowerUps.size(); ++i)
		delete PowerUps[i];
	pthread_mutex_unlock(&movables.m_balls);
	pthread_detach(*thread);
	CloseWindow();
	delete thread;
	return 0;
}

