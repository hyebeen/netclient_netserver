#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <thread>

#pragma comment(lib, "ws2_32.lib")

void Error(char* message);
void receive(SOCKET ClientSock, bool echo);

int main(int argc, char* argv[])
{
	WSADATA wsaData;
	SOCKET ServerSock, ClientSock;
	SOCKADDR_IN ServerAddr;
	int result;
	bool echo = false;

	if ((argc == 3) || (argc == 2))
	{
		printf("입력한 값 : %s   %s \n", argv[1], argv[2]);
	}
	else
		Error("usage error");

	if (argc == 3 && strcmp(argv[2], "-echo") == 0)
	{
		echo = true;
	}

	result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (result != 0)
	{
		Error("WSAStartup error");
	}

	ServerSock = socket(PF_INET, SOCK_STREAM, 0);
	if (ServerSock == INVALID_SOCKET)
	{
		Error("socket error");
	}

	memset(&ServerAddr, 0, sizeof(ServerAddr));
	ServerAddr.sin_family = AF_INET;
	ServerAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	ServerAddr.sin_port = htons(atoi(argv[1]));

	result = bind(ServerSock, (SOCKADDR*)&ServerAddr, sizeof(ServerAddr));
	if (result == SOCKET_ERROR)
	{
		Error("bind error");
	}

	result = listen(ServerSock, SOMAXCONN);
	if (result == SOCKET_ERROR)
	{
		Error("listen error");
	}

	while (1) {
		ClientSock = accept(ServerSock, NULL, NULL);
		if (ClientSock == INVALID_SOCKET)
		{  
			printf("accept error\n");
			continue; 
		}
		std::thread(&receive, ClientSock, echo).detach();
	}
	
	closesocket(ServerSock);
	WSACleanup();

	return 0;
}

void receive(SOCKET ClientSock, bool echo)
{
	char message[100];
	char message1[100];
	int result;

	while (1)
	{
		result = recv(ClientSock, message, 100, 0);
		if (result <= 0)  
		{
			break;
		}
		else
		{
			printf("clinet -> %s\n", message);
		}
		if (echo)
		{
			send(ClientSock, message, strlen(message) + 1, 0);
		}
	} 
	closesocket(ClientSock);

	return;
}

void Error(char* message)
{
	puts(message);
	exit(1);
}