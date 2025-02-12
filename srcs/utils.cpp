#include "../inc/Game.h"

float	getRandom(float min, float max)
{
	float rand = 0;

	while (rand == 0)
		rand = (float)GetRandomValue((int)(min * 10), (int)(max * 10)) / 10;
	return rand;
}

void free_and_close(t_movables *movables, pthread_t *thread)
{
	pthread_cancel(*thread);
	CloseAudioDevice();
	pthread_mutex_lock(&movables->m_balls);
	for (unsigned long i = 0; i < movables->PowerUps.size(); ++i)
		delete movables->PowerUps[i];
	pthread_mutex_unlock(&movables->m_balls);
	delete thread;
	delete movables;
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