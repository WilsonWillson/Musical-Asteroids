#include "Engine/Networking/NetObject.hpp"
#include "Engine/Networking/NetMessage.hpp"
#include "Engine/Networking/NetSession.hpp"
#include "Engine/Networking/NetConnection.hpp"
#include "Engine/Core/StringUtils.hpp"
#include "Game/Game.hpp"
#include <vector>

std::map<NetworkObjectType, NetObjectTypeDefinition> g_NetObjectDefinitionsInGame;
int g_NetID = -1;
std::map<uint16_t, NetObject*> g_RegisteredObjects;

float currentNetTime = 0.0f;
float intervalTime = 1.0f / 15.f;


//-------------------------------Commands--------------------------------------

void CreateObject(void* user_arg, void* msg_arg)
{
	user_arg;
	NetMessage* msg = (NetMessage*)msg_arg;
	//DebuggerPrintf("Create Object Response");
	//DebuggerPrintf("Payload Bytes Used: ");
	//DebuggerPrintf(std::to_string(msg->payload_bytes_used).c_str());

	std::string sentName = msg->readString();

	std::vector<std::string> namesSent = ParseStringIntoPiecesByDelimiter(sentName, ' ');

	const uint8_t* p = reinterpret_cast<const uint8_t*>(namesSent[0].c_str());
	unsigned int type = (unsigned int)*p - '0';
	NetworkObjectType objectType = (NetworkObjectType)type;
	NetObjectTypeDefinition *defn = NetObjectFindDefinition(objectType);

	NetObject *nop = new NetObject(defn);
	int netId = std::atoi((namesSent[1].c_str()));

	nop->net_id = netId;

	void *local_object = defn->process_create_info(msg, nop);
	//ASSERT_OR_DIE(local_object != nullptr);

	nop->local_object = local_object;
	NetObjectRegister(nop); // register object with system
}

void DeleteObject(void* user_arg, void* msg_arg)
{
	user_arg;
	NetMessage* msg = (NetMessage*)msg_arg;
	//DebuggerPrintf("Delete Object Response");
	//DebuggerPrintf("Payload Bytes Used: ");
	//DebuggerPrintf(std::to_string(msg->payload_bytes_used).c_str());

	std::string sentName = msg->readString();

	std::vector<std::string> namesSent = ParseStringIntoPiecesByDelimiter(sentName, ' ');

	const uint8_t* p = reinterpret_cast<const uint8_t*>(namesSent[0].c_str());
	unsigned int type = (unsigned int)*p - '0';
	NetworkObjectType objectType = (NetworkObjectType)type;
	NetObjectTypeDefinition *defn = NetObjectFindDefinition(objectType);

	int netId = std::atoi((namesSent[1].c_str()));

	NetObject *nop = NetObjectFind(netId);

	if (nop == nullptr) {
		return;
	}

	NetObjectUnregister(nop);

	// THIS is critical;
	nop->definition->process_destroy_info(msg, nop->local_object);

	delete nop;
}

//------------------------------------------------------------------------
uint16_t  NetObjectGetUnusedID()
{
	// excercise for the user; 
	// start with last returned value, increment, make sure it's not in used;

	while (1)
	{
		g_NetID++;

		bool usedID = true;
		for (std::map<uint16_t, NetObject*>::iterator it = g_RegisteredObjects.begin(); it != g_RegisteredObjects.end(); ++it)
		{
			if (it->second->net_id != g_NetID)
				usedID = true;
			else
				usedID = false;
		}
		
		if(usedID = true)
			return g_NetID;
	}
}

void ReplicateAllKnownObjects(int connection_index)
{

	for (std::map<uint16_t, NetObject*>::iterator it = g_RegisteredObjects.begin(); it != g_RegisteredObjects.end(); ++it)
	{
		//NetObject *nop = new NetObject(it->second->definition);
		//nop->local_object = object_ptr;
		//nop->net_id = NetObjectGetUnusedID();

		NetMessage create(NETOBJECT_CREATE_OBJECT);

		std::vector<std::string> name;
		name.push_back(std::to_string(it->second->definition->netType));
		name.push_back(std::to_string(it->second->net_id));

		it->second->definition->append_create_info(&name, it->second->local_object);
		create.write_string(name, 0);

		NetSession *sp = NetObjectSession();
		//g_theGame->m_TotalBytesSent += create.payload_bytes_used;
		sp->sendToOnePerson(&create, connection_index);

	}
}

NetObject* NetObjectReplicate(void *object_ptr, NetworkObjectType type_id)
{

	NetObjectTypeDefinition *defn = NetObjectFindDefinition(type_id);
	if (defn == nullptr) {
		return nullptr;
	}

	NetObject *nop = new NetObject(defn);

	nop->local_object = object_ptr;
	nop->net_id = NetObjectGetUnusedID();

	NetObjectRegister(nop);

	NetMessage create(NETOBJECT_CREATE_OBJECT);

	std::vector<std::string> name;
	name.push_back(std::to_string(type_id));
	name.push_back(std::to_string(nop->net_id));

	//create.write_string(name, 0);

	defn->append_create_info(&name, object_ptr);
	create.write_string(name, 0);

	NetSession *sp = NetObjectSession();
	//g_theGame->m_TotalBytesSent += create.payload_bytes_used;
	sp->send_message_to_others(create);

	return nop;
}

void OnReceiveNetObjectCreate(NetMessage *msg)
{
	
	//Read in the type_id first
	uint8_t type_id; //= msg->read<uint8_t>();
	uint16_t net_id; //= msg->read<uint16_t>();

	NetObjectTypeDefinition *defn = NetObjectFindDefinition(type_id);
	//ASSERT_OR_DIE(defn != nullptr);

	NetObject *nop = new NetObject(defn);
	nop->net_id = net_id;

	void *local_object = defn->process_create_info(msg, nop);
	//ASSERT_OR_DIE(local_object != nullptr);

	nop->local_object = local_object;
	NetObjectRegister(nop); // register object with system
}

void  NetObjectStopRelication(uint16_t net_id, NetworkObjectType type_id)
{
	// remove from our system
	NetObject *nop = NetObjectFind(net_id);
	if (nop == nullptr) {
		return;
	}

	NetObjectUnregister(nop);

	// tell everyone 
	NetMessage msg(NETOBJECT_DESTROY_OBJECT);

	std::vector<std::string> name;
	name.push_back(std::to_string(type_id));
	name.push_back(std::to_string(nop->net_id));
	msg.write_string(name, 0);

	// usually does nothing - no-op.
	//nop->definition->append_destroy_info(&msg, nop->local_object);

	NetSession *sp = NetObjectSession();
	//g_theGame->m_TotalBytesSent += msg.payload_bytes_used;
	sp->send_message_to_others(msg);
}


NetObjectTypeDefinition* NetObjectFindDefinition(uint8_t type_id)
{	
	std::map<NetworkObjectType, NetObjectTypeDefinition>::iterator it = g_NetObjectDefinitionsInGame.find((NetworkObjectType)type_id);

	if (it == g_NetObjectDefinitionsInGame.end())
		return nullptr;
	
	return 	&it->second;
	
	return nullptr;
}

NetObjectTypeDefinition* NetObjectFindDefinition(NetworkObjectType type_id)
{
	std::map<NetworkObjectType, NetObjectTypeDefinition>::iterator it = g_NetObjectDefinitionsInGame.find(type_id);

	if (it == g_NetObjectDefinitionsInGame.end())
		return nullptr;

	return 	&it->second;
}

void NetObjectRegister(NetObject * nop)
{
	g_RegisteredObjects[nop->net_id] = nop;
}

NetObject* NetObjectFind(uint16_t net_id)
{
	if (g_RegisteredObjects.find(net_id) != g_RegisteredObjects.end())
		return g_RegisteredObjects.find(net_id)->second;
	else
		return nullptr;
}

void NetObjectUnregister(NetObject * nop)
{
	if (g_RegisteredObjects.find(nop->net_id) != g_RegisteredObjects.end())
		g_RegisteredObjects.erase(nop->net_id);
}

NetSession* NetObjectSession()
{
	//return (NetSession*)g_theGame->m_gameSession;
	return nullptr;
}

void NetObjectSystemRegisterType(NetworkObjectType objectType, NetObjectTypeDefinition defn)
{
	g_NetObjectDefinitionsInGame[objectType] = defn;
}



//-----------------Snapshot-------------------------------------------------------
void NetObjectSystemStep(float deltaSeconds)
{
	currentNetTime += deltaSeconds;
	NetSession* session = NetObjectSession();


	if (currentNetTime >= intervalTime) 
	{
		currentNetTime -= intervalTime;

		if (session->am_i_host()) 
			SendNetObjectUpdates();
	}

	if (session->am_i_client())
	{
		// clients do shit every frame... 
		for (std::map<uint16_t, NetObject*>::iterator it = g_RegisteredObjects.begin(); it != g_RegisteredObjects.end(); ++it)
		{

			if (it->second->lastReceivedSnapshot == nullptr)
				it->second->lastReceivedSnapshot = malloc(it->second->definition->snapShotSize);

			it->second->definition->apply_snapshot_info(it->second->local_object, it->second->lastReceivedSnapshot);
		}
	}
}

void SendNetObjectUpdates()
{
	NetSession* session = NetObjectSession();
	
	for (std::map<uint16_t, NetObject*>::iterator it = g_RegisteredObjects.begin(); it != g_RegisteredObjects.end(); ++it)
	{
		if (it->second->current_SnapShot == nullptr)
			it->second->current_SnapShot = malloc(it->second->definition->snapShotSize);

		it->second->definition->get_snapshot_info(it->second->current_SnapShot, it->second->local_object);


		for (unsigned int i = 0; i < session->connections.size(); i++)
		{
			if (session->connections[i] != session->my_connection)
			{
				if (it->second->LastSentSnapShots.size() != session->connections.size())
				{
					it->second->LastSentSnapShots.resize(session->connections.size());
					it->second->didConnectionChange.resize(session->connections.size());
				}
				
				for (unsigned int j = 0; j < it->second->LastSentSnapShots.size(); j++)
				{

					if (it->second->LastSentSnapShots[j] == nullptr)
						it->second->LastSentSnapShots[j] = malloc(it->second->definition->snapShotSize);

					if (session->connections[i] != session->my_connection)
						it->second->didConnectionChange[i] = it->second->definition->compare_snapshot_info(it->second->LastSentSnapShots[j], it->second->current_SnapShot);

					if (it->second->didConnectionChange[i] == false)
						it->second->definition->equal_snapshot_info(it->second->LastSentSnapShots[j], it->second->current_SnapShot);
				}
			}
		}
	}
	

	std::map<uint16_t, NetObject*>::iterator it = g_RegisteredObjects.begin();
	for (unsigned int i = 0; i < session->connections.size(); i++)
	{
		if (session->connections[i] != session->my_connection && it != g_RegisteredObjects.end()) //&& it->second->didConnectionChange[i] == false
		{
			SendNetObjectUpdateTo(session->connections[i]);
			it->second->didConnectionChange[i] = true;
		}
	}
}

void SendNetObjectUpdateTo(NetConnection *cp)
{
	for (std::map<uint16_t, NetObject*>::iterator it = g_RegisteredObjects.begin(); it != g_RegisteredObjects.end(); ++it)
	{
	
		NetMessage update_msg(NETOBJECT_SEND_SNAPSHOT);
		std::vector<std::string> name;
		name.push_back(std::to_string(it->second->net_id));

		if (it->second->current_SnapShot == nullptr)
			it->second->current_SnapShot = malloc(it->second->definition->snapShotSize);
		if (it->second->lastReceivedSnapshot == nullptr)
			it->second->lastReceivedSnapshot = malloc(it->second->definition->snapShotSize);


		it->second->definition->append_snapshot_info(&name, it->second->current_SnapShot);
		update_msg.write_string(name, 0);

		//g_theGame->m_TotalBytesSent += update_msg.payload_bytes_used;
		cp->send(&update_msg);

		it->second->definition->equal_snapshot_info(it->second->lastReceivedSnapshot, it->second->current_SnapShot);
	}
}

void OnNetObjectUpdateRecieved(void* user_arg, void* msg_arg)
{
	user_arg;

	NetMessage* update_msg = (NetMessage*)msg_arg;
	//DebuggerPrintf("On Net Object Response");
	//DebuggerPrintf("Payload Bytes Used: ");
	//DebuggerPrintf(std::to_string(update_msg ->payload_bytes_used).c_str());

	std::string sentName = update_msg->readString();

	std::vector<std::string> namesSent = ParseStringIntoPiecesByDelimiter(sentName, ' ');

	int netId = std::atoi((namesSent[0].c_str()));
	NetObject *nop = NetObjectFind(netId);

	if (nop == nullptr)
		return;

	// like current snapshot - we can just overwrite here
	// so only allocate this memory once. 
	if (nop->lastReceivedSnapshot == nullptr)
		nop->lastReceivedSnapshot = malloc(nop->definition->snapShotSize);

	nop->definition->process_snapshot_info(update_msg, nop->lastReceivedSnapshot);
}

//----------------------NET OBJECT TYPE DEFINITION----------------------------------------------------
void append_destroy_info(NetMessage* msg, void * local_object)
{
	msg; local_object;
}

void process_destroy_info(NetMessage * msg, void * local_object)
{
	msg; local_object;
}
