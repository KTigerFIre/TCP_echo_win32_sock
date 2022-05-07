#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <string>
#include <winsock2.h>

void ShowErrMsg(const std::string& msg)
{
	std::cout << "[ERROR]: " << msg << std::endl;
	system("pause");
	exit(1);
}

int main()
{
	WSADATA wsaData;
	SOCKET clientSocket;
	SOCKADDR_IN serverAddress;

	int serverPort = 9876;
	char received[256];
	std::string sent;

	// init winsock
	// MAKEWORD(2, 2) 2.2버전을 사용할것을 명시
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		ShowErrMsg("WSAStartup()");
	}

	// generate TCP socket
	// PF_INET ipv4를 사용할것을 명시, SOCK_STREAM는 TCP를 사용할것을 명시
	clientSocket = socket(PF_INET, SOCK_STREAM, 0);

	if (clientSocket == INVALID_SOCKET)
	{
		ShowErrMsg("socket()");
	}

	memset(&serverAddress, 0, sizeof(serverAddress));


	serverAddress.sin_family = AF_INET;
	//4바이트 정수를 네트워크 바이트 형식으로
	serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
	serverAddress.sin_port = htons(serverPort);

	if (connect(clientSocket, (SOCKADDR*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR)
	{
		ShowErrMsg("connect()");
	}

	std::cout << "[State] connect " << std::endl;
	while (1)
	{
		std::cout << "[Send msg]: ";
		std::getline(std::cin, sent);
		if (sent == "") continue;
		send(clientSocket, sent.c_str(), sent.length(), 0);
		int length = recv(clientSocket, received, sizeof(received), 0);
		received[length] = '\0';

		if (strcmp(received, "[exit]") == 0)
		{
			std::cout << "[Server exit]" << std::endl;
			break;
		}

		std::cout << "[Server msg]: " << received << std::endl;
	}

	closesocket(clientSocket);
	WSACleanup();
	system("pause");

	return 0;
}