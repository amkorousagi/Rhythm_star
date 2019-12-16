/*
	socklib.c
	--------------------------------
	멀티 플레이를 위한 socket 함수들을 정의한 파일
	connectSocket을 제외한 함수들은 수업시간에 사용한 코드를 변형한 함수
*/

#include "socklib.h"

#define BACKLOG 1

/*
	Function : make_server_socket
	---------------------------------
	서버의 소켓을 생성 및 리턴하는 함수
	Queue의 크기는 BACKLOG로 정의
*/
int make_server_socket(int portnum){
    return make_server_socket_q(portnum, BACKLOG);
}

/*
	Function : make_server_socket
	---------------------------------
	서버의 소켓을 생성 및 리턴하는 함수
	Queue의 크기는 BACKLOG로 정의
*/
int make_server_socket_q(int portnum, int backlog)
{
    struct sockaddr_in  saddr;
    int sock_id;

    sock_id = socket(PF_INET, SOCK_STREAM, 0);

    if (sock_id == -1)  return -1;

    bzero((void*)&saddr, sizeof(saddr));
    saddr.sin_addr.s_addr = htonl(INADDR_ANY);
    saddr.sin_port = htons(portnum);
    saddr.sin_family = AF_INET;

    if (bind(sock_id, (struct sockaddr*)&saddr, sizeof(saddr)) != 0)
        return -1;
    
    if (listen(sock_id, backlog) != 0)
        return -1;
    return sock_id;
}

/*
	Function : connect_to_server
	---------------------------------
	Client에 해당하는 유저가 서버에 접속하기 위한 함수
	서버의 ip, port를 전달하면, 해당하는 소켓을 생성 및 리턴
*/
int connect_to_server(char *ip, int portnum)
{
    int sock;
    struct sockaddr_in  servadd;

    sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock == -1) return -1;

    bzero(&servadd, sizeof(servadd));
    servadd.sin_addr.s_addr = inet_addr(ip);
    servadd.sin_port = htons(portnum);
    servadd.sin_family = AF_INET;
    if (connect(sock, (struct sockaddr*)&servadd, sizeof(servadd)) != 0)
        return -1;
    return sock;
}

/*
	Function : connectSocket
	---------------------------------
	서버의 Host와 Client의 동작을 정의한 함수
*/
int connectSocket(int multiFlag, int portNum){
	int sockid, sockfd;
	char serverIp[20], serverPortStr[20];
	int serverPort;
	int c, i;

	serverIp[0] = serverPortStr[0] = '\0';

	if (multiFlag == 2){	// make server
		sockid = make_server_socket(portNum);		
		clear();	
		move(5, 5);	
		printw("Waiting for players...");
		refresh();
		sockfd = accept(sockid, NULL, NULL);

		if (sockfd == -1){
			endwin();
			fprintf(stderr, "Socket Error!!\n");
			exit(1);
		}		
	}
	else{					// connect to server
		clear();
		move(5, 5);
		printw("Input server ip and port\n");
		printw("   IP :\n");
		printw("   Port :\n");
		refresh();
		
		// ip 입력받음
		i = 0;
		move(6, 8);
		while((c = getch()) != '\n'){
			if (i < 20 && (isdigit(c) || c == '.'))	{
				serverIp[i++] = c;
				serverIp[i] = '\0';
			}
			else if((c == 127 || c == '\b') && i > 0)
				serverIp[--i] = '\0';
			
			move(6, 8);
			printw("%20s", "");
			move(6, 8);
			printw("%s", serverIp);
			refresh();
		}
		serverIp[i] = '\0';

		// port 입력받음
		i = 0;
		move(7, 10);
		while((c = getch()) != '\n'){
			if (i < 20 && isdigit(c))	{
				serverPortStr[i++] = c;
				serverPortStr[i] = '\0';
			}
			else if((c == 127 || c == '\b') && i > 0)
				serverPortStr[--i] = '\0';

			move(7, 10);
			printw("%20s", "");
			move(7, 10);
			printw("%s", serverPortStr);
			refresh();
		}
		serverPortStr[i] = '\0';
		serverPort = atoi(serverPortStr);

		sockfd = connect_to_server(serverIp, serverPort);	// 소켓 생성

		if (sockfd == -1){
			endwin();
			fprintf(stderr, "Socket Error!!\n");
			exit(1);
		}
	}
	return sockfd;
}
