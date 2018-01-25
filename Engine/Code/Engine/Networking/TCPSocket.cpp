#include "Engine/Networking/TCPSocket.hpp"

TCPSocket::TCPSocket()
	: my_socket(INVALID_SOCKET),
	m_isListenSocket(false)
{
}
TCPSocket::~TCPSocket()
{
	close();
}


//Client
bool TCPSocket::join(net_address_t address)
{
	/*
	if (is_valid())
	{
		return false;
	}
	*/
	SOCKET sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock == INVALID_SOCKET)
		return false;

	sockaddr_storage internal_address;

	int addr_size;
	SockAddressFromNetAddress((sockaddr*)&internal_address, &addr_size, address);

	int result = ::connect(sock, (sockaddr*)&internal_address, sizeof(sockaddr_in));

	if (result != 0)
	{
		int error = WSAGetLastError();
		error;
		::closesocket(sock);
		return false;
	}
	else
	{
		//address = internal_address;
		my_socket = sock;
		return true;
	}
}

bool TCPSocket::is_valid() const
{
	return (my_socket != INVALID_SOCKET);
}


//Both
unsigned int TCPSocket::Send(void const *payload, unsigned int payloadSizeInBytes)
{
	if (!is_valid())
		return 0;

	if (payloadSizeInBytes == 0)
		return 0;

	int bytes_sent = ::send(my_socket, (char const*)payload, (int)payloadSizeInBytes, 0);

	int error = WSAGetLastError();

	if (bytes_sent <= 0)
	{
		//int error = WSAGetLastError();
		if (error == WSAEWOULDBLOCK)
			return 0;
		else
		{
			printf("*TCPSocket::send failed. Error: %u\n", error);
			close();
			return 0;
		}
	}

	return bytes_sent;
}

unsigned int TCPSocket::receive(void* payload, int max_size)
{
	if (!is_valid() || (max_size == 0))
		return 0;

	int bytes_read = ::recv(my_socket, (char*)payload, (int)max_size, 0);

	if (bytes_read <= 0)
	{
		if (bytes_read == SOCKET_ERROR)
		{
			int error = WSAGetLastError();

			if (error == WSAEWOULDBLOCK)
				return 0;
			else
			{
				printf("*TCPSocket::send failed. Error: %u\n", error);
				close();
				return 0;
			}
		}
		else
		{
			return 0;
		}
	}
	else
		return (unsigned int)bytes_read;
}

//Hosting
bool TCPSocket::Listen(uint16_t port)
{
	if (is_valid())
		return false;

	std::vector<net_address_t> addresses = GetAddressFromHostName("", port, true);

	if (addresses.size() == 0)
		return false;

	SOCKET listen_socket = ::socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (listen_socket == INVALID_SOCKET)
		return false;

	sockaddr_storage listen_address;
	unsigned int addr_size = 0;
	SockAddressFromNetAddress((sockaddr*)&listen_address, (int*)&addr_size, addresses[0]);

	//associate address to this socket
	int result = ::bind(listen_socket, (sockaddr*)&listen_address, (int)addr_size);
	if (result == SOCKET_ERROR)
	{
		int errorcode = WSAGetLastError();
		errorcode;
		::closesocket(listen_socket);
		return false;
	}

	int max_queued = 8;
	result = ::listen(listen_socket, (int)max_queued);



	my_socket = listen_socket;
	address = addresses[0];
	m_isListenSocket = true;

	return true;
}

bool TCPSocket::IsListening() const
{
	return m_isListenSocket;
}

TCPSocket* TCPSocket::Accept()
{
	if (!IsListening())
		return nullptr;

	sockaddr_storage conn_addr;
	int conn_addr_len = sizeof(conn_addr);

	SOCKET their_socket = ::accept(my_socket, (sockaddr*)&conn_addr, &conn_addr_len);

	if (their_socket == INVALID_SOCKET)
		return nullptr;

	net_address_t net_addr;

	if (!NetAddressFromSocketAddress(&net_addr, (sockaddr*)&conn_addr))
	{
		::closesocket(their_socket);
		return nullptr;
	}

	TCPSocket *theirTcpSocket = new TCPSocket();

	theirTcpSocket->my_socket = their_socket;
	theirTcpSocket->address = net_addr;

	return theirTcpSocket;
}

bool TCPSocket::CheckForDisconnect()
{
	if (!is_valid())
		return false;

	WSAPOLLFD fd;
	fd.fd = my_socket;
	fd.events = POLLRDNORM;

	if (SOCKET_ERROR == WSAPoll(&fd, 1, 0))
		return false;

	if ((fd.revents & POLLHUP) != 0)
	{
		close();
		return true;
	}
	return false;
}

void TCPSocket::SetBlocking(bool blocking)
{
	if (!is_valid())
		return;

	u_long non_blocking = blocking ? 0 : 1;
	::ioctlsocket(my_socket, FIONBIO, &non_blocking);

}


void TCPSocket::close()
{
	if (is_valid())
	{
		::closesocket(my_socket);
		my_socket = INVALID_SOCKET;
	}
}