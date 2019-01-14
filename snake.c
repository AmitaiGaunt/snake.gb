#include <gb/gb.h>
#include <time.h>
#include <stdio.h>
#include "sprites.c"

#define MAX_SPRITES 40

#define LENGTH_INIT 5

#define UP 2
#define DOWN 3
#define LEFT 0
#define RIGHT 1

void init();
void checkInput();
void updateSwitches();
void move();
void selfCollision();

// The player array will hold the player's position as X ([0]) and Y ([1])

UINT8 i;
UINT8 j;
UINT8 pos[MAX_SPRITES][2];
UINT8 front[2];
UINT8 head[2];
UINT8 direction;
UINT8 newDirection;
UINT8 length;
UINT8 tail;
UINT8 loss;

void main() {

	init();

	while (!loss) {
		checkInput();				// Check for user input (and act on it)
		if (clock() % 8 == 0) {
			direction = newDirection;
			move();
			selfCollision();
		}
		updateSwitches();			// Make sure the SHOW_SPRITES and SHOW_BKG switches are on each loop
		wait_vbl_done();			// Wait until VBLANK to avoid corrupting memory


	}

}

void init() {

	DISPLAY_ON;						// Turn on the display

	// Load the the 'sprites' tiles into sprite memory
	set_sprite_data(0, 1, sprites);

	length = LENGTH_INIT;

	// Set the first movable sprite (0) to be the first tile in the sprite memory (0)
	for (i = 1; i <= length; i++) {
		set_sprite_tile(i, 0);
	}

	front[0] = 80;
	front[1] = 72;
	tail = 0;
	direction = 0;
	loss = 0;

	for (i = 0; i < MAX_SPRITES; ++i) {
		for (j = 0; j < 2; ++j) {
			pos[i][j] = 0;
		}
	}
}

void updateSwitches() {

	HIDE_WIN;
	SHOW_SPRITES;
	SHOW_BKG;

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
	//pos[tail][0] = pos[tail - 1][0];
	//pos[tail][1] = pos[tail - 1][1];

	tail = (tail % length) + 1;

	pos[tail][0] = front[0];
	pos[tail][1] = front[1];
	move_sprite(tail, front[0], front[1]);




}

UINT8 collisionCount;

void selfCollision() {
	collisionCount = 0;
	for (i = 0; i < length; ++i) {
		if (pos[i][0] == front[0] && pos[i][1] == front[1]) { //If a sprite of the snake body is at the same position as the front sprite die

			//if () {
			++collisionCount;
			//}

		}
	}

	if (collisionCount > 1) {
		while (1);
	}
}