#include<list>
#include<math.h>
#include"GameUnits.h"

using namespace std;

Player::Player(int screen_width, int screen_height)
{
	this->screen_width = screen_width;
	this->screen_height = screen_height;
	this->width = 10;
	this->height = 10;
	this->max_health = 100;
	this->current_health = 100;
	this->player_down = false;
	this->player_left = false;
	this->player_right = false;
	this->player_up = false;
	this->player_position.x = 0;
	this->player_position.y = 0;
	this->center_position.x = player_position.x + width/2;
	this->center_position.y = player_position.y + height/2;
	this->reg_damage = 10;
	this->reg_rate_of_fire = 20;
	this->speed = 3;
	this->shooting = false;
	this->player_bullets;
}

int Player::get_current_health()
{
	return this->current_health;
}

void Player::set_current_health(int health)
{
	this->current_health = health;
}

int Player::get_max_health()
{
	return this->max_health;
}

int Player::get_height()
{
	return this->height;
}

int Player::get_width()
{
	return this->width;
}

int Player::get_speed()
{
	return this->speed;
}

Point Player::get_player_position()
{
	return this->player_position;
}

Point Player::get_center_position()
{
	this->center_position.x = player_position.x + width/2;
	this->center_position.y = player_position.y + height/2;
	return this->center_position;
}

void Player::set_player_position(Point position)
{
	this->player_position = position;
}

int Player::get_reg_damage()
{
	return this->reg_damage;
}

int Player::get_reg_rate_of_fire()
{
	return this->reg_rate_of_fire;
}

void Player::set_screen_width(int width)
{
	this->screen_width = width;
}

void Player::set_screen_height(int height)
{
	this->screen_height = height;
}

void Player::set_player_up(bool player_up)
{
	this->player_up = player_up;
}

bool Player::is_player_up()
{
	return this->player_up;
}

void Player::set_player_down(bool player_down)
{
	this->player_down = player_down;
}

bool Player::is_player_down()
{
	return this->player_down;
}

void Player::set_player_left(bool player_left)
{
	this->player_left = player_left;
}

bool Player::is_player_left()
{
	return this->player_left;
}

void Player::set_player_right(bool player_right)
{
	this->player_right = player_right;
}

bool Player::is_player_right()
{
	return this->player_right;
}

void Player::reset()
{
	player_position.x = 0;
	player_position.y = 0;
	current_health = max_health;
	player_up = false;
	player_down = false;
	player_left = false;
	player_right = false;
}

void Player::move_player()
{
	if (player_up)
	{
		if (player_position.y > 0)
		{
			player_position.y -= speed;
		}
		else
		{
			player_position.y = 0;
		}
	}
	if (player_down)
	{
		if (player_position.y < screen_height - height - 1)
		{
			player_position.y += speed;
		}
		else
		{
			player_position.y = (float)(screen_height - height - 1);
		}
	}
	if (player_left)
	{
		if (player_position.x > 0)
		{
			player_position.x -= speed;
		}
		else
		{
			player_position.x = 0;
		}
	}
	if (player_right)
	{
		if (player_position.x < screen_width - width - 1)
		{
			player_position.x += speed;
		}
		else
		{
			player_position.x = (float)(screen_width - width - 1);
		}
	}
}

list<Bullet>* Player::get_player_bullets()
{
	return &player_bullets;
}

void Player::fire(Point destination)
{
	if (shooting)
	{
		Point start = {player_position.x + width/2, player_position.y + height/2};
		Bullet bullet = Bullet(start, destination, screen_width, screen_height);
		player_bullets.push_back(bullet);
	}
}

bool Player::is_shooting()
{
	return this->shooting;
}

void Player::set_shooting(bool shooting)
{
	this->shooting = shooting;
}

void Player::move_bullets()
{
	list<Bullet> live_bullets;
	for (list<Bullet>::iterator it = player_bullets.begin(); it != player_bullets.end(); it++)
	{
		it->move_bullet();
		
		if (it->is_active())
		{
			live_bullets.push_back(*it);
		}
	}
	
	player_bullets.clear();
	player_bullets = live_bullets;
}

Point calculate_path(Point start, Point destination, int speed)
{
	Point move;
	float vector_y = destination.y - start.y;
	float vector_x = destination.x - start.x;

	float dist_y = abs(vector_y);
	float dist_x = abs(vector_x);

	vector_y /= dist_y;
	vector_x /= dist_x;

	if (dist_x == 0 && dist_y == 0)
	{
		move.x = 0;
		move.y = 0;
	}
	else if (dist_x == 0)
	{
		move.y = speed * vector_y;
		move.x = 0;
	}
	else if (dist_y == 0)
	{
		move.y = 0;
		move.x = speed * vector_x;
	}
	else
	{
		move.y = speed * sin(atan(dist_y/dist_x));
		move.x = speed * cos(atan(dist_y/dist_x));

		move.y *= vector_y;
		move.x *= vector_x;
	}

	return move;
}

bool rect_intersection(Point rect1_position, int rect1_width, int rect1_height, Point rect2_position, int rect2_width, int rect2_height)
{
	if (rect1_position.x >= rect2_position.x && rect1_position.x <= (rect2_position.x + rect2_width))
	{
		if (rect1_position.y >= rect2_position.y && rect1_position.y <= (rect2_position.y + rect2_height))
		{
			return true;
		}
		else if ((rect1_position.y + rect1_height) >= rect2_position.y && (rect1_position.y + rect1_height) <= (rect2_position.y + rect2_height))
		{
			return true;
		}
	}
	else if ((rect1_position.x + rect1_width) >= rect2_position.x && (rect1_position.x + rect1_width) <= (rect2_position.x + rect2_width))
	{
		if (rect1_position.y >= rect2_position.y && rect1_position.y <= (rect2_position.y + rect2_height))
		{
			return true;
		}
		else if ((rect1_position.y + rect1_height) >= rect2_position.y && (rect1_position.y + rect1_height) <= (rect2_position.y + rect2_height))
		{
			return true;
		}
	}

	return false;
}

bool rect_to_circle_intersection(Point rect_center, int rect_width, int rect_height, Point circle_center, int circle_radius)
{
	float dist_x = abs(circle_center.x - rect_center.x);
	float dist_y = abs(circle_center.y - rect_center.y);
	float distance = sqrt(dist_y * dist_y + dist_x * dist_x);
	float hypotenuse = 0;

	if (dist_x == 0)
	{
		hypotenuse = rect_height/2;
	}
	else if (dist_y == 0)
	{
		hypotenuse = rect_width/2;
	}
	else if (dist_y >= dist_x)
	{
		hypotenuse = (rect_height/2) / sin(atan(dist_y/dist_x));
	}
	else
	{
		hypotenuse = (rect_width/2) / cos(atan(dist_y/dist_x));
	}

	if (hypotenuse + circle_radius >= distance)
	{
		return true;
	}
	
	return false;
}