#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

void ErrorHandling(char* message);


int main(int argc, char* argv[])
{
	WSADATA wsaData;
	SOCKET Socket;
	SOCKADDR_IN ServerAddr;
	int timeout = 1000;
	char message[100];
	int result;

	if (argc != 3)
	{
		ErrorHandling("usage error");
	}

	result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (result != 0)
	{
		ErrorHandling("WSAStartup() error");
	}

	Socket = socket(PF_INET, SOCK_STREAM, 0);
	if (Socket == INVALID_SOCKET)
	{
		ErrorHandling("socket() error");
	}

	memset(&ServerAddr, 0, sizeof(ServerAddr));
	ServerAddr.sin_family = AF_INET;
	ServerAddr.sin_addr.S_un.S_addr = inet_addr(argv[1]);
	ServerAddr.sin_port = htons(atoi(argv[2]));

	result = connect(Socket, (SOCKADDR*)&ServerAddr, sizeof(ServerAddr));
	if (result == INVALID_SOCKET)
	{
		ErrorHandling("connect error");
	}

	printf("connected!\n");

	while (1)
	{
		printf("보낼 메세지를 입력하세요. : ");
		scanf("%s", message);

		if (strcmp(message, "quit") == 0)
		{
			break;
		}
		result = send(Socket, message, strlen(message) + 1, 0);
		setsockopt(Socket, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout));
		result = recv(Socket, message, 100, 0);
		if (result > 0)
		{
			printf("서버가 '%s'를 수신했습니다.\n", message);
		}
	}

	printf("Dissconnected!\n");
	closesocket(Socket);
	WSACleanup();

	system("pause");
	return 0;
}

void ErrorHandling(char* message)
{
	fputs(message, stdout);
	fputc('\n', stdout);
	system("pause");
	exit(1);
}