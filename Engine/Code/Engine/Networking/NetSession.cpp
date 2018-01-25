#include "Engine/Networking/NetSession.hpp"
#include "Engine/Networking/NetMessageDefinition.hpp"
#include "Engine/Networking/NetConnection.hpp"
#include "Engine/Networking/NetMessage.hpp"

NetSession::NetSession()
{

}

NetSession::~NetSession()
{

}

bool NetSession::register_message_definition(uint8_t msg_id, NetMessageDefinition &defn)
{

	for (unsigned int i = 0; i < message_defintions.size(); i++)
	{
		if (message_defintions[i]->type_index == msg_id)
			return false;
	}
	message_defintions.push_back(&defn);
	return true;
	
}

NetMessageDefinition* NetSession::get_message_definition(uint8_t id) const
{

	for (unsigned int i = 0; i < message_defintions.size(); i++)
	{
		if (message_defintions[i]->type_index == id)
			return message_defintions[i];
	}

	return nullptr;
}

uint8_t NetSession::get_free_connection_index() const
{
	unsigned int index = 0;
	for (index = 0; index < connections.size(); ++index) {
		if (connections[index] == nullptr) {
			return (uint8_t)index;
		}
	}

	if (index < max_connection_count) {
		return (uint8_t)index;
	}
	else {
		return INVALID_CONNECTION_INDEX;
	}

}

void NetSession::join_connection(uint8_t idx, NetConnection*conn)
{
	conn->connection_index = idx;

	//ASSERT_OR_DIE((idx >= connections.size()) || (connections[idx] == nullptr));

	if (idx >= connections.size()) {
		connections.resize(idx + 1);
	}

	connections[idx] = conn;
	//Append(connection_list, conn);
}

void NetSession::destroy_connection(NetConnection *cp)
{
	if (nullptr == cp) {
		return;
	}

	for (unsigned int i = 0; i < connections.size(); i++)
	{
		if (connections[i] == cp)
			connections.erase(connections.begin() + i);
	}


	if (my_connection == cp) {
		my_connection = nullptr;
	}

	if (host_connection == cp) {
		host_connection = nullptr;
	}

	/*
	if (cp->connection_index != INVALID_CONNECTION_INDEX) {
		connections[cp->connection_index] = nullptr;
		cp->connection_index = INVALID_CONNECTION_INDEX;
	}
*/


	delete cp;
}

NetConnection* NetSession::get_connection(uint8_t idx)
{
	if (idx < connections.size()) {
		return connections[idx];
	}

	return nullptr;
}

void NetSession::send_message_to_others(NetMessage const &msg)
{
	NetConnection *cp = connection_list;
	for (unsigned int i = 0; i < connections.size(); i++)
	{
		NetConnection *cp = connections[i];
		if (cp != my_connection)				
		{
			cp->send(new NetMessage(msg));
		}

	}
}

void NetSession::sendToOnePerson(NetMessage* message, int connectionIndex)
{
	for (unsigned int i = 0; i < connections.size(); ++i)
	{
		if (connections[i]->connection_index == connectionIndex)
		{
			NetConnection *cp = connections[i];
			if ((cp != nullptr))
			{
				connections[i]->send(message);
			}
		}
	}
}