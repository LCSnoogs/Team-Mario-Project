#include<math.h>
#include "GameUnits.h"

Bullet::Bullet(Point start, Point destination, int screen_width, int screen_height)
{
	this->bullet_center_position = start;
	this->target_center_position = destination;
	this->bullet_radius = 5;
	this->damage = 10;
	this->speed = 6;
	this->active = true;
	this->screen_height = screen_height;
	this->screen_width = screen_width;
	this->move = calculate_path(start, destination, speed);
}

int Bullet::get_damage()
{
	return this->damage;
}

void Bullet::move_bullet()
{
	if (active)
	{
		bullet_center_position.x += move.x;
		bullet_center_position.y += move.y;
		
		if ((bullet_center_position.x > screen_width) || (bullet_center_position.x < 0) || (bullet_center_position.y > screen_height) || (bullet_center_position.y < 0))
		{
			this->active = false;
		}
	}
}

Point Bullet::get_bullet_center_position()
{
	return this->bullet_center_position;
}

bool Bullet::is_active()
{
	return this->active;
}

int Bullet::get_bullet_radius()
{
	return this->bullet_radius;
}

void Bullet::set_is_active(bool active)
{
	this->active = active;
}

void Bullet::set_screen_width(int width)
{
	this->screen_width = width;
}

void Bullet::set_screen_height(int height)
{
	this->screen_height = height;
}