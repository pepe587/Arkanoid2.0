#include "../inc/Game.h"


int main(void)
{
	std::vector<Ball> Balls;
	InitWindow(GetMonitorWidth(0), GetMonitorHeight(0), "Game");
	ToggleFullscreen(); 
	
	Balls.push_back(Ball(GetMonitorWidth(0) / 2, GetMonitorHeight(0) / 2, Vector2 {0.5, 0.5}));
	//for (int i = 0; i < 200; ++i)
	//	Balls.push_back(Ball(GetMonitorWidth(0) / 2, GetMonitorHeight(0) / 2, Vector2 {(float)(GetRandomValue(-2000, 2000) / 10), (float)(GetRandomValue(-2000, 2000) / 10)}));
	C_Rectangle R;
	double time = 0;
	int fps = 0;
	while (!WindowShouldClose())
	{
		if (GetTime() - time >= 1)
		{
			std::cout << "FPS = " << fps << "\n";
			time = GetTime();
			fps = 0;
		}
		if (IsKeyDown(KEY_RIGHT))
			R.move(1);
		if (IsKeyDown(KEY_LEFT))
			R.move(-1);
		for (unsigned long i = 0; i < Balls.size(); ++i)
		{
			Balls[i].updatePos();
			if (!Balls[i].checkCollition(R))
			{
				Balls.erase(Balls.begin() + i);
				--i;
			}
		}
		if (!Balls.size())
			break ;
		BeginDrawing();
		ClearBackground(BLUE);
		R.draw();
		for (unsigned long i = 0; i < Balls.size(); ++i)
			Balls[i].draw();
        EndDrawing();
		++fps;
	}
	CloseWindow();
}