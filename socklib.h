#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <time.h>
#include <string.h>
#include <ncursesw/curses.h>
#include <stdlib.h>
#include <ctype.h>

int make_server_socket(int);
int make_server_socket_q(int, int);
int connect_to_server(char*, int);
int connectSocket(int, int);