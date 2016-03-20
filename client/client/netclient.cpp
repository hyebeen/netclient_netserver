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

	if (argc != 3) //�� �⺻�� 1 + 2���Է��ϴϱ� 3��
	{
		ErrorHandling("usage error");
	}

	result = WSAStartup(MAKEWORD(2, 2), &wsaData); //�������α׷����Ҷ� �⺻������ ������ϴ°�
	if (result != 0)
	{
		ErrorHandling("WSAStartup() error");
	}

	hSocket = socket(PF_INET, SOCK_STREAM, 0); //ipv4 , tcp ���� ����
	if (hSocket == INVALID_SOCKET)
	{
		ErrorHandling("socket() error");
	}

	memset(&serverAddr, 0, sizeof(serverAddr)); //����޸��ּ�, ��� ��,��� ������
	serverAddr.sin_family = AF_INET; //ipv4
	serverAddr.sin_addr.S_un.S_addr = inet_addr(argv[1]); //ip
	serverAddr.sin_port = htons(atoi(argv[2])); //��Ʈ atoint ���������� �ٲ�

	result = connect(hSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)); //�������� ����
	if (result == INVALID_SOCKET) 
	{
		ErrorHandling("connect() error");
	}

	printf("connected!\n");

	while (1)
	{
		printf("���� �޼����� �Է��ϼ���:");
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
	system("pause"); //���α׷��� ����ǵ� â�� �ȴ���
	exit(1);
}