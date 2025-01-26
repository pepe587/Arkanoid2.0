#include "../inc/Game.h"

/*To do List:
	-Add all the Sounds
	-Add more Power Ups + (IDEA) add power-downs
	-Fix the bug of the collision (if the collision is with the x axis the ball goes to a bad direction)
	-Find solution to the problem of FPS (if FPS > speed >) IMPORTANT (raycasting can be a solution)
	-Add a text with the percent of destroyed square
	-Change the method of the install the dependencies (idk if this its a good idea), search for AppImage for linux and a .exe installer for windows
	Improve FPS
*/

bool thereAreUtil(std::vector<APowerUp *> &v)
{
	for (unsigned long i = 0; i < v.size(); ++i)
		if (dynamic_cast<ExtraBall *>(v[i]))
			return true;
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
		"00112233443322110000000000000011223344332211000000", // Línea 11
		"00112233443322111111111111111111223344332211000000", // Línea 12
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
		"00001111222233334444555555554444333322221111000000", // Línea 25
		"00000000000000001111222233332222111100000000000000", // Línea 26
		"00000000000000000000000000000000000000000000000000", // Línea 27
		"00001111222200000000000000000000222211110000000000", // Línea 28
		"00000000000000000000000000000000000000000000000000", // Línea 29
		"00111111111111111111111111111111111111111111110000", // Línea 30
		"00222222222222222222222222222222222222222222220000", // Línea 31
		"00333333333333333333333333333333333333333333330000", // Línea 32
		"00000000000000000000000000000000000000000000000000", // Línea 33
		"00001111222233334444555555554444333322221111000000", // Línea 34
		"00001111222200000000000000000000222211110000000000", // Línea 35
		"00000000000000000000000000000000000000000000000000", // Línea 36
		"00000000000000001111111111111111000000000000000000", // Línea 37
		"00000000000000000000000000000000000000000000000000", // Línea 38
		"00001122334455443322110000001122334455443322110000", // Línea 39
		"00001111222233334444555555554444333322221111000000", // Línea 40
		"00000000000000000000000000000000000000000000000000", // Línea 41
		"00111111111111111111111111111111111111111111110000", // Línea 42
		"00222222222222222222222222222222222222222222220000", // Línea 43
		"00333333333333333333333333333333333333333333330000", // Línea 44
		"00000000000000000000000000000000000000000000000000", // Línea 45
		"00001111222233334444555555554444333322221111000000", // Línea 46
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
	InitWindow(GetMonitorWidth(0), GetMonitorHeight(0), "Game");
	ToggleFullscreen();
	InitAudioDevice();
	Sound sound = LoadSound("sounds/ball-bounce.wav");
	std::vector<Brick> Bricks = intToBrick();
	int	BricksBreakables = brickeableBricks(Bricks);
	int BricksBreaks = 0;

	/*TEST FOR POWERUPS*/
	//PowerUps.push_back(new ExtraBall(Vector2 {(float)GetMonitorWidth(0) / 2, (float)GetMonitorHeight(0) / 3}, Vector2 {0, 0.5}));
	//PowerUps.push_back(new MultiBall(Vector2 {(float)GetMonitorWidth(0) / 2, 0}, Vector2 {0, 0.5}));
	/*END OF THE TEST*/

	/*First Ball of the Game (HARDCODED VALUES)*/
	Balls.push_back(Ball(Vector2 {(float)GetMonitorWidth(0) / 2, (float)(GetMonitorHeight(0) / 1.5)}, Vector2 {0.5, 2}));
	
	/*TEST FOR ADD X QTY OF BALLS IN A RANDOM POS WITH A RANDOM VECTOR SPEED*/
	//for (int i = 0; i < 1000; ++i)
	//	Balls.push_back(Ball(GetMonitorWidth(0) / 2, GetMonitorHeight(0) / 2, Vector2 {(float)(GetRandomValue(-2000, 2000) / 10), (float)(GetRandomValue(-2000, 2000) / 10)}));
	/*END OF THE TEST*/
	C_Rectangle R;
	double time = 0;
	int fps_counter = 0;
	int actual_fps = 0;
	while (!WindowShouldClose())
	{
		/*Calculate FPS*/
		if (GetTime() - time >= 1)
		{
			actual_fps = fps_counter;
			time = GetTime();
			fps_counter = 0;
		}
		/*Rectangle movement*/
		if (IsKeyDown(KEY_RIGHT))
			R.move(2);
		if (IsKeyDown(KEY_LEFT))
			R.move(-2);
		/*Ball movement & check collition*/
		for (unsigned long i = 0; i < Balls.size(); ++i)
		{
			Balls[i].updatePos();
			if (!Balls[i].checkCollition(R, sound, Bricks, BricksBreaks))
			{
				Balls.erase(Balls.begin() + i);
				--i;
			}
		}
		/*Power Ups movement & check collition + Boost*/
		for (unsigned long i = 0; i < PowerUps.size(); ++i)
		{
			PowerUps[i]->updatePos();
			if (!PowerUps[i]->checkCollition(R, sound, Balls))
			{
				delete PowerUps[i];
				PowerUps.erase(PowerUps.begin() + i);
				--i;
			}
		}
		/*No balls = No party*/
		if (!Balls.size() && !thereAreUtil(PowerUps))
			break ;
		/*Draw Scope*/
		BeginDrawing();
		ClearBackground(BLACK);
		DrawText(("FPS: " + std::to_string(actual_fps)).c_str(), 10, 10, 50, WHITE);
		DrawText(((std::to_string(((BricksBreaks / BricksBreakables)) * 100) + " %").c_str()), 10, 100, 50, WHITE);
		DrawRectangle(GetMonitorWidth(0) * 0.25, 0, GetMonitorWidth(0) * 0.5, GetMonitorHeight(0), BLUE);
		R.draw();
		for (unsigned long i = 0; i < Bricks.size(); ++i)
			Bricks[i].Draw();
		for (unsigned long i = 0; i < Balls.size(); ++i)
			Balls[i].Draw();
		for (unsigned long i = 0; i < PowerUps.size(); ++i)
			PowerUps[i]->Draw();
        EndDrawing();
		++fps_counter;
	}
	for (unsigned long i = 0; i < PowerUps.size(); ++i)
		delete PowerUps[i];
	CloseWindow();
}