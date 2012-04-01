#include<list>
#include<time.h>
#include "GameUnits.h"

using namespace std;

Enemy_Factory::Enemy_Factory(int screen_width, int screen_height)
{
	this->enemy_count = 0;
	this->enemy_max = 10;
	this->screen_height = screen_height;
	this->screen_width = screen_width;

	time_t now;
	time(&now);
	this->counter = now;
}

void Enemy_Factory::run_level()
{
	if (enemy_list.size() == 0)
	{
		enemy_max++;
		enemy_count = 0;
	}
	if (enemy_count < enemy_max)
	{
		Enemy enemy = Enemy(spawn_enemy_point(), screen_width, screen_height, enemy_count);
		enemy_list.push_back(enemy);
		enemy_count++;
	}
}

Point Enemy_Factory::spawn_enemy_point()
{
	int temp;
	int quadrant;
	Point spawn_point;
	srand(counter);
	quadrant = rand() % 4 + 1;

	switch (quadrant)
	{
	case 1:
		spawn_point.x = 0;

		temp = rand() % screen_height + 1;
		spawn_point.y = temp;
		break;

	case 2:
		temp = rand() % screen_width + 1;
		spawn_point.x = temp;
		
		spawn_point.y = 0;
		break;

	case 3:
		spawn_point.x = screen_width;

		temp = rand() % screen_height + 1;
		spawn_point.y = temp;
		break;

	case 4:
		temp = rand() % screen_width + 1;
		spawn_point.x = temp;

		spawn_point.y = screen_height;
		break;
	}

	counter++;
	return spawn_point;
}

list<Enemy>* Enemy_Factory::get_enemy_list()
{
	return &this->enemy_list;
}

void Enemy_Factory::move_enemies(Player player)
{
	list<Enemy> live_enemies;
	for (list<Enemy>::iterator it = enemy_list.begin(); it != enemy_list.end(); it++)
	{
		it->move_behavior(player);
		it->flocking(enemy_list);
		if (it->is_alive())
		{
			live_enemies.push_back(*it);
		}
	}

	enemy_list = live_enemies;
}