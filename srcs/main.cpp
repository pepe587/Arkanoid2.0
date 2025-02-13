#include "../inc/Game.h"

/*To do List:
	-Add all the Sounds
	-Add more Power Ups + (IDEA) add power-downs
	-Make the level creator
	-Suicidarme.
*/

void *refresh_movables(void *_movables)
{
	t_movables *movables = (t_movables *)_movables;
	double time = 0;
	int fps = -1;
	//struct timeval start;
	//struct timeval end;
	int fps_counter = 0;

	InitAudioDevice();
	Sound sound = LoadSound("sounds/ball-bounce.wav");
	/*Rectangle movement*/
	while (!WindowShouldClose())
	{
		try { fps = GetFps(time, fps_counter); std::cout << fps <<"\n";}
		catch (...) {}
		//gettimeofday(&start, NULL);
		if (IsKeyDown(KEY_RIGHT))
		{
			pthread_mutex_lock(&(movables->m_rectangle));
			movables->R.move(+0.1);
			pthread_mutex_unlock(&(movables->m_rectangle));
		}
		if (IsKeyDown(KEY_LEFT))
		{
			pthread_mutex_lock(&(movables->m_rectangle));
			movables->R.move(-0.1);
			pthread_mutex_unlock(&(movables->m_rectangle));
		}
		/*Ball movement & check collision*/
		pthread_mutex_lock(&(movables->m_balls));
		for (unsigned long i = 0; i < movables->Balls.size(); ++i)
		{
			movables->Balls[i].updatePos();
			if (!movables->Balls[i].checkCollision(movables->R, sound, movables->Bricks, movables->BricksBreaks, movables->PowerUps, movables))
			{
				movables->Balls.erase(movables->Balls.begin() + i);
				--i;
			}
		}
		pthread_mutex_unlock(&(movables->m_balls));
		/*Power Ups movement & check collision + Boost*/
		pthread_mutex_lock(&(movables->m_powerups));
		for (unsigned long i = 0; i < movables->PowerUps.size(); ++i)
		{
			movables->PowerUps[i]->updatePos();
			if (!movables->PowerUps[i]->checkCollision(movables->R, sound, movables->Balls, movables))
			{
				delete movables->PowerUps[i];
				movables->PowerUps.erase(movables->PowerUps.begin() + i);
				--i;
			}
		}
		pthread_mutex_unlock(&(movables->m_powerups));
		//gettimeofday(&end, NULL);
		//thread_sleep(start, end);
		usleep(100);
		++fps_counter;
	}
	return (NULL);
}


int brickeableBricks(std::vector<Brick> &bricks)
{
	int i = 0;

	for (unsigned long z = 0; z < bricks.size(); z++)
		if (bricks[z].ImBreakable())
			++i;
	return i;
}

std::vector<Brick> fileToBrick(char *filename)
{
	std::ifstream level(filename);
	std::string map[50];
	std::string buffer;
	int i;

	if (!checkExtension(filename))
		return std::vector<Brick> ();
	if (!level)
	{
		std::cout << "Level file not found\n";
		return std::vector<Brick> ();
	}
	for (i = 0; std::getline(level, buffer, '\n'); ++i)
	{
		if (buffer.size() != 50)
		{
			std::cout << "Invalid level!\n";
			return std::vector<Brick> ();
		}
		map[i] = buffer;
	}
	if (i != 50)
	{
		std::cout << "50 lines is required for a level!\n";
		return std::vector<Brick> ();
	}

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

void new_game(char *level)
{
	std::vector<Ball> Balls;
	std::vector<APowerUp *> PowerUps;
	std::vector<Brick> Bricks = fileToBrick(level);
	if (!Bricks.size())
		return ;
	int	BricksBreakables = brickeableBricks(Bricks);
	int BricksBreaks = 0;

	/*TEST FOR POWERUPS*/
	//PowerUps.push_back(new ExtraBall(Vector2 {(float)GetMonitorWidth(0) / 2, (float)GetMonitorHeight(0) / 3}, Vector2 {0, 0.5}));
	//PowerUps.push_back(new MultiBall(Vector2 {(float)GetMonitorWidth(0) / 2, 0}, Vector2 {0, 0.5}));
	/*END OF THE TEST*/
	/*First Ball of the Game (HARDCODED VALUES)*/
	Balls.push_back(Ball(Vector2 {(float)GetMonitorWidth(0) / 2, (float)(GetMonitorHeight(0) / 1.5)}, Vector2 {0, 0.1}));
	
	C_Rectangle R;
	pthread_mutex_t mutexes[4];
	pthread_mutex_init(&(mutexes[0]), NULL);
	pthread_mutex_init(&(mutexes[1]), NULL);
	pthread_mutex_init(&(mutexes[2]), NULL);
	pthread_mutex_init(&(mutexes[3]), NULL);
	t_movables *movables = new t_movables;
	*movables = {Balls, R, Bricks, PowerUps, BricksBreaks, mutexes[0], mutexes[1], mutexes[2], mutexes[3]};

	pthread_t *thread = new pthread_t;
	pthread_create(thread, NULL, refresh_movables, movables);
	/*Render Thread*/
	while (!WindowShouldClose())
	{
		/*No balls = No party*/
		pthread_mutex_lock(&movables->m_balls);
		if ((!movables->Balls.size() && !thereAreUtil(movables)) || movables->BricksBreaks == BricksBreakables)
		{
			pthread_mutex_unlock(&movables->m_balls);
			break ;
		}
		pthread_mutex_unlock(&movables->m_balls);

		/*Draw Scope*/
		BeginDrawing();
		ClearBackground(BLACK);
		DrawText(("FPS: " + std::to_string(GetFPS())).c_str(), 10, 10, 50, WHITE);
		DrawText(((std::to_string((((float)movables->BricksBreaks / (float)BricksBreakables)) * 100) + " %").c_str()), 10, 100, 50, WHITE);
		DrawRectangle(GetMonitorWidth(0) * 0.25, 0, GetMonitorWidth(0) * 0.5, GetMonitorHeight(0), BLUE);
		
		pthread_mutex_lock(&movables->m_rectangle);
		movables->R.draw();
		pthread_mutex_unlock(&movables->m_rectangle);

		pthread_mutex_lock(&movables->m_bricks);
		for (unsigned long i = 0; i < movables->Bricks.size(); ++i)
			movables->Bricks[i].Draw();
		pthread_mutex_unlock(&movables->m_bricks);

		pthread_mutex_lock(&movables->m_balls);
		for (unsigned long i = 0; i < movables->Balls.size(); ++i)
			movables->Balls[i].Draw();
		pthread_mutex_unlock(&movables->m_balls);
		
		pthread_mutex_lock(&movables->m_powerups);
		for (unsigned long i = 0; i < movables->PowerUps.size(); ++i)
			movables->PowerUps[i]->Draw();
		pthread_mutex_unlock(&movables->m_powerups);
        
		EndDrawing();
	}
	free_and_close(movables, thread);
	return ;
}

void choose_level()
{
	DIR *levels;
	dirent *file;
	std::vector<std::pair<Button, std::string> > map;
	std::vector<Button> buttons;

	levels = opendir("levels/");
	if (!levels)
		exit(0);
	float x = 0.05;
	float y = 0.1;
	file = readdir(levels);
	while (file)
	{
		if (file->d_name[0] != '.')
		{
			buttons.push_back(Button(Vector2 {(float)(GetMonitorWidth(0) * x), (float)(GetMonitorHeight(0) * y)}, Vector2 {(float)(GetMonitorWidth(0) * 0.1), (float)(GetMonitorHeight(0) * 0.05)}, file->d_name, WHITE));
			map.push_back(std::make_pair(buttons[buttons.size() - 1], std::string ("levels/") + (file->d_name)));
			x += 0.2;
			if (x >= 1.05)
			{
				x = 0.05;
				y += 0.1;
			}
		}
		file = readdir(levels);
	}
	closedir(levels);
	while (1)
	{
		if (IsKeyDown(KEY_LEFT))
			return ;
		BeginDrawing();
		ClearBackground(BLUE);
		for (size_t i = 0; i < buttons.size(); ++i)
			buttons[i].Draw(BLACK, GRAY);
		for (size_t i = 0; i < buttons.size(); ++i)
		{
			if (buttons[i].isClicked())
			{
				EndDrawing();
				new_game((char *)(map[i].second).c_str());	
				return ;
			}
		}
		EndDrawing();
	}
}

int main(void)
{
	InitWindow(1, 1, "Game");
	SetWindowSize(GetMonitorWidth(0), GetMonitorHeight(0));
	ToggleFullscreen();

	Button create_level_button(Vector2 {(float)(GetMonitorWidth(0) * 0.15), (float)(GetMonitorHeight(0) * 0.2)}, Vector2{ (float)(GetMonitorWidth(0) * 0.1), (float)(GetMonitorHeight(0) * 0.05)}, "Create Level", WHITE);
	Button new_game_button(Vector2 {(float)(GetMonitorWidth(0) * 0.15), (float)(GetMonitorHeight(0) * 0.1)}, Vector2{ (float)(GetMonitorWidth(0) * 0.1), (float)(GetMonitorHeight(0) * 0.05)}, "New Game", WHITE);
	while (!WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(BLUE);
		new_game_button.Draw(BLACK, GRAY);
		create_level_button.Draw(BLACK, GRAY);
		EndDrawing();
		if (new_game_button.isClicked())
			choose_level();
	}
	CloseWindow();
	return 0;
}

