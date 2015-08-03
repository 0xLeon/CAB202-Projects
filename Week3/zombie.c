#include <limits.h>
#include <stdio.h>
#include <string.h>
#include "cab202_graphics.h"

#define I_TO_S_LENGTH ((CHAR_BIT * sizeof(int) - 1) / 3) + 2

void draw_score(int x, int y, int score) {
	char i_to_s[I_TO_S_LENGTH];
	char str[I_TO_S_LENGTH + 7];
	
	snprintf(i_to_s, I_TO_S_LENGTH, "%d", score);
	stpcpy(str, "Score: ");
	strcat(str, i_to_s);
	
	draw_string(x, y, str);
}

void draw_menu(int width, int height) {
	draw_string(0, height - 1, "Menu: 8 = N; 2 = S; 6 = E; 4 = W; 9 = NE; 3 = SE; 7 = NW; 1 = SW; q = Quit.");
}

void running_zombie() {
	int width;
	int height;
	get_screen_size(width, height);
	
	int x = width * 4 / 5;
	int y = height * 4 / 5;
	
	int score = 0;
	int turn = 0;
	
	draw_score(30, 0, score);
	draw_menu(width, height);
	draw_char(x, y, 'Z');
	
	show_screen();
	
	int key = wait_char();
	turn++;
	
	while ((key != 'q') && (key >= 0)) {
		switch (key) {
			case '4':
				x--;
				break;
			case '6':
				x++;
				break;
			case '8':
				y--;
				break;
			case '2':
				y++;
				break;
			case '7':
				x--;
				y--;
				break;
			case '3':
				x++;
				y++;
				break;
			case '9':
				x++;
				y--;
				break;
			case '1':
				x--;
				y++;
				break;
		}
		
		if ((turn % 5) == 0) {
			score++;
			turn = 0;
		}
		
		clear_screen();
		
		draw_score(30, 0, score);
		draw_menu(width, height);
		draw_char(x, y, 'Z');
		
		show_screen();
		
		key = wait_char();
		turn++;
	}
}

int main() {
	setup_screen();
	running_zombie();
	cleanup_screen();
	
	return 0;
}