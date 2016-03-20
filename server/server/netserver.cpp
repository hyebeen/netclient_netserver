#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

void ErrorHandling(char* message);

int main(int argc, char* argv[])
{
	WSADATA wsaData;
	SOCKET hServerSock, hClientSock;
	SOCKADDR_IN serverAddr, clientAddr;

	char message[100];
	char message1[100];
	int sizeClientAddr;
	int result;

	if (argc != 3)
	{
		ErrorHandling("usage error");
	}

	result = WSAStartup(MAKEWORD(2, 2), &wsaData); //기본적으로 해야하는거
	if (result != 0)
	{
		ErrorHandling("WSAStartup() error");
	}

	hServerSock = socket(PF_INET, SOCK_STREAM, 0); //소켓만듬
	if (hServerSock == INVALID_SOCKET)
	{
		ErrorHandling("socket() error");
	}

	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	serverAddr.sin_port = htons(atoi(argv[1]));

	result = bind(hServerSock, (SOCKADDR*)&serverAddr, sizeof(serverAddr));
	if (result == SOCKET_ERROR)
	{
		ErrorHandling("bind() error");
	}

	result = listen(hServerSock, 5); //오는거 기다림 입력이 동시에 몇개 올수있는지
	if (result == SOCKET_ERROR)
	{
		ErrorHandling("listen() error");
	}

	sizeClientAddr = sizeof(clientAddr);
	hClientSock = accept(hServerSock, (SOCKADDR*)&clientAddr, &sizeClientAddr);
	if (hClientSock == INVALID_SOCKET)
	{
		ErrorHandling("accept() error");
	}


	//메세지 받아서 화면에 출력
	while (1)
	{
		result = recv(hClientSock, message, 100, 0);
		if (result > 0)
		{
			printf("clinet -> %s\n", message);
		}
		else
			break;
		strcpy(message1, message);
		result = send(hClientSock, message1, strlen(message1) + 1, 0);
	}

	printf("Dissconnected!\n");
	closesocket(hServerSock);
	closesocket(hClientSock);
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