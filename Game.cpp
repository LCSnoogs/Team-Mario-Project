#include <stdio.h>
#include <iostream>
#include <list>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include "GameUnits.h"

using namespace std;
 
void draw_player();
void draw_bullets();
void draw_enemies();
void draw_game();
void move_bullets();
void move_units();
void fire();
bool check_for_input();
bool is_active(Bullet bullet);
void setup();
void collision();

int screen_width = 800;
int screen_height = 600;
Player player = Player(screen_width, screen_height);
Enemy_Factory enemy_factory = Enemy_Factory(screen_width, screen_height);
ALLEGRO_EVENT_QUEUE* game_event;
bool done;
bool player_is_firing = false;
ALLEGRO_TIMER* move_timer;
ALLEGRO_TIMER* shooting_timer;
ALLEGRO_DISPLAY *display;
Point mouse_position;
int counter = 0;

int main(int argc, char **argv)
{
	setup();

	while (!done)
	{  
		done = check_for_input();
		
		move_units();

		draw_game();
 
		al_flip_display();
	}
 
	al_destroy_display(display);
 
	return 0;
}

void setup()
{
	display = NULL;

	if(!al_init()) {
		fprintf(stderr, "failed to initialize allegro!\n");
	}

	al_init_primitives_addon();
	al_install_keyboard();
	al_install_mouse();
 
	display = al_create_display(screen_width, screen_height);
	if(!display) {
		fprintf(stderr, "failed to create display!\n");
	}

	al_clear_to_color(al_map_rgb(255,255,255));

	al_flip_display();

	game_event = al_create_event_queue();

	al_register_event_source(game_event, al_get_keyboard_event_source());
	al_register_event_source(game_event, al_get_mouse_event_source());
	
	move_timer = al_create_timer(.0333);
	shooting_timer = al_create_timer(.5);

	ALLEGRO_BITMAP* window = al_create_sub_bitmap(al_get_backbuffer(display),0,0,screen_width,screen_height);
	al_set_target_bitmap(window);

	done = false;
	player.set_screen_height(screen_height);
	player.set_screen_width(screen_width);
	al_start_timer(move_timer);
	al_start_timer(shooting_timer);
}

void move_units()
{
	if (al_get_timer_count(shooting_timer) > 0)
	{
		

		al_set_timer_count(shooting_timer, 0);
	}

	if (al_get_timer_count(move_timer) > 0)
	{
		counter++;
		if (counter >= player.get_reg_rate_of_fire() && player.is_shooting())
		{
			player.fire(mouse_position);
			counter = 0;
		}
		enemy_factory.run_level();
		player.move_bullets();
		player.move_player();
		enemy_factory.move_enemies(player);
		collision();
			
		al_set_timer_count(move_timer, 0);
	}
}

void draw_game()
{
	al_clear_to_color(al_map_rgb(255,255,255));
	draw_bullets();
	draw_player();
	draw_enemies();
	al_flip_display();
}

void draw_player()
{
	al_draw_filled_rectangle(player.get_player_position().x,player.get_player_position().y,player.get_player_position().x+player.get_width(),player.get_player_position().y+player.get_height(),al_map_rgb(255,255,0));
	al_draw_rectangle(player.get_player_position().x,player.get_player_position().y,player.get_player_position().x+player.get_width(),player.get_player_position().y+player.get_height(),al_map_rgb(0,0,0),0);
}

void draw_bullets()
{
	list<Bullet> bullet_list = *player.get_player_bullets();

	for (list<Bullet>::iterator it = bullet_list.begin(); it != bullet_list.end(); it++)
	{
		al_draw_filled_circle(it->get_bullet_center_position().x, it->get_bullet_center_position().y, it->get_bullet_radius(), al_map_rgb(0,0,255));
	}
}

void draw_enemies()
{
	list<Enemy> enemy_list = *enemy_factory.get_enemy_list();

	for (list<Enemy>::iterator it = enemy_list.begin(); it != enemy_list.end(); it++)
	{
		al_draw_filled_rectangle(it->get_enemy_position().x, it->get_enemy_position().y, it->get_enemy_position().x + it->get_width(), it->get_enemy_position().y + it->get_height(), al_map_rgb(255,165,0));
		al_draw_rectangle(it->get_enemy_position().x, it->get_enemy_position().y, it->get_enemy_position().x + it->get_width(), it->get_enemy_position().y + it->get_height(), al_map_rgb(0,0,0), 0);
	}
}

bool check_for_input()
{
	bool done = false;
	ALLEGRO_EVENT input;
	al_get_next_event(game_event,&input);
	if (input.type == ALLEGRO_EVENT_KEY_DOWN)
	{
		switch (input.keyboard.keycode)
		{
		case ALLEGRO_KEY_W:
			player.set_player_up(true);
			break;
		case ALLEGRO_KEY_S:
			player.set_player_down(true);
			break;
		case ALLEGRO_KEY_A:
			player.set_player_left(true);
			break;
		case ALLEGRO_KEY_D:
			player.set_player_right(true);
			break;
		case ALLEGRO_KEY_ESCAPE:
			done = true;
			break;
		}
	} else if (input.type == ALLEGRO_EVENT_KEY_UP)
	{
		switch (input.keyboard.keycode)
		{
		case ALLEGRO_KEY_W:
			player.set_player_up(false);
			break;
		case ALLEGRO_KEY_S:
			player.set_player_down(false);
			break;
		case ALLEGRO_KEY_A:
			player.set_player_left(false);
			break;
		case ALLEGRO_KEY_D:
			player.set_player_right(false);
			break;
		}
	}

	if (input.type == ALLEGRO_EVENT_MOUSE_AXES)
	{
		mouse_position.x = input.mouse.x;
		mouse_position.y = input.mouse.y;
	}

	if (input.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
	{
		player.set_shooting(true);
		mouse_position.x = input.mouse.x;
		mouse_position.y = input.mouse.y;
	} else if (input.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
	{
		player.set_shooting(false);
		counter = player.get_reg_rate_of_fire();
	}

	return done;
}

void collision()
{
	for (list<Bullet>::iterator bullet_it = player.get_player_bullets()->begin(); bullet_it != player.get_player_bullets()->end(); bullet_it++)
	{
		for (list<Enemy>::iterator enemy_it = enemy_factory.get_enemy_list()->begin(); enemy_it != enemy_factory.get_enemy_list()->end(); enemy_it++)
		{
			if (rect_to_circle_intersection(enemy_it->get_enemy_center_position(), enemy_it->get_width(), enemy_it->get_height(), bullet_it->get_bullet_center_position(), bullet_it->get_bullet_radius()))
			{
				enemy_it->set_current_health(enemy_it->get_current_health() - player.get_reg_damage());
				bullet_it->set_is_active(false);
			}
		}
	}
}