#include "Engine/Networking/net_address_t.hpp"


std::string NetAddressToString(net_address_t &addr)
{
	unsigned int i = addr.address;
	unsigned char e[4];
	e[0] = i & 0xFF;
	e[1] = (i >> 8) & 0xFF;
	e[2] = (i >> 16) & 0xFF;
	e[3] = (i >> 24) & 0xFF;
	std::string string_addr2 = std::to_string(e[3]) + "." + std::to_string(e[2]) + "." + std::to_string(e[1]) + "." + std::to_string(e[0]) + ".";
	
	
	
	unsigned char* arr = (unsigned char*)&addr.address;

	unsigned char a = arr[0];
	unsigned char b = arr[1];
	unsigned char c = arr[2];
	unsigned char d = arr[3];

	std::string string_addr = std::to_string(d) + "." + std::to_string(c) + "." + std::to_string(b) + "." + std::to_string(a) + ".";
	
	return string_addr2;
}


std::string SocketAddressToString(struct sockaddr * ai_addr)
{
	return inet_ntoa((((struct sockaddr_in*)ai_addr)->sin_addr));
}

bool NetAddressFromSocketAddress(net_address_t* out, struct sockaddr * ai_addr)
{
	if (nullptr == ai_addr)
		return false;

	if (ai_addr->sa_family != AF_INET)
		return false;

	sockaddr_in* ipv4_addresss = (sockaddr_in*)ai_addr;

	out->port = ntohs(ipv4_addresss->sin_port);
	out->address = ntohl(ipv4_addresss->sin_addr.S_un.S_addr);
	return true;
}

std::vector<net_address_t> GetAddressFromHostName(char const *hostname, uint16_t port, bool bindable)
{
	char service[16];
	sprintf_s(service, "%u", port);

	addrinfo hints;
	memset(&hints, 0, sizeof(hints));

	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM; //udp = SOCK_DGRAM

	hints.ai_flags |= (bindable ? AI_PASSIVE : 0);
	//hints.ai_flags |= AI_NUMBERICHOST; //Used for when youre only using numeric IP's

	addrinfo *results = nullptr;
	int status = ::getaddrinfo(hostname, service, &hints, &results);

	std::vector<net_address_t> addresses;
	if (status != 0)
	{
		return addresses;
	}

	addrinfo *addr = results;
	while (nullptr != addr)
	{
		net_address_t netAddress;

		if (NetAddressFromSocketAddress(&netAddress, addr->ai_addr))
		{
			addresses.push_back(netAddress);
		}

		addr = addr->ai_next;
	}

	::freeaddrinfo(addr);
	return addresses;
}

bool SockAddressFromNetAddress(sockaddr * out, int* out_size, net_address_t const &addr)
{
	sockaddr_in *out_addr = (sockaddr_in*)out;
	memset(out_addr, 0, sizeof(sockaddr_in));

	out_addr->sin_family = AF_INET;
	out_addr->sin_port = htons(addr.port);
	out_addr->sin_addr.S_un.S_addr = htonl(addr.address);

	*out_size = sizeof(sockaddr_in);
	return true;
}