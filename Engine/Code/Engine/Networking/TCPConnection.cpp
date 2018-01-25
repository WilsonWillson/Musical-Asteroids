#include "Engine/Networking/TCPConnection.hpp"
#include "Engine/Networking/TCPSocket.hpp"
#include "Engine/Networking/NetMessage.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"

TCPConnection::TCPConnection()
{
	socket = new TCPSocket();
	BytesReadSoFar = 0;

}

TCPConnection::~TCPConnection()
{

}

void TCPConnection::send(NetMessage *msg)
{
	socket->Send(msg->payload, msg->payload_bytes_used);
}

bool TCPConnection::receive(NetMessage **msg)
{
	if (BytesReadSoFar < 2)
	{
		BytesReadSoFar += socket->receive(&bufferMessage[BytesReadSoFar], 2 - BytesReadSoFar);
	}

	if (BytesReadSoFar < 2)
		return false;

	uint16_t  bytesToRead =*(uint16_t*)&bufferMessage[0]; //Only works for little edian


	if (BytesReadSoFar < bytesToRead + 2)
	{
		BytesReadSoFar += socket->receive(&bufferMessage[BytesReadSoFar], (bytesToRead + 2) - BytesReadSoFar);
	}


	if (BytesReadSoFar < bytesToRead + 2)
		return false;

	//ReadMessageFromBuffer();

	NetMessage* newMessage = new NetMessage();

	newMessage->message_type_index = bufferMessage[2];
	newMessage->payload_bytes_used = bytesToRead - 1;
	
	std::memcpy(newMessage->payload, &bufferMessage[3], newMessage->payload_bytes_used);

	*msg = newMessage;

	//msg->payload = msgg->payload;
	BytesReadSoFar = 0;

	return true;
}

bool TCPConnection::connect()
{
	return socket->join(address);
}

bool TCPConnection::is_disconnected()
{
	if (socket == nullptr)
		return true;

	return socket->CheckForDisconnect();
}
