#include "../inc/Game.h"

/*To do List:
	-Add all the Sounds
	-Add more Power Ups + (IDEA) add power-downs
	-Make the level creator (90 %)
	-Limit the while of the movement to 3000 iters per second
	-Fix the Bug of the corners (Gameplay)
*/

Texture2D CreateBackground() {
    int screenWidth = GetMonitorWidth(0);
    int screenHeight = GetMonitorHeight(0);
    Image background = GenImageColor(screenWidth, screenHeight, (Color){ 0, 0, 0, 255 });

    // Zona izquierda (Información)
    ImageDrawRectangle(&background, 0, 0, screenWidth * 0.25, screenHeight, (Color){ 15, 15, 20, 255 });

    // Zona derecha (Información)
    ImageDrawRectangle(&background, screenWidth * 0.75, 0, screenWidth * 0.25, screenHeight, (Color){ 15, 15, 20, 255 });

    // Zona de gameplay (Central)
    for (int y = 0; y < screenHeight; y++) {
        float t = (float)y / (float)screenHeight;
        Color gradientColor = ColorLerp((Color){ 10, 10, 20, 255 }, (Color){ 0, 0, 40, 255 }, t);
        ImageDrawRectangle(&background, screenWidth * 0.25, y, screenWidth * 0.5, 1, gradientColor);
    }

    // Bordes luminosos para separar zonas
    ImageDrawRectangle(&background, screenWidth * 0.25 - 2, 0, 2, screenHeight, (Color){ 255, 107, 107, 200 }); // Izquierda
    ImageDrawRectangle(&background, screenWidth * 0.75, 0, 2, screenHeight, (Color){ 255, 107, 107, 200 });     // Derecha

    Texture2D texture = LoadTextureFromImage(background);
    UnloadImage(background);
    return texture;
}

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
	Texture2D background = CreateBackground();
	while (1)
	{
		if (IsKeyDown(KEY_LEFT))
		{
			if (actual)
				delete actual;
			break ;
		}
		if (IsKeyDown(KEY_C))
		{
			delete actual;
			return ;
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
		DrawTexture(background, 0, 0, WHITE);
		DrawRectangle(GetMonitorWidth(0) * 0.25, 0, GetMonitorWidth(0) * 0.5, GetMonitorHeight(0), (Color){ 0, 102, 204, 50 });
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
		if (IsKeyDown(KEY_C))
		{
			delete actual;
			return ;
		}
	}
	BricksToFile(input.getValue(), Bricks);
}

/*Los cuadrantes son para el spatial partioning,
el diametro de la bola es (ancho de la pantalla * 0.75) / 100
quiero que sean 3 bolas de ancho y 3 de largo supongo pero tengoqq hacer q encaje perfecto *****
*/

void init_quadrants(t_quadrants **q)
{
	int width = GetMonitorWidth(0);
	int height = GetMonitorHeight(0);
	int x = width * 0.25;
	int y = 0;
	double ballDiameter = (GetMonitorPhysicalWidth(0) * 0.75) / 100;

	while (i <= width)
	{
		
	}
}

void *refresh_movables(void *_movables)
{
	t_movables *movables = (t_movables *)_movables;
	double time = 0;
	int fps = -1;
	//struct timeval start;
	//struct timeval end;
	int fps_counter = 0;
	t_quadrant q[5][10];
	InitAudioDevice();
	Sound sound = LoadSound("sounds/ball-bounce.wav");
	/*Rectangle movement*/
	init_quadrants(q);
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
	Texture2D background = CreateBackground();

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
		DrawTexture(background, 0, 0, WHITE);
		DrawText(("FPS: " + std::to_string(GetFPS())).c_str(), 10, 10, 50, WHITE);
		DrawText(((std::to_string((((float)movables->BricksBreaks / (float)BricksBreakables)) * 100) + " %").c_str()), 10, 100, 50, WHITE);
		DrawRectangle(GetMonitorWidth(0) * 0.25, 0, GetMonitorWidth(0) * 0.5, GetMonitorHeight(0), (Color){ 0, 102, 204, 50 });

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
	free_and_close(movables, thread, background);
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
		DrawMenuBackground();
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

	// Tamaño y espacio entre los botones
	float buttonWidth = (float)(GetMonitorWidth(0) * 0.3); // 30% del ancho de la pantalla
	float buttonHeight = (float)(GetMonitorHeight(0) * 0.05); // 5% de la altura de la pantalla
	float verticalSpacing = (float)(GetMonitorHeight(0) * 0.05); // Espacio entre botones

	// Posición de los botones
	float startX = (GetMonitorWidth(0) - buttonWidth) / 2; // Centrado horizontal
	float startY = (GetMonitorHeight(0) - (buttonHeight * 4 + verticalSpacing * 3)) / 2; // Centrado vertical (considerando 4 botones y el espacio entre ellos)

	// Crear los botones con posiciones ajustadas
	Button Playbutton(Vector2{ startX, startY }, Vector2{ buttonWidth, buttonHeight }, "Play", WHITE);
	Button create_level_button(Vector2{ startX, startY + buttonHeight + verticalSpacing }, Vector2{ buttonWidth, buttonHeight }, "Create Level", WHITE);
	Button settings_button(Vector2{ startX, startY + (buttonHeight + verticalSpacing) * 2 }, Vector2{ buttonWidth, buttonHeight }, "Settings", WHITE);
	Button exit_button(Vector2{ startX, startY + (buttonHeight + verticalSpacing) * 3 }, Vector2{ buttonWidth, buttonHeight }, "Exit", WHITE);
	while (!WindowShouldClose())
	{
		BeginDrawing();
		DrawMenuBackground();
		Playbutton.Draw(BLACK, GRAY);
		create_level_button.Draw(BLACK, GRAY);
		settings_button.Draw(BLACK, GRAY);
		exit_button.Draw(BLACK, GRAY);
		EndDrawing();
		if (Playbutton.isClicked())
			choose_level();
		if (create_level_button.isClicked())
			create_level();
		if (settings_button.isClicked())
			std::cout << "en desarrollo\n";
		if (exit_button.isClicked())
			break;
	}
	CloseWindow();
	return 0;
}

