#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

void ErrorHandling(char* message);

int main(int argc, char* argv[])
{
	WSADATA wsaData;
	SOCKET hSocket;
	SOCKADDR_IN serverAddr, clientAddr;

	char message[100];
	char message1[4] = "bye";
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

	hSocket = socket(PF_INET, SOCK_STREAM, 0);
	if (hSocket == INVALID_SOCKET)
	{
		ErrorHandling("socket() error");
	}

	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.S_un.S_addr = inet_addr(argv[1]);
	serverAddr.sin_port = htons(atoi(argv[2]));

	result = connect(hSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr));
	if (result == INVALID_SOCKET)
	{
		ErrorHandling("connect error");
	}

	printf("connected!\n");

	while (1)
	{
		printf("보낼 메세지를 입력하세요. : ");
		scanf("%s", message);

		if (strcmp(message, message1) == 0)
		{
			break;
		}
		result = send(hSocket, message, strlen(message) + 1, 0);

		result = recv(hSocket, message, 100, 0);
			if (result > 0)
			{
				printf("서버가 '%s'를 수신했습니다.\n", message);
			}
	}

	printf("Dissconnected!\n");
	closesocket(hSocket);
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