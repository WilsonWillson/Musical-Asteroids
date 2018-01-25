#pragma once
#include "Engine/Core/FileBinaryStream.hpp"
#include <stdint.h>
#include <string>
#include <vector>

class NetConnection;

class NetMessage : public FileBinaryStream // or ByteStream - some sort of writer;
{
public:
	NetMessage();
	NetMessage(uint8_t type_index);

	unsigned int write_string(std::vector<std::string> command, unsigned int test);
	void write_byte(uint8_t byte);
	std::string readString();


public:
	uint8_t message_type_index;
	NetConnection *sender;

	byte_t payload[1000];
	uint8_t index;
	unsigned int payload_bytes_used;

};
