#include <gb/gb.h>
#include <time.h>
#include <stdio.h>
#include <rand.h>
#include "sprites.c"

#define INT UINT8
#define BOOL UINT8

#define MAX_SPRITES 40
#define LENGTH_INIT 3
#define UP 2
#define DOWN 3
#define LEFT 0
#define RIGHT 1
#define APPLE 0

INT i;
INT j;
INT pos[MAX_SPRITES][2];
INT front[2];
INT head;
INT tail;
INT direction;
INT newDirection;
INT length;
BOOL loss;

#define COLLIDE(a, b) (pos[a][0] == pos[b][0] && pos[a][1] == pos[b][1])

#define MOVE_OBJECT(id, x, y) {\
		move_sprite(id, x, y); \
		pos[id][0] = x; \
		pos[id][1] = y; \
	}

#define UPDATE_SWITCHES {\
		HIDE_WIN;\
		SHOW_SPRITES;\
		SHOW_BKG;\
	}

void checkInput() {

	newDirection = direction;

	// UP
	if (joypad() & J_UP && direction != DOWN) {

		newDirection = UP;

	}

	// DOWN
	if (joypad() & J_DOWN && direction != UP) {

		newDirection = DOWN;

	}

	// LEFT
	if (joypad() & J_LEFT && direction != RIGHT) {

		newDirection = LEFT;

	}

	// RIGHT
	if (joypad() & J_RIGHT && direction != LEFT) {

		newDirection = RIGHT;

	}

	// Move the sprite in the first movable sprite list (0)
	//  the the position of X (player[0]) and y (player[1])

}

#define CHECK_BIT(var,pos) ((var) & (1<<(pos)))

void move() {

	switch (direction) {
	case LEFT:
		front[0] -= 8;
		break;
	case RIGHT:
		front[0] += 8;
		break;
	case UP:
		front[1] -= 8;
		break;
	case DOWN:
		front[1] += 8;
		break;
	}

	head = tail;
	tail = (tail % length) + 1;
	MOVE_OBJECT(head, front[0], front[1]);

}

BOOL collides_with_snake(INT id) {
	for (i = 1; i <= length; i++) {
		if (COLLIDE(id, i) && i != id) {
			return 1;
		}
	}
	return 0;
}

void make_apple() {
	i = rand();
	i = i % 19;
	i = (i + 2) * 8;
	j = rand();
	j = j % 17;
	j = (j + 2) * 8;
	MOVE_OBJECT(0, i, j);
}

void check_apple() {
	if (COLLIDE(APPLE, head)) {
		make_apple();
		length++;
	}
}

void init() {

	DISPLAY_ON;						// Turn on the display

	waitpad(0xFF);
	waitpadup();
	initrand(clock());

	// Load the the 'sprites' tiles into sprite memory
	set_sprite_data(0, 1, sprites);

	length = LENGTH_INIT;

	// Set the first movable sprite (0) to be the first tile in the sprite memory (0)
	for (i = 1; i <= length; i++) {
		set_sprite_tile(i, 0);
	}

	front[0] = 80;
	front[1] = 72;
	tail = 1;
	direction = 0;
	loss = 0;

	for (i = 0; i < MAX_SPRITES; ++i) {
		for (j = 0; j < 2; ++j) {
			pos[i][j] = 0;
		}
	}

	set_sprite_tile(0, 0x19);
	make_apple();

}

void main() {

	init();

	while (!loss) {
		checkInput();				// Check for user input (and act on it)
		if (clock() % 8 == 0) {
			direction = newDirection;
			move();
			if (collides_with_snake(head)) {
				puts("ded");
				return;
			}
			check_apple();
		}
		UPDATE_SWITCHES;			// Make sure the SHOW_SPRITES and SHOW_BKG switches are on each loop
		wait_vbl_done();			// Wait until VBLANK to avoid corrupting memory


	}

}