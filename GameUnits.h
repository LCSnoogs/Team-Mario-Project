#ifndef GAMEUNITS_H
#define GAMEUNITS_H

#include<list>

using namespace std;

struct Point
{
	float x;
	float y;
};

class Bullet
{
private:
	int screen_width, screen_height;
	int speed;
	Point bullet_center_position;
	Point target_center_position;
	int bullet_radius;
	int direction;
	bool active;
	int damage;
	Point move;

public:
	Bullet(Point start, Point destination, int screen_width, int screen_height);
	int get_damage();
	void move_bullet();
	Point get_bullet_center_position();
	bool is_active();
	int get_bullet_radius();
	void set_is_active(bool active);
	void set_screen_width(int width);
	void set_screen_height(int height);
};

class Player
{
private:
	bool player_up, player_down, player_left, player_right;
	int speed;
	Point player_position;
	Point center_position;
	int current_health, max_health;
	int screen_width, screen_height;
	int width, height;
	int reg_damage;
	int reg_rate_of_fire;
	list<Bullet> player_bullets;
	bool shooting;

public:
	Player(int screen_width, int screen_height);
	int get_width();
	int get_height();
	int get_speed();
	Point get_player_position();
	Point get_center_position();
	void set_player_position(Point position);
	int get_current_health();
	void set_current_health(int health);
	int get_max_health();
	void move_player();
	void set_screen_width(int width);
	void set_screen_height(int width);
	void set_player_up(bool player_up);
	bool is_player_up();
	void set_player_down(bool player_down);
	bool is_player_down();
	void set_player_left(bool player_left);
	bool is_player_left();
	void set_player_right(bool player_right);
	bool is_player_right();
	void reset();
	int get_reg_damage();
	int get_reg_rate_of_fire();
	list<Bullet>* get_player_bullets();
	void fire(Point destination);
	bool is_shooting();
	void set_shooting(bool shooting);
	void move_bullets();
};

class Enemy
{
private:
	int enemy_number;
	int speed;
	Point enemy_position;
	Point enemy_center_position;
	int current_health, max_health;
	int screen_width, screen_height;
	int width, height;
	bool alive;
	int damage;
	float dist_y;
	float dist_x;
	float vector_y;
	float vector_x;
	float move_y;
	float move_x;
	unsigned int counter;

public:
	Enemy(Point spawn_point, int screen_width, int screen_height, int enemy_number);
	int get_enemy_number();
	int get_damage();
	int get_speed();
	Point get_enemy_position();
	Point get_enemy_center_position();
	void set_enemy_position(Point enemy_position);
	int get_current_health();
	void set_current_health(int health);
	int get_max_health();
	void move_behavior(Player player);
	void flocking(list<Enemy> enemy_list);
	bool is_alive();
	void set_alive(bool alive);
	int get_width();
	int get_height();
};

class Enemy_Factory
{
private:
	list<Enemy> enemy_list;
	int enemy_max;
	int enemy_count;
	int screen_width, screen_height;
	unsigned int counter;

	Point spawn_enemy_point();

public:
	Enemy_Factory(int screen_width, int screen_height);
	void run_level();
	list<Enemy>* get_enemy_list();
	void move_enemies(Player player);
};

Point calculate_path(Point start, Point destination, int speed);
bool rect_intersection(Point rect1_position, int rect1_width, int rect1_height, Point rect2_position, int rect2_width, int rect2_height);
bool rect_to_circle_intersection(Point rect_center, int rect_width, int rect_height, Point circle_center, int circle_radius);

#endif
