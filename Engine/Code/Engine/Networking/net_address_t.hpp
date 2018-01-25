#pragma once;
#include "Engine/Networking/NetSystem.hpp"
#include <string>
#include <vector>






struct net_address_t
{
	unsigned int address;
	uint16_t port;
};

std::string NetAddressToString(net_address_t &addr);
std::string SocketAddressToString(struct sockaddr * ai_addr);
bool NetAddressFromSocketAddress(net_address_t* out, struct sockaddr * ai_addr);
std::vector<net_address_t> GetAddressFromHostName(char const *hostname, uint16_t port, bool bindable);
bool SockAddressFromNetAddress(sockaddr* out, int* out_size, net_address_t const &addr);
