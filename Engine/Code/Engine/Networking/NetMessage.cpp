#include "Engine/Networking/NetMessage.hpp"
#include <string>
#include <vector>

NetMessage::NetMessage()
{
	payload_bytes_used = 0;
}

NetMessage::NetMessage(uint8_t type_index)
{
	message_type_index = type_index;
	payload_bytes_used = 0;
}

unsigned int NetMessage::write_string(std::vector<std::string> command, unsigned int test)
{
	uint16_t stringSize = 0;
	for (unsigned int i = 0; i < command.size(); i++)
	{
		std::string word = command[i];
		stringSize += word.size();

		if(i + 1< command.size())
			stringSize += 1;
	}
	

	uint16_t payloadSize = stringSize + 3;

	write_byte(payloadSize & 0xff);
	write_byte(payloadSize & 0xff00);
	write_byte(message_type_index);
	write_byte(stringSize & 0xff);
	write_byte(stringSize & 0xff00);


	for (unsigned int j = 0; j < command.size(); j++)
	{
		uint16_t wordSize = command[j].size();
		for (unsigned int i = 0; i < wordSize; i++)
		{
			write_byte((uint8_t)command[j][i]);
		}

		if (j + 1 < command.size())
			write_byte(' ');
	}

	return test;
}

void NetMessage::write_byte(uint8_t byte)
{
	payload[payload_bytes_used] = byte;
	payload_bytes_used++;
}

std::string NetMessage::readString()
{
	std::string info;
	info.resize(payload_bytes_used - 2);
	for (unsigned int i = 2; i < payload_bytes_used; i++)
	{
		char letter = payload[i];
		info[i -2] = letter;
	}



	return info;
}
