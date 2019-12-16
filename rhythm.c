/*
	rhythm3.c
	---------------------------------
	프로그램의 주요 로직들이 정의되어 있는 파일로, main함수를 포함한다.
*/

#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <fcntl.h>
#include <termios.h>
#include <locale.h>
#include <pthread.h>
#include <signal.h>
#include <ctype.h>
#include <linux/input.h>
#include "gui.h"
#include "socklib.h"

#define FALSE 	0
#define TRUE 	1
#define TRUE2 2
#define DOWN 10
#define UP 11
#define PORTNUM	12345

// 플레이어의 점수를 저장하는 구조체
struct AllState {
	int combo;
	int maxcombo;
	int perfect;
	int good;
	int bad;
	int miss;
};

struct AllState all_state = { 0, 0, 0, 0, 0, 0 };
struct AllState other_state = { 0, 0, 0, 0, 0, 0 };		// for multiplay

int floor_state[7] = { 0, };
int note[21][7] = { 0, };								// note의 상태
int check_note[4][7] = { 0, };
int note_change[4][7] = { 0, };
int tic, startwait;										// 노트의 delay와 초기 delay
int key_response[107] = { 0, };							// 유효한 키 여부
int key_color[7] = { 1, 3, 1, 4, 1, 3, 1 };				// floor의 색깔
int note_color[4] = { 0, 0, 1, 1 };	
int update = TRUE;
int update_start = FALSE;
int note_alarm = FALSE;
char *device_name;

int long_note_interval[7] = { FALSE, };
int long_note_enter[7] = { FALSE, };

char* note_path[5] = { "sample.txt","","","","" };
char* music_path[5] = { "Prototype.mp3","","","","" };
int pid_toKill = 0;
int prior_pid_toKill = 0;
int index_toPlay = 0;

void playMusic(int index);
void showscore(int score, int y, int x);

void set_tic()
{
	struct itimerval new_timeset;
	long n_sec, n_usecs;

	new_timeset.it_interval.tv_sec = 0;
	new_timeset.it_interval.tv_usec = tic;
	new_timeset.it_value.tv_sec = 0;
	new_timeset.it_value.tv_usec = tic;

	setitimer(ITIMER_REAL, &new_timeset, NULL);
}

void tic_response(int s)
{
	note_alarm = TRUE;
}
/*
	Function : showState
	---------------------------------
	플레이어의 점수를 보여준다
*/
void showState(void)
{
	move(13, 68);	printw("maxcombo : %d", all_state.maxcombo);
	showscore(all_state.combo,3,68);
	move(14, 68);	printw("perfect : %d", all_state.perfect);
	move(15, 68);	printw("good : %d", all_state.good);
	move(16, 68);	printw("bad : %d", all_state.bad);
	move(17, 68);	printw("miss : %d", all_state.miss);
}

/*
	Function : showOtherState
	---------------------------------
	멀티플레이에서 상대 플레이어의 점수를 보여준다
*/
void showOtherState()
{
	move(13, 98);	printw("Other player's current state");
	move(14, 98);	printw("perfect : %d", other_state.perfect);
	move(15, 98);	printw("good : %d", other_state.good);
	move(16, 98);	printw("bad : %d", other_state.bad);
	move(17, 98);	printw("miss : %d", other_state.miss);
}

/*
	Function : check_long_note
	---------------------------------
	롱노트인지 판단하고, 롱노트일 경우 입력에 대한 처리를 진행한다.
	판단 및 처리 결과를 바탕으로 리턴값 반환.
*/
int check_long_note(int key, int state, int event)
{
	if (state == 1)			// 롱노트가 아닌 경우
		return FALSE;
	else if (state == 2) {
		if (long_note_interval[key] == TRUE && long_note_enter[key] == FALSE)
			return TRUE2;

		long_note_interval[key] = !long_note_interval[key];
		long_note_enter[key] = !long_note_enter[key];

		return TRUE;
	}
	else if (state == 3 && event == UP) {
		if (all_state.maxcombo < all_state.combo)
			all_state.maxcombo = all_state.combo;
		all_state.combo = 0;
		all_state.miss++;

		long_note_enter[key] = FALSE;

		return TRUE2;
	}
	else
		return TRUE2;
}

/*
	Function : check_state
	---------------------------------
	입력한 키와, 그 키에 해당하는 라인을 조사해서 perfect, good, bad를 판단한다
*/
void check_state(const int key, int *arrb, int *arrg1, int *arrg2, int *arrp, int event)
{
	if (*arrg1 || *arrg2 || *arrp)	//만약 노트가 있으면 어느 판정선에 있는지 확인	
	{
		if (*arrp) {	//perfect인지 판단		
			if (check_long_note(key, *arrp, event) != TRUE2) {	//리턴 값이 TRUE2일 때 : *arrp가 1이거나 2가 아닐 때, *arrp가 롱노트 구간이 끝나는 2인데 롱노트진입상태가 아닐 때
				*arrp = 0; all_state.perfect++;
				all_state.combo++;
				if (all_state.maxcombo < all_state.combo)
					all_state.maxcombo = all_state.combo;
				note_change[3][key] = TRUE;
			}
		}
		else if (*arrg2){ //good인지 판단
			if (check_long_note(key, *arrg2, event) != TRUE2) {
				*arrg2 = 0; all_state.good++;
				all_state.combo++;
				if (all_state.maxcombo < all_state.combo)
					all_state.maxcombo = all_state.combo;
				note_change[2][key] = TRUE;
			}
		}
		else { //good인지 판단
			if (check_long_note(key, *arrg1, event) != TRUE2) {
				*arrg1 = 0; all_state.good++;
				all_state.combo++;
				if (all_state.maxcombo < all_state.combo)
					all_state.maxcombo = all_state.combo;
				note_change[1][key] = TRUE;
			}
		}
	}
	else if (*arrb)	//bad인지 판단
	{
		if (check_long_note(key, *arrb, event) != TRUE2) {
			*arrb = 0; all_state.bad++;
			if (all_state.maxcombo < all_state.combo)
				all_state.maxcombo = all_state.combo;
			all_state.combo = 0;
			note_change[0][key] = TRUE;
		}
	}
}

/*
	Function : note_update
	---------------------------------
	입력한 키를 처리하며, 프레임을 체크한다.
*/
void note_update(int fd)
{
	int key, i, j;	//key가 0:S, 1:D, 2:F, 3:SpaceBar, 4:J, 5:K, 6:L
	int device = open(device_name, O_RDONLY | O_NONBLOCK);
	struct input_event ev;
	clock_t s, e;
	void tic_response(int);

	while(update_start != TRUE);

	usleep(startwait);
	signal(SIGALRM, tic_response);
	set_tic();

	while (1)
	{
		for(i = 0; i < 3; i++)
			for (j = 0; j < 7; j++) {
				if (note_change[i][j] == TRUE) {
					note_change[i][j] = FALSE;	note[i + 17][j] = 0;
					move(i + 21, 8 * j + 9);	printw("      ");
				}
				else
					check_note[i][j] = note[i + 16][j];
			}
		for (j = 0; j < 7; j++) {
			if (note_change[i][j] == TRUE) {
				note_change[i][j] = FALSE;	note[i + 17][j] = 0;
			}
			else
				check_note[i][j] = note[i + 16][j];
		}
		refresh();

		update = TRUE;
		note_alarm = FALSE;

		while (note_alarm != TRUE)	//노트들에 대한 기능이 끝나고 프레임이 끝나기전 남은 시간 동안 키보드 입력을 받는다.
		{
			read(device, &ev, sizeof(ev));

			if (ev.type != EV_KEY)
				continue;
			if ((key = key_response[ev.code]) == 0)
				continue;

			if (ev.value == 1) {		// 키가 눌렸을 경우
				key--;
				floor_state[key] = 1;
				check_state(key, &check_note[0][key], &check_note[1][key], &check_note[2][key], &check_note[3][key], DOWN);
			}
			else if (ev.value == 0) {	// 키가 떼어졌을 경우
				key--;
				floor_state[key] = 0;
				if (long_note_interval[key] == TRUE)
					check_state(key, &check_note[0][key], &check_note[1][key], &check_note[2][key], &check_note[3][key], UP);
			}
		}

		if (fd != 0)
			write(fd, &all_state, sizeof(all_state));
	}
}

/*
	Function : node_drop
	---------------------------------
	파일로 부터 노트를 입력받고, 현재 노트를 출력 및 갱신한다.
	노트를 프레임마다 내리며, miss에 대한 판정을 이 함수에서 처리한다.
*/
void *note_drop(void *a)
{
	FILE* stream;
	int multiFlag = *(int*)a;
	int i, j;
	int long_output_interval[7] = { FALSE, };

	stream = fopen(note_path[index_toPlay], "rt");
	fscanf(stream, "%d %d", &startwait, &tic);
	update_start = TRUE;

	while (1)
	{	
		if (update == TRUE)
		{
			for (i = 0; i < 7; i++) //미스 검사
			{
				if (note[20][i] % 3 > 0)
				{
					if (note[20][i] == 2) {		//롱노트구간 확인
						long_note_interval[i] = !long_note_interval[i];
						if (long_note_interval[i] != TRUE && long_note_enter[i] == FALSE)	//롱노트구간에 있었는데 롱노트진입 상태가 아니면 미스로 판정하지 않는다.
							continue;
					}

					if (all_state.maxcombo < all_state.combo)
						all_state.maxcombo = all_state.combo;
					all_state.combo = 0;
					all_state.miss++;
				}
			}

			draw_floor(floor_state);
			showState();
			if (multiFlag != 1)
				showOtherState();

			fscanf(stream, "%d %d %d %d %d %d %d", &note[0][0], &note[0][1], &note[0][2], &note[0][3], &note[0][4], &note[0][5], &note[0][6]);	

			for (i = 0; i < 7; i++) {		//롱노트를 출력하기 위해 롱노트 구간을 표시한다.
				if (note[0][i] == 2)
					long_output_interval[i] = !long_output_interval[i];
				else if (long_output_interval[i] == TRUE)
					note[0][i] = 3;
			}

			for (i = 0; i < 19; i++)
			{
				for (j = 0; j < 7; j++)
				{
					if (note[i][j])
					{
						move(i + 5, 8 * j + 9);
						attron(COLOR_PAIR(note_color[note[i][j]]));
						printw(" ▆▆▆▆▆");
						attroff(COLOR_PAIR(note_color[note[i][j]]));
					}
					else
					{
						move(i + 5, 8 * j + 9);
						printw("      ");
					}
				}
			}

			for (i = 20; i > 0; i--)
				for (j = 0; j < 7; j++)
					note[i][j] = note[i - 1][j];

			update = FALSE;
		}
	}

	fclose(stream);

	update_start = FALSE;

	return NULL;
}

/*
	Function : showscore
	---------------------------------
	플레이어의 현재 콤보를 보여준다.
*/
void showscore(int score, int y, int x) {
	//in case 3 digit score
	if ((score / 100) % 10)
		shownum((score / 100) % 10, y, x);
	else
		showblank(y, x);

	if ((score / 10) % 10 || (score / 100) % 10)
		shownum((score / 10) % 10, y, x + 9);
	else
		showblank(y, x + 9);

	shownum(score % 10, y, x + 18);
}

/*
	Function : select_screen_helper
	---------------------------------
	노래를 선택하는 화면을 보여준다.
*/
void select_screen_helper(char num)
{   
	attron(COLOR_PAIR(1));
	printw("\n\n\n\n");
	printw("    CHOOSE SONG BY PRESS NUMBER\n\n");
	if(num =='1'){attroff(COLOR_PAIR(1));attron(COLOR_PAIR(2)); printw("     1. song 1    \n\n"); attroff(COLOR_PAIR(2));attron(COLOR_PAIR(1));}else{
	printw("     1. song 1\n\n");}
	if(num =='2'){attroff(COLOR_PAIR(1));attron(COLOR_PAIR(2)); printw("     2. song 2    \n\n"); attroff(COLOR_PAIR(2));attron(COLOR_PAIR(1));}else{
	printw("     2. song 2\n\n");}
	if(num =='3'){attroff(COLOR_PAIR(1));attron(COLOR_PAIR(2)); printw("     3. song 3    \n\n"); attroff(COLOR_PAIR(2));attron(COLOR_PAIR(1));}else{
	printw("     3. song 3\n\n");}
	if(num =='4'){attroff(COLOR_PAIR(1));attron(COLOR_PAIR(2)); printw("     4. song 4    \n\n"); attroff(COLOR_PAIR(2));attron(COLOR_PAIR(1));}else{
	printw("     4. song 4\n\n");}
	if(num =='5'){attroff(COLOR_PAIR(1));attron(COLOR_PAIR(2)); printw("     5. song 5    \n\n"); attroff(COLOR_PAIR(2));attron(COLOR_PAIR(1));}else{
	printw("     5. song 5\n\n\n\n\n\n\n\n\n\n\n");}
	printw("      (press space bar to start)\n");
	attroff(COLOR_PAIR(1));
}

/*
	Function : select_screen
	---------------------------------
	노래를 선택하는 로직을 정의한 함수
*/
void select_screen(){
	char c = '0';
	char prior = '0';
	move(0,0);
    select_screen_helper(c);
    refresh();
	while (1){
        c = getch();
        
		if (c == ' '){
			index_toPlay = prior- '0' - 1;
			break;
		}		
		if(c=='1'){
			if(prior != '1'){
				playMusic(0);
			}
		}
		else if(c=='2'){
			if(prior != '2'){
				playMusic(1);
			}
		}
		else if(c=='3'){
			if(prior != '3'){
				playMusic(2);
			}
		}
		else if(c=='4'){
			if(prior != '4'){
				playMusic(3);
			}
		}
		else if(c=='5'){
			if(prior != '5'){
				playMusic(4);
			}
		}
        move(0,0);
        select_screen_helper(c);
		prior = c;
        refresh();
    }
}

/*
	Function : renderPlayTypeScreen
	---------------------------------
	플레이 타입을 선택하는 화면을 보여준다.
*/
int renderPlayTypeScreen(char num){

	const int typeNum = 3;
	int flag = num - '0';
	char *type[] = {NULL, "싱글 플레이", "멀티 플레이(Host)", "멀티 플레이(Guest)"};
	char *description[] = {
		NULL
	,	"싱글 플레이 모드는 혼자서 게임을 즐길 수 있으며, 자신의 최고 기록을 갱신하는 것이 목표인 모드입니다."
	,	"멀티 플레이(Host) 모드는 서버를 열어, 서버에 접속한 상대방과 대결을 하는 모드입니다."
	,	"멀티 플레이(Guest) 모드는 원하는 서버에 접속해, 상대방과 대결을 하는 모드입니다. "};

	// print play type
	attron(COLOR_PAIR(1));
	printw("\n\n\n\n");
	printw("    플레이 타입을 고르세요        \n\n");

	for (int cur = 1; cur <= typeNum; cur++)
	{
		if (cur == flag){
			attroff(COLOR_PAIR(1));
			attron(COLOR_PAIR(2)); printw("%10s%d. %-s\n\n", "", cur, type[cur]); attroff(COLOR_PAIR(2));
			attron(COLOR_PAIR(1));
		}
		else
			printw("%10s%d. %-s\n\n", "", cur, type[cur]);
	}

	// if flag is invalid, then return
	if (flag <= 0 || flag > typeNum)
		return -1;

	printw("\n\n\n");
	attroff(COLOR_PAIR(1)); printw("%5s%s\n\n", "", description[flag]); attron(COLOR_PAIR(1));
	printw("%10s(press space bar to confirm)\n", "");
	printw("        ");
	attroff(COLOR_PAIR(1));

	return 0;
}

/*
	Function : playTypeScreen
	---------------------------------
	플레이 타입을 결정하는 로직을 정의
*/
int playTypeScreen(){
	char c = '0';
	char prevc = '0';
	move(0,0);
   	int okay = renderPlayTypeScreen(c);
    refresh();
	while (1){
        c = getch();

		if (c == ' ' && okay == 0)
			break;

        move(0,0);
		clear();
        okay = renderPlayTypeScreen(c);
		prevc = c;
        refresh();
    }

	return (int)(prevc - '0');
}

/*
	Function : playMusic
	---------------------------------
	선택한 곡을 재생하며, 이미 재생되고 있는 곡이 있으면 해당 프로세스를 kill 한다.
*/
void playMusic(int index){
	if((pid_toKill=fork())==-1){
		endwin();
		printf("fork() is wrong\n");
		exit(1);
	}
	if(pid_toKill ==0){//child process do play music
		close(2);//not stderr
		execlp("mpg321","mpg321",music_path[index],NULL);
		exit(1);
	}
	else{//parent process do turn off prior music if it exists

		if(prior_pid_toKill >0){//if prior exists
			int pid;
			if((pid = fork())== -1){
				endwin();
				printf("fork() is wrong\n");
				exit(1);
			}
			if(pid ==0){
				close(2);//not stderr
				char string_toKill[10];
				sprintf(string_toKill,"%d",prior_pid_toKill);
				execlp("kill","kill","-KILL",string_toKill,NULL);
			}
			else{
				
			}
		}

		prior_pid_toKill = pid_toKill;//update prior pid(whether if or if not)
	}
}

/*
	Function : reader
	---------------------------------
	멀티 플레이에서 상대방의 점수를 읽어온다.
*/
void *reader(void *p)
{
	int fd = *(int*)p;
	while(read(fd, &other_state, sizeof(other_state)) > 0);
	return NULL;
}

/*
	Function : sigpipeHandler
	---------------------------------
	socket 통신에서 write 에러시 발생하는 sigpipe를 처리하는 handler이다.
*/
void sigpipeHandler(int signal)
{
	endwin();
	fprintf(stderr, "Connection Error!\n");
	
	if(prior_pid_toKill >0){ //if prior exists
		int pid;
		if((pid = fork())== -1){
			endwin();
			printf("fork() is wrong\n");
			exit(1);
		}
		if(pid ==0){
			close(2);//not stderr
			char string_toKill[10];
			sprintf(string_toKill,"%d",prior_pid_toKill);
			execlp("kill","kill","-KILL",string_toKill,NULL);
		}
	}

	exit(1);
}

/*
	Function : main
	---------------------------------
	메인 로직을 정의한 함수
*/
int main(int argc, char *argv[])
{	
	//signal(SIGCHLD,SIG_IGN);//for turning off music
	//alternative is using thread for while(waitpid(-1,) = ....)

	pthread_t nd, readThd;
	int sockfd = 0;
	int multiFlag = 0;
	device_name = argv[1];

	key_response[31] = 1; key_response[32] = 2; key_response[33] = 3; key_response[57] = 4; key_response[36] = 5; key_response[37] = 6; key_response[38] = 7; //키를 누르면 그 키에 대응되는 노트배열의 색인을 찾음

	signal(SIGPIPE, sigpipeHandler);	// socket에서 write 오류 처리
	setlocale(LC_ALL, "ko_KR.utf8");	// 유니코드 처리
	setlocale(LC_CTYPE, "ko_KR.utf8");

	initscr();
	if(has_colors() == FALSE)			// 색깔 지원 여부 확인              
	{       endwin();                                                           
			printf("You terminal does not support color\n");                    
			exit(1);                                                            
	}                                                                           
	start_color();

	//assign color pair(text color, back ground color) to integer
	init_pair(1, COLOR_BLUE, COLOR_BLACK);
	init_pair(2, COLOR_GREEN, COLOR_BLACK);
	init_pair(3, COLOR_YELLOW, COLOR_BLACK);
	init_pair(4, COLOR_MAGENTA, COLOR_BLACK);
	init_pair(5, COLOR_CYAN, COLOR_BLACK);
	init_pair(6, COLOR_YELLOW, COLOR_BLACK);

	clear();
	crmode();
	noecho();

	//show start screen
	move(0,0);
	start_screen();
	refresh();
	while(!(getch() == '\n')){
		//while loop untill press 's'
	}

	
	//show play type select screen
	move(0,0);
	clear();
	multiFlag = playTypeScreen();

	if (multiFlag != 1)		// 멀티 플레이면 소켓 연결
			sockfd = connectSocket(multiFlag, PORTNUM);	

	//show select screen
	move(0,0);
	clear();

	// 플레이 타입에 따라 동작 분기 (1 = 싱글, 2 = 멀티(host), 3 = 멀티(guest))
	if (multiFlag == 1)
		select_screen();
	else if(multiFlag == 2)
	{
		select_screen();
		write(sockfd, &index_toPlay, sizeof(index_toPlay));
		pthread_create(&readThd, NULL, reader, (void*)&sockfd);	// 멀티플레이에서 상대의 점수 입력받음
	}
	else
	{
		clear();
		move(5, 5);
		printw("호스트가 노래를 선택하고 있습니다...");
		refresh();
		read(sockfd, &index_toPlay, sizeof(index_toPlay));
		pthread_create(&readThd, NULL, reader, (void*)&sockfd);	// 멀티플레이에서 상대의 점수 입력받음
	}

	//start music and show game screen
	move(0,0);
	clear();
	playMusic(index_toPlay);
	draw_stage(floor_state);
	refresh();
	
	// thread로 입력과 출력 분리
	pthread_create(&nd, NULL, note_drop, (void*)&multiFlag);
	note_update(sockfd);
	
	endwin();

	return 0;
}
