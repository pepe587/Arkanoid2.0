#include "../inc/Game.h"

/*To do List:
	-Add all the Sounds
	-Add the boosters
	-Add the squares
	-Find solution to the problem of FPS (if FPS > speed >)
	-Add a text with the percent of destroyed square
	-Change the method of the install the dependencies
*/

bool thereAreUtil(std::vector<APowerUp *> &v)
{
	for (unsigned long i = 0; i < v.size(); ++i)
		if (dynamic_cast<ExtraBall*>(v[i]))
			return true;
	return false;
}

int main(void)
{
	std::vector<Ball> Balls;
	std::vector<APowerUp *> PowerUps;
	InitWindow(GetMonitorWidth(0), GetMonitorHeight(0), "Game");
	ToggleFullscreen();
	InitAudioDevice();
	Sound sound = LoadSound("sounds/ball-bounce.wav");
	

	/*TEST FOR POWERUPS*/
	PowerUps.push_back(new ExtraBall(Vector2 {(float)GetMonitorWidth(0) / 2, (float)GetMonitorHeight(0) / 3}, Vector2 {0, 0.5}));
	//PowerUps.push_back(new MultiBall(Vector2 {(float)GetMonitorWidth(0) / 2, 0}, Vector2 {0, 0.5}));
	/*END OF THE TEST*/

	Balls.push_back(Ball(Vector2 {(float)GetMonitorWidth(0) / 2, (float)GetMonitorHeight(0) / 2}, Vector2 {0.5, 0.5}));
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
			R.move(1);
		if (IsKeyDown(KEY_LEFT))
			R.move(-1);
		/*Ball movement & check collition*/
		for (unsigned long i = 0; i < Balls.size(); ++i)
		{
			Balls[i].updatePos();
			if (!Balls[i].checkCollition(R, sound))
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
		DrawRectangle(GetMonitorWidth(0) * 0.25, 0, GetMonitorWidth(0) * 0.5, GetMonitorHeight(0), BLUE);
		R.draw();
		for (unsigned long i = 0; i < Balls.size(); ++i)
			Balls[i].draw();
		for (unsigned long i = 0; i < PowerUps.size(); ++i)
			PowerUps[i]->Draw();
        EndDrawing();
		++fps_counter;
	}
	for (unsigned long i = 0; i < PowerUps.size(); ++i)
		delete PowerUps[i];
	CloseWindow();
}