/*
	gui.c
	---------------------------------
	출력만을 처리하는 함수들을 정의한 파일
*/

#include "gui.h"


/*
	String : _draw_floor	
*/
char *_draw_floor[2] = { "█████", "░░░░░" };

/*
	Function : draw_floor
	---------------------------------
	게임 화면의 floor을 출력한다.
	draw_floor에서 내부적으로 사용하는 함수
	flag에 따라 출력이 다름
*/
void draw_floor(int floor_state[])
{
	move(25, 0);
	attron(COLOR_PAIR(2));	printw("        ██"); attroff(COLOR_PAIR(2));
	attron(COLOR_PAIR(1));	printw(_draw_floor[floor_state[0]]); attroff(COLOR_PAIR(1));
	attron(COLOR_PAIR(2));	printw(" █ "); attroff(COLOR_PAIR(2));
	attron(COLOR_PAIR(3));	printw(_draw_floor[floor_state[1]]); attroff(COLOR_PAIR(3));
	attron(COLOR_PAIR(2));	printw(" █ "); attroff(COLOR_PAIR(2));
	attron(COLOR_PAIR(1));	printw(_draw_floor[floor_state[2]]); attroff(COLOR_PAIR(1));
	attron(COLOR_PAIR(2));	printw(" █ "); attroff(COLOR_PAIR(2));
	attron(COLOR_PAIR(4));	printw(_draw_floor[floor_state[3]]); attroff(COLOR_PAIR(4));
	attron(COLOR_PAIR(2));	printw(" █ "); attroff(COLOR_PAIR(2));
	attron(COLOR_PAIR(1));	printw(_draw_floor[floor_state[4]]); attroff(COLOR_PAIR(1));
	attron(COLOR_PAIR(2));	printw(" █ "); attroff(COLOR_PAIR(2));
	attron(COLOR_PAIR(3));	printw(_draw_floor[floor_state[5]]); attroff(COLOR_PAIR(3));
	attron(COLOR_PAIR(2));	printw(" █ "); attroff(COLOR_PAIR(2));
	attron(COLOR_PAIR(1));  printw(_draw_floor[floor_state[6]]); attroff(COLOR_PAIR(1));
	attron(COLOR_PAIR(2)); 	printw("██"); attroff(COLOR_PAIR(2));
	move(0, 0);
}

/*
	Function : draw_stage
	---------------------------------
	게임화면의 뼈대(라인)를 출력하는 함수
*/
void draw_stage(int floor_state[])
{
	printw("\n\n\n\n\n");
	printw("        │       │       │       │       │       │       │       │\n");
	printw("        │       │       │       │       │       │       │       │\n");
	printw("        │       │       │       │       │       │       │       │\n");
	printw("        │       │       │       │       │       │       │       │\n");
	printw("        │       │       │       │       │       │       │       │\n");
	printw("        │       │       │       │       │       │       │       │\n");
	printw("        │       │       │       │       │       │       │       │\n");
	printw("        │       │       │       │       │       │       │       │\n");
	printw("        │       │       │       │       │       │       │       │\n");
	printw("        │       │       │       │       │       │       │       │\n");
	printw("        │       │       │       │       │       │       │       │\n");
	printw("        │       │       │       │       │       │       │       │\n");
	printw("        │       │       │       │       │       │       │       │\n");
	printw("        │       │       │       │       │       │       │       │\n");
	printw("        │       │       │       │       │       │       │       │\n");
	printw("        │       │       │       │       │       │       │       │\n");
	printw("        ├       │       │       │       │       │       │       ┤\n");
	printw("        ├       ┼       ┼       ┼       ┼       ┼       ┼       ┤\n");
	printw("        ├       ┼       ┼       ┼       ┼       ┼       ┼       ┤\n");
	printw("        └─");
	attron(COLOR_PAIR(1));	printw("─────"); attroff(COLOR_PAIR(1));
	attron(COLOR_PAIR(2)); 	printw("─┴─"); attroff(COLOR_PAIR(2));
	attron(COLOR_PAIR(3));  printw("─────"); attroff(COLOR_PAIR(3));
	attron(COLOR_PAIR(2));	printw("─┴─");	attroff(COLOR_PAIR(2));
	attron(COLOR_PAIR(1));	printw("─────"); attroff(COLOR_PAIR(1));
	attron(COLOR_PAIR(2));	printw("─┴─"); attroff(COLOR_PAIR(2));
	attron(COLOR_PAIR(4));	printw("─────"); attroff(COLOR_PAIR(4));
	attron(COLOR_PAIR(2));	printw("─┴─"); attroff(COLOR_PAIR(2));
	attron(COLOR_PAIR(1));	printw("─────"); attroff(COLOR_PAIR(1));
	attron(COLOR_PAIR(2));	printw("─┴─"); attroff(COLOR_PAIR(2));
	attron(COLOR_PAIR(3));	printw("─────"); attroff(COLOR_PAIR(3));
	attron(COLOR_PAIR(2));  printw("─┴─"); attroff(COLOR_PAIR(2));
	attron(COLOR_PAIR(1));	printw("─────"); attroff(COLOR_PAIR(1));
	attron(COLOR_PAIR(2));	printw("─┘\n"); attroff(COLOR_PAIR(2));
	draw_floor(floor_state);
}

/*
	Function : shownum
	---------------------------------
	지정한 좌표에 num에 해당하는 숫자를 출력한다.
*/
void shownum(int num, int y, int x) {
	if (num == 0) {
		move(y, x);
		printw("████████\n"); move(y + 1, x);
		printw("██    ██\n"); move(y + 2, x);
		printw("██    ██\n"); move(y + 3, x);
		printw("██    ██\n"); move(y + 4, x);
		printw("██    ██\n"); move(y + 5, x);
		printw("████████\n");
	}
	else if (num == 1) {
		move(y, x);
		printw("      ██\n"); move(y + 1, x);
		printw("      ██\n"); move(y + 2, x);
		printw("      ██\n"); move(y + 3, x);
		printw("      ██\n"); move(y + 4, x);
		printw("      ██\n"); move(y + 5, x);
		printw("      ██\n");
	}
	else if (num == 2) {
		move(y, x);
		printw("████████\n"); move(y + 1, x);
		printw("      ██\n"); move(y + 2, x);
		printw("████████\n"); move(y + 3, x);
		printw("██      \n"); move(y + 4, x);
		printw("██      \n"); move(y + 5, x);
		printw("████████\n");
	}
	else if (num == 3) {
		move(y, x);
		printw("████████\n"); move(y + 1, x);
		printw("      ██\n"); move(y + 2, x);
		printw("████████\n"); move(y + 3, x);
		printw("      ██\n"); move(y + 4, x);
		printw("      ██\n"); move(y + 5, x);
		printw("████████\n");
	}
	else if (num == 4) {
		move(y, x);
		printw("██    ██\n"); move(y + 1, x);
		printw("██    ██\n"); move(y + 2, x);
		printw("████████\n"); move(y + 3, x);
		printw("      ██\n"); move(y + 4, x);
		printw("      ██\n"); move(y + 5, x);
		printw("      ██\n");
	}
	else if (num == 5) {
		move(y, x);
		printw("████████\n"); move(y + 1, x);
		printw("██      \n"); move(y + 2, x);
		printw("████████\n"); move(y + 3, x);
		printw("      ██\n"); move(y + 4, x);
		printw("      ██\n"); move(y + 5, x);
		printw("████████\n");
	}
	else if (num == 6) {
		move(y, x);
		printw("████████\n"); move(y + 1, x);
		printw("██      \n"); move(y + 2, x);
		printw("████████\n"); move(y + 3, x);
		printw("██    ██\n"); move(y + 4, x);
		printw("██    ██\n"); move(y + 5, x);
		printw("████████\n");
	}
	else if (num == 7) {
		move(y, x);
		printw("████████\n"); move(y + 1, x);
		printw("      ██\n"); move(y + 2, x);
		printw("      ██\n"); move(y + 3, x);
		printw("      ██\n"); move(y + 4, x);
		printw("      ██\n"); move(y + 5, x);
		printw("      ██\n");
	}
	else if (num == 8) {
		move(y, x);
		printw("████████\n"); move(y + 1, x);
		printw("██    ██\n"); move(y + 2, x);
		printw("████████\n"); move(y + 3, x);
		printw("██    ██\n"); move(y + 4, x);
		printw("██    ██\n"); move(y + 5, x);
		printw("████████\n");
	}
	else if (num == 9) {
		move(y, x);
		printw("████████\n"); move(y + 1, x);
		printw("██    ██\n"); move(y + 2, x);
		printw("████████\n"); move(y + 3, x);
		printw("      ██\n"); move(y + 4, x);
		printw("      ██\n"); move(y + 5, x);
		printw("      ██\n");
	}
}

/*
	Function : showblank
	---------------------------------
	지정한 좌표에 공백을 출력하는 함수
*/
void showblank(int y, int x) {
	move(y, x);
	printw("        \n"); move(y + 1, x);
	printw("        \n"); move(y + 2, x);
	printw("        \n"); move(y + 3, x);
	printw("        \n"); move(y + 4, x);
	printw("        \n"); move(y + 5, x);
	printw("        \n");
}

/*
	Function : start_screen
	---------------------------------
	시작 화면을 출력하는 함수
*/
void start_screen()
{
	attron(COLOR_PAIR(1));
	printw("\n\n\n\n");
	printw("        █████           █       █         █   █████████      \n");
	printw("       █    ██         █ █      ██       ██   █               \n");
	printw("      █               █   █     █ █     █ █   █████████       \n");
	printw("      █   ██████     ███████    █  █   █  █   █              \n");
	printw("       █   █ █      █       █   █   █ █   █   █               \n");
	printw("        ███  █     █         █  █    █    █   █████████       \n");
	printw("                                                         \n");
	printw("      █████████                    █                              \n");
	printw("      █        █                  █ █                          \n");
	printw("      █         █                █   █                        \n");
	printw("      █          █              █     █                      \n");
	printw("      █          █             █       █                     \n");
	printw("      █ ████████        ███████         ████████                   \n");
	printw("      ██                █                     █            \n");
	printw("      █ █                 █                 █            \n");
	printw("      █   █                 █             █                \n");
	printw("      █     █                 █         █                  \n");
	printw("      █       █               ███████████                     \n");
	printw("                                                        \n\n\n");
	printw("      (press enter to start)                                           \n");
	attroff(COLOR_PAIR(1));
}