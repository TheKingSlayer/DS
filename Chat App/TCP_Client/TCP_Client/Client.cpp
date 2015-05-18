//JSR

#define _WINSOCK_DEPRECATED_NO_WARNINGS
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
		return false;

	if (LOBYTE(wsadata.wVersion) != 2 || HIBYTE(wsadata.wVersion) != 0)
		return false;

	return true;
}

int main(int argc, char**argv)
{

		char*			serverPort, messagetosend[BUFFER_LEN+1], messagereceived[BUFFER_LEN+1];
		SOCKET			sock;
		char*			serverIp;
		struct addrinfo hintsAddrInfo, *listAddrInfo = NULL, *iterator = NULL;
		int				wserror, connectCheck, bytesSent;
		SOCKADDR_IN		target;


	serverIp	= argv[1];
	serverPort	= argv[2];

	if (!Initialize())
		return (ERROR_CODE);

	//	memset(&hintsAddrInfo, 0, sizeof(hintsAddrInfo));
	//	hintsAddrInfo.ai_family		= AF_INET;
	//	hintsAddrInfo.ai_socktype	= SOCK_STREAM;
	//	hintsAddrInfo.ai_flags		= AI_PASSIVE;
	//
	//	if (getaddrinfo(serverIp, serverPort, &hintsAddrInfo, &listAddrInfo) != 0) 
	//		return 1;
	//
	//	iterator = listAddrInfo;
	//	while (iterator)
	//	{
	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if ( sock == INVALID_SOCKET)
	{
		wserror = WSAGetLastError();
		return 1;
	}

	target.sin_family		= AF_INET;
	target.sin_port			= htons(5000);
	target.sin_addr.s_addr	= inet_addr("10.20.19.13");

	connectCheck = connect(sock, (SOCKADDR*)&target, sizeof(target));

	if (connectCheck == SOCKET_ERROR)
	{
		return 1;
	}

	while (1)
	{
		memset(messagetosend, 0, sizeof(char)*(BUFFER_LEN + 1));

		printf(">");
		scanf("%s", &messagetosend);

		bytesSent	= send(sock, messagetosend, (int)strlen(messagetosend), 0);
		if (bytesSent == SOCKET_ERROR) 
		{
			printf("Send failed with error: %d\n", WSAGetLastError());
			return 1;
		}

		if (_stricmp(messagetosend, "QUIT") == 0)
			break;

		memset(messagereceived, 0, sizeof(char)*(BUFFER_LEN + 1));
		recv(sock, messagereceived, BUFFER_LEN, 0);
		printf("%s\n",messagereceived);

		if (_stricmp(messagereceived, "QUIT") == 0)
			break;

	}

	
	closesocket(sock);
//		iterator = iterator->ai_next;
//	}

	if (!iterator) return 1;
	freeaddrinfo(listAddrInfo);


	return 0;
}