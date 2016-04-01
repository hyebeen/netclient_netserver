#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <thread>

#pragma comment(lib, "ws2_32.lib")

void Error(char* message);
void _receive(SOCKET Socket, char maeesage[100]);
void _send(SOCKET Socket, char message[100]);

bool done=false;

int main(int argc, char* argv[])
{
	WSADATA wsaData;
	SOCKET Socket;
	SOCKADDR_IN ServerAddr;
	char message[100];
	int result;

	if (argc != 3)
	{
		Error("usage error");
	}

	result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (result != 0)
	{
		Error("WSAStartup error");
	}

	Socket = socket(PF_INET, SOCK_STREAM, 0);
	if (Socket == INVALID_SOCKET)
	{
		Error("socket error");
	}

	memset(&ServerAddr, 0, sizeof(ServerAddr));
	ServerAddr.sin_family = AF_INET;
	ServerAddr.sin_addr.S_un.S_addr = inet_addr(argv[1]);
	ServerAddr.sin_port = htons(atoi(argv[2]));

	result = connect(Socket, (SOCKADDR*)&ServerAddr, sizeof(ServerAddr));
	if (result == INVALID_SOCKET)
	{
		Error("connect error");
	}

	printf("connected!\n");
	
	std::thread receive_thread(&_receive, Socket, message);
	std::thread send_thread(&_send, Socket, message);

	receive_thread.join();
	send_thread.join();

	printf("Dissconnected!\n");
	closesocket(Socket);
	WSACleanup();

	return 0;
}
void _send(SOCKET Socket, char message[100])
{	
	int result;
	while (1) {
		printf("보낼 메세지를 입력하세요. : ");
		scanf("%s", message);

		if (strcmp(message, "quit") == 0)
		{
			done = true;
			break;
		}
		result = send(Socket, message, strlen(message) + 1, 0);
		if (result != strlen(message)+1)
		{
			printf("message가 정상적으로 전송되지 않았습니다.");
		}
	}
}
void _receive(SOCKET Socket, char message[100])
{
	int result;
	int timeout = 1000;
	setsockopt(Socket, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout));
	while (done==false)
	{
		result = recv(Socket, message, 100, 0);
		if (result > 0)
		{
			printf("\n서버로부터 '%s'를 수신했습니다.\n", message);
		}
	}
}
void Error(char* message)
{
	puts(message);
	exit(1);
}