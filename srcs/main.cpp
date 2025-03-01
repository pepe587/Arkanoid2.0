#include "../inc/Game.h"

/*To do List:
	-Add all the Sounds
	-Add more Power Ups + (IDEA) add power-downs
	-Make the level creator (90 %)
	-Limit the while of the movement to 3000 iters per second
	-Fix the Bug of the corners (Gameplay)
*/

void	create_level()
{
	std::vector<Brick> Bricks;
	Vector2 size = {(float)(GetMonitorWidth(0) / 5.9), (float)(GetMonitorHeight(0) / 10)};
	Vector2 sizeB = {(float)(GetMonitorWidth(0) / 100), (float)(GetMonitorHeight(0) / 90)};
	Button SBrick(Vector2 {(float)(GetMonitorWidth(0) * 0.25), (float)(GetMonitorHeight(0) - GetMonitorHeight(0) / 10)}, size);
	Button IBrick(Vector2 {(float)(GetMonitorWidth(0) * 0.25 + GetMonitorWidth(0) / 6), (float)(GetMonitorHeight(0) - GetMonitorHeight(0) / 10)}, size);
	Button RBrick(Vector2 {(float)(GetMonitorWidth(0) * 0.25 + GetMonitorWidth(0) / 3), (float)(GetMonitorHeight(0) - GetMonitorHeight(0) / 10)}, size);

	Bricks.push_back(Brick(SIMPLE, Vector2 {(float)(GetMonitorWidth(0) * 0.25 +  GetMonitorWidth(0) / 12 - sizeB.x * 2), (float)(GetMonitorHeight(0) - GetMonitorHeight(0) / 10 + GetMonitorHeight(0) / 20 - sizeB.y * 2)}, Vector2 {sizeB.x * 4, sizeB.y * 4}));
	Bricks.push_back(Brick(INDESTRUCTIBLE, Vector2 {(float)(GetMonitorWidth(0) * 0.25 + GetMonitorWidth(0) / 6 +  GetMonitorWidth(0) / 12 - sizeB.x * 2), (float)(GetMonitorHeight(0) - GetMonitorHeight(0) / 10 + GetMonitorHeight(0) / 20 - sizeB.y * 2)}, Vector2 {sizeB.x * 4, sizeB.y * 4}));
	Bricks.push_back(Brick(RESISTANT, Vector2 {(float)(GetMonitorWidth(0) * 0.25 + GetMonitorWidth(0) / 3 +  GetMonitorWidth(0) / 12 - sizeB.x * 2), (float)(GetMonitorHeight(0) - GetMonitorHeight(0) / 10 + GetMonitorHeight(0) / 20 - sizeB.y * 2)}, Vector2 {sizeB.x * 4, sizeB.y * 4}));
	Brick *actual = NULL;
	while (1)
	{
		if (IsKeyDown(KEY_LEFT))
		{
			if (actual)
				delete actual;
			break ;
		}

		if (actual)
			actual->setPos(roundPos(GetMousePosition()));

		if (IsMouseButtonDown(0) && actual && canPutBrick())
		{
			actual->setPos(roundPos(GetMousePosition()));
			if (!IsTaken(Bricks, *actual))
				Bricks.push_back(*actual);
		}

		if (IsMouseButtonDown(1))
			deleteByPos(Bricks);

		BeginDrawing();
		ClearBackground(BLACK);
		DrawRectangle(GetMonitorWidth(0) * 0.25, 0, GetMonitorWidth(0) * 0.5, GetMonitorHeight(0), BLUE);
		IBrick.Draw(BLACK, BLANK);
		RBrick.Draw(BLACK, BLANK);
		SBrick.Draw(BLACK, BLANK);

		if (IBrick.isClicked())
		{
			if (actual)
				delete actual;
			actual = new Brick(INDESTRUCTIBLE, GetMousePosition(), sizeB);
			usleep(100000);		
		}
		else if (SBrick.isClicked())
		{
			if (actual)
				delete actual;
			actual = new Brick(SIMPLE, GetMousePosition(), sizeB);
			usleep(100000);
		}
		else if (RBrick.isClicked())
		{
			if (actual)
				delete actual;
			actual = new Brick(RESISTANT, GetMousePosition(), sizeB);
			usleep(100000);
		}
		if (actual)
			actual->Draw();
		
		for (size_t i = 0; i < Bricks.size(); ++i)
			Bricks[i].Draw();
		EndDrawing();
	}
	Input input(Vector2 {(float)(GetMonitorWidth(0) * 0.33), (float)(GetMonitorHeight(0) * 0.5)}, Vector2 {(float)(GetMonitorWidth(0) * 0.33), (float)(GetMonitorHeight(0) * 0.1)});
	
	while (!IsKeyDown(KEY_ENTER))
	{
		BeginDrawing();
		DrawRectangleV(Vector2 {(float)(GetMonitorWidth(0) * 0.33), (float)(GetMonitorHeight(0) * 0.5)}, Vector2 {(float)(GetMonitorWidth(0) * 0.33), (float)(GetMonitorHeight(0) * 0.1)}, BLACK);
		input.Draw(BLACK);
		EndDrawing();
		input.listenKeys();
	}
	BricksToFile(input.getValue(), Bricks);
}


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
		if (buffer.size() != 51)
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
	t_movables local;

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
		local.R = movables->R;
		pthread_mutex_unlock(&movables->m_rectangle);
		local.R.draw();

		pthread_mutex_lock(&movables->m_bricks);
		local.Bricks = movables->Bricks;
		pthread_mutex_unlock(&movables->m_bricks);
		for (unsigned long i = 0; i < local.Bricks.size(); ++i)
			local.Bricks[i].Draw();

		pthread_mutex_lock(&movables->m_balls);
		local.Balls = movables->Balls;
		pthread_mutex_unlock(&movables->m_balls);
		for (unsigned long i = 0; i < local.Balls.size(); ++i)
			local.Balls[i].Draw();

		pthread_mutex_lock(&movables->m_powerups);
		local.PowerUps = movables->PowerUps;
		pthread_mutex_unlock(&movables->m_powerups);
        for (unsigned long i = 0; i < local.PowerUps.size(); ++i)
			local.PowerUps[i]->Draw();

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
		if (file->d_name[0] != '.' && checkExtension(file->d_name))
		{
			buttons.push_back(Button(Vector2 {(float)(GetMonitorWidth(0) * x), (float)(GetMonitorHeight(0) * y)}, Vector2 {(float)(GetMonitorWidth(0) * 0.1), (float)(GetMonitorHeight(0) * 0.05)}, std::string (file->d_name).substr(0, strlen(file->d_name) - 5), WHITE));
			map.push_back(std::make_pair(buttons[buttons.size() - 1], std::string ("levels/") + (file->d_name)));
			x += 0.2;
			if (x >= 0.80)
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
		if (create_level_button.isClicked())
			create_level();
	}
	CloseWindow();
	return 0;
}

