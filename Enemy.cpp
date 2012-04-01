#include<list>
#include<math.h>
#include<time.h>
#include "GameUnits.h"

using namespace std;

Enemy::Enemy(Point spawn_point, int screen_width, int screen_height, int enemy_number)
{
	this->enemy_number = enemy_number;
	this->speed = 1;
	this->enemy_position = spawn_point;
	this->current_health = 30;
	this->max_health = 30;
	this->width = 20;
	this->height = 20;
	this->enemy_center_position.x = enemy_position.x + width/2;
	this->enemy_center_position.y = enemy_position.y + height/2;
	this->alive = true;
	this->damage = 1;
	this->screen_height = screen_height;
	this->screen_width = screen_width;

	time_t now;
	time(&now);
	this->counter = now;
}

int Enemy::get_enemy_number()
{
	return this->enemy_number;
}

int Enemy::get_damage()
{
	return this->damage;
}

int Enemy::get_speed()
{
	return this->speed;
}

Point Enemy::get_enemy_position()
{
	return this->enemy_position;
}

Point Enemy::get_enemy_center_position()
{
	return this->enemy_center_position;
}

void Enemy::set_enemy_position(Point enemy_position)
{
	this->enemy_position = enemy_position;
	enemy_center_position.x = enemy_position.x - width/2;
	enemy_center_position.y = enemy_position.y - height/2;
}

int Enemy::get_current_health()
{
	return this->current_health;
}

void Enemy::set_current_health(int health)
{
	this->current_health = health;
	if (this->current_health <= 0)
	{
		this->alive = false;
	}
}

int Enemy::get_max_health()
{
	return this->max_health;
}

void Enemy::move_behavior(Player player)
{
	Point move = calculate_path(enemy_center_position, player.get_center_position(), speed);
	enemy_center_position.x += move.x;
	enemy_center_position.y += move.y;
	enemy_position.x = enemy_center_position.x - width/2;
	enemy_position.y = enemy_center_position.y - height/2;
}

void Enemy::flocking(list<Enemy> enemy_list)
{
	for (list<Enemy>::iterator it = enemy_list.begin(); it != enemy_list.end(); it++)
	{
		if (enemy_number != it->get_enemy_number())
		{
			if (rect_intersection(enemy_position, width, height, it->get_enemy_position(), it->get_width(), it->get_height()))
			{
				srand(counter);
				if (enemy_position.x >= it->get_enemy_position().x)
				{
					enemy_position.x += 1;
				}
				else
				{
					enemy_position.x -= 1;
				}
		
				if (enemy_position.y >= it->get_enemy_position().y)
				{
					enemy_position.y += 1;
				}
				else
				{
					enemy_position.y -= 1;
				}
				counter++;
			}
		}
	}
}

bool Enemy::is_alive()
{
	return this->alive;
}

void Enemy::set_alive(bool alive)
{
	this->alive = alive;
}

int Enemy::get_width()
{
	return this->width;
}

int Enemy::get_height()
{
	return this->height;
}