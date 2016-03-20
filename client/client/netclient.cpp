#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

void ErrorHandling(char* message);

int main(int argc, char* argv[])
{
	WSADATA wsaData;
	SOCKET hSocket;
	SOCKADDR_IN serverAddr;

	char message[100];
	char message1[4] = "bye";
	int strLen;
	int result;

	if (argc != 3) //언래 기본값 1 + 2개입력하니까 3개
	{
		ErrorHandling("usage error");
	}

	result = WSAStartup(MAKEWORD(2, 2), &wsaData); //소켓프로그래밍할때 기본적으로 해줘야하는것
	if (result != 0)
	{
		ErrorHandling("WSAStartup() error");
	}

	hSocket = socket(PF_INET, SOCK_STREAM, 0); //ipv4 , tcp 소켓 생성
	if (hSocket == INVALID_SOCKET)
	{
		ErrorHandling("socket() error");
	}

	memset(&serverAddr, 0, sizeof(serverAddr)); //덮어쓸메모리주소, 덮어쓸 값,덮어쓸 사이즈
	serverAddr.sin_family = AF_INET; //ipv4
	serverAddr.sin_addr.S_un.S_addr = inet_addr(argv[1]); //ip
	serverAddr.sin_port = htons(atoi(argv[2])); //포트 atoint 정수형으로 바꿈

	result = connect(hSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)); //서버로의 연결
	if (result == INVALID_SOCKET) 
	{
		ErrorHandling("connect() error");
	}

	printf("connected!\n");

	while (1)
	{
		printf("보낼 메세지를 입력하세요:");
		scanf("%s", message);


		if ( strcmp(message, message1)==0 )
		{
			break;
		}


		strLen = send(hSocket, message, strlen(message) + 1, 0);
		
		if (strLen == -1)
		{
			ErrorHandling("recv() error");
		}

	}
	
	printf("Dissconnected\n");
	closesocket(hSocket);
	WSACleanup();

	system("pause");
	return 0;
}

void ErrorHandling(char* message)
{
	fputs(message, stdout);
	fputc('\n', stdout);
	system("pause"); //프로그램이 종료되도 창이 안닫힘
	exit(1);
}