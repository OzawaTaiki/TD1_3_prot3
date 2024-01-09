#include <stdio.h>
#include "Phill.h"

#pragma comment(lib, "ws2_32.lib")

int Phill::InitWSA(WSADATA* _wsaData)
{
	int errorCode = WSAStartup(WINSOCK_VERSION, _wsaData);
	if (errorCode != 0)
	{
		return 1;
	}
	return errorCode;
}

SOCKET Phill::InitSocket(sockaddr_in* _addr, u_short _port)
{
	SOCKET resultSoc;
	_addr->sin_family = AF_INET;
	_addr->sin_port = htons(_port);
	// any ip address will be suru
	_addr->sin_addr.S_un.S_addr = INADDR_ANY;
	resultSoc = socket(_addr->sin_family, SOCK_STREAM, 0);
	if (resultSoc == INVALID_SOCKET)
	{
		return SOCKET();
	}
	return resultSoc;
}

int Phill::BindAddress(SOCKET* _socket, sockaddr* addr)
{
	int errorcode = 0;
	errorcode = bind(*_socket, addr, sizeof(*addr));
	if (errorcode == SOCKET_ERROR)
	{
		return 1;
	}
	return 0;
}

int Phill::Listen(SOCKET* _socket, int _backlog)
{
	if (listen(*_socket, _backlog) == SOCKET_ERROR)
	{
		return 1;
	}
	return 0;
}

SOCKET Phill::WaitAccept(SOCKET* _listen, sockaddr* _client, int* _clientLen)
{
	return accept(*_listen, _client, _clientLen);
}
