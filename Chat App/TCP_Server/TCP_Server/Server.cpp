//JSR
#define _CRT_SECURE_NO_WARNINGS
#include<string.h>
#include<winsock2.h>
#include<WS2tcpip.h>
#include<stdio.h>

#pragma comment(lib,"ws2_32.lib")

#define ERROR_CODE	1
#define BUFFER_LEN	512

bool Initialize()
{
	WSADATA wsadata;

	if (WSAStartup(MAKEWORD(2, 0), &wsadata) != 0)
	{
		WSACleanup();
		return false;
	}
	if (LOBYTE(wsadata.wVersion) != 2 || HIBYTE(wsadata.wVersion) != 0)
	{
		WSACleanup();
		return false;
	}
	return true;
}


bool CreateBindSocket(SOCKET *pDestinationForSocket, char* pPortNo, int pSockType)
{
	struct addrinfo	hintsAddrinfo, *listAddress = NULL, *iterator = NULL;

	memset(&hintsAddrinfo, 0, sizeof(hintsAddrinfo));

	// TCP protocol with IPV4
	hintsAddrinfo.ai_family		= AF_INET;
	hintsAddrinfo.ai_socktype	= pSockType;
	hintsAddrinfo.ai_flags		= AI_PASSIVE;

	// Get server address info
	if (getaddrinfo(NULL, pPortNo, &hintsAddrinfo, &listAddress) != 0)
		return false;

	iterator = listAddress;
	while (iterator)
	{
		*pDestinationForSocket = socket(iterator->ai_family, iterator->ai_socktype, iterator->ai_protocol);
		if (*pDestinationForSocket == INVALID_SOCKET)
			continue;

		if (bind(*pDestinationForSocket, iterator->ai_addr, (int)iterator->ai_addrlen) == 0)
			// bind() success
			break;

		// Close socket if failed to bind
		closesocket(*pDestinationForSocket);
		iterator	= iterator->ai_next;
	}
	freeaddrinfo(listAddress);

	// failed to bind any address
	if (!iterator)
	{
		*pDestinationForSocket = 0;
		return false;
	}
	return true;
}


int main(int argc, char**argv)
{
		char	*ipAddress, *portNum, receivingBuffer[BUFFER_LEN + 1], sendingBuffer[BUFFER_LEN + 1];

		int		portNumber, quitLoop = 0;
		SOCKET	sock, acceptedSocket;

	// 2nd arg must be the port number
	if (argc == 2)
	{
		portNum = argv[1];
		sscanf_s(argv[1], "%d", &portNumber);
	}

	if (!Initialize())
		return (ERROR_CODE);

	if (!(CreateBindSocket(&sock, argv[1], SOCK_STREAM)))
	{
		WSACleanup();
		return (ERROR_CODE);
	}
	printf("Bound on port: %s \n", argv[1]);

	// Listen
	if (listen(sock, 2) == SOCKET_ERROR)
		return ERROR_CODE;
	printf("Listening on port: %s \n", argv[1]);

	// Keep waiting for a client to connect()
	while (1)
	{

		acceptedSocket = accept(sock, NULL, NULL);
		if (acceptedSocket == INVALID_SOCKET)
			continue;

		printf("Connected to a client\n");

		while (1)
		{
			quitLoop = 0;
			memset(receivingBuffer, 0, sizeof(char)*(BUFFER_LEN + 1));

			if (recv(acceptedSocket, receivingBuffer, BUFFER_LEN, 0) > 0)
			{
				if (_stricmp(receivingBuffer, "QUIT") == 0)
				{
					quitLoop = 1;
					break;
				}
			}

			printf("Received:%s\n>",receivingBuffer);
			memset(sendingBuffer, 0, sizeof(char)*(BUFFER_LEN + 1));
			scanf("%s", &sendingBuffer);
			if (send(acceptedSocket, sendingBuffer, strlen(sendingBuffer), 0) == SOCKET_ERROR)
			{
				printf("Error sending data\n");
				break;
			}

			if (_stricmp(sendingBuffer, "QUIT") == 0)
			{
				quitLoop = 1;
				break;
			}
		}
		if (quitLoop)
			break;
	}

	shutdown(sock, 2);
	closesocket(acceptedSocket);
	WSACleanup();
	return 0;
}