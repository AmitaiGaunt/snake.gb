#include <gb/gb.h>
#include <time.h>
#include <stdio.h>
#include <rand.h>
#include "sprites.c"

#define INT UINT8
#define BOOL UINT8

#define MAX_SPRITES 40
#define LENGTH_INIT 2
#define WIDTH 10
#define HEIGHT 18
#define X 0
#define Y 1
#define APPLE 0
#define UP 0
#define LEFT 1
#define RIGHT 2
#define DOWN 3

INT i;
INT j;
char buffer[8];
INT pos[MAX_SPRITES][2];
INT head;
INT tail;
INT direction;
INT new_direction;
INT length;
INT level;

#define P(button) (joypad() & button)

#define CONFIRM { \
		waitpadup(); \
		while (!P(J_START) && !P(J_A)); \
		waitpadup(); \
	}

void get_input() {

	if (P(J_START)) {
		CONFIRM;
	}

	new_direction = P(J_UP) ? UP
		: P(J_LEFT) ? LEFT
		: P(J_RIGHT) ? RIGHT
		: P(J_DOWN) ? DOWN
		: new_direction;
}

#define MOVE_OBJECT(id, x, y) {\
		pos[id][X] = x; \
		pos[id][Y] = y; \
		move_sprite(id, (x + 1) * 8, (y + 2) * 8); \
	}

#define COLLIDE(a, b) (pos[a][X] == pos[b][X] && pos[a][Y] == pos[b][Y])

BOOL collides_with_snake(INT id) {
	for (i = 1; i <= length; i++) {
		if (COLLIDE(id, i) && i != id) {
			return 1;
		}
	}
	return 0;
}

#define MAKE_APPLE do { \
		i = rand(); \
		i = i % WIDTH; \
		j = rand(); \
		j = j % HEIGHT; \
		MOVE_OBJECT(APPLE, i, j); \
	} while (collides_with_snake(APPLE));

#define CHECK_APPLE if (COLLIDE(APPLE, head)) { \
		MAKE_APPLE; \
		length++; \
	}

#define LEVEL_UP { \
		++level; \
	}

#define DED (collides_with_snake(head) || pos[head][X] >= WIDTH || pos[head][Y] >= HEIGHT)

#define UPDATE_SWITCHES {\
		HIDE_WIN; \
		SHOW_SPRITES; \
		SHOW_BKG; \
	}

void init() {

	DISPLAY_ON;

	CONFIRM;

	for (i = 0; i < MAX_SPRITES; i++) {
		MOVE_OBJECT(i, -1, -1);
	}

	printf("          %c\n", level + 48);
	for (i = 0; i < HEIGHT - 2; i++) {
		puts("          |");
	}

	initrand(clock());

	// Load the the 'sprites' tiles into sprite memory
	set_sprite_data(0, 1, sprites);

	set_sprite_tile(APPLE, 0x19);
	MAKE_APPLE;
	length = LENGTH_INIT;
	tail = 2;
	head = 1;
	direction = DOWN;
	MOVE_OBJECT(head, 5, 9)

}

void move() {

	if (direction + new_direction != 3) {
		direction = new_direction;
	}

	i = pos[head][X];
	j = pos[head][Y];

	switch (direction) {
	case UP:
		j--;
		break;
	case LEFT:
		i--;
		break;
	case RIGHT:
		i++;
		break;
	case DOWN:
		j++;
		break;
	}

	head = tail;
	tail = (tail % length) + 1;

	MOVE_OBJECT(head, i, j);

}

void main() {

	level = 1;

	while (1) {

		init();

		while (1) {
			get_input();
			if (clock() % (9 - level) == 0) {
				move();
				if (length == MAX_SPRITES) {
					LEVEL_UP;
					break;
				}
				if (DED) {
					level = 1;
					puts("ded");
					break;
				}
				CHECK_APPLE;
			}
			UPDATE_SWITCHES;
			wait_vbl_done();
		}

	}

}