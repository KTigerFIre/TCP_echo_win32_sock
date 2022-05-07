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
	SOCKET serverSocket, clientSocket;
	SOCKADDR_IN serverAddress, clientAddress;

	int serverPort = 9876;
	char received[256];

	// init winsock
	// MAKEWORD(2, 2) 2.2버전을 사용할것을 명시
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		ShowErrMsg("WSAStartup()");
	}

	// generate TCP socket

	// PF_INET ipv4를 사용할것을 명시, SOCK_STREAM는 TCP를 사용할것을 명시
	serverSocket = socket(PF_INET, SOCK_STREAM, 0);

	if (serverSocket == INVALID_SOCKET)
	{
		ShowErrMsg("socket()");
	}

	memset(&serverAddress, 0, sizeof(serverAddress));

	serverAddress.sin_family = AF_INET;
	//4바이트 정수를 네트워크 바이트 형식으로
	serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	serverAddress.sin_port = htons(serverPort);

	if (bind(serverSocket, (SOCKADDR*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR)
	{
		ShowErrMsg("bind()");
	}
	std::cout << "[State] bind()" << std::endl;

	if (listen(serverSocket, 5) == SOCKET_ERROR)
	{
		ShowErrMsg("listen()");
	}
	std::cout << "[State] listen()" << std::endl;


	int sizeClientAddress = sizeof(clientAddress);
	clientSocket = accept(serverSocket, (SOCKADDR*)&clientAddress, &sizeClientAddress);
	if (clientSocket == INVALID_SOCKET)
	{
		ShowErrMsg("accept()");
	}
	std::cout << "[State] accept" << std::endl;

	while (1)
	{
		int len = recv(clientSocket, received, sizeof(received), 0);
		received[len] = NULL;
		std::cout << "[Client msg]:" << received << std::endl;
		std::cout << "[Sending msg]:" << received << std::endl;
		if (strcmp(received, "[exit]") == 0)
		{
			send(clientSocket, received, sizeof(received) - 1, 0);
			std::cout << "[Server exit]" << std::endl;
			break;
		}
		send(clientSocket, received, sizeof(received) - 1, 0);
	}

	closesocket(clientSocket);
	closesocket(serverSocket);
	WSACleanup();
	system("pause");

	return 0;
}