#pragma once
#include "Engine/Networking/NetMessage.hpp"
#include <map>
class NetObjectTypeDefinition;
class NetSession;
class NetMessage;
class NetObject;
//enum NetworkObjectType;

enum NetworkObjectType
{
	NETOBJECT_SHIP,
	NETOBJECT_BULLET
};

const uint8_t NETOBJECT_CREATE_OBJECT = 1;
const uint8_t NETOBJECT_DESTROY_OBJECT = 2;
const uint8_t NETOBJECT_SEND_SNAPSHOT = 3;
const uint8_t INVALID_NETWORK_ID = 0xFFFF;

void NetObjectSystemStep(float deltaSeconds);
void SendNetObjectUpdates();

void SendNetObjectUpdateTo(NetConnection *cp);
void OnNetObjectUpdateRecieved(void* user_arg, void* msg_arg);


typedef void(*append_create_info_cb)(std::vector<std::string>*, void*);
typedef void* (*process_create_info_cb)(NetMessage*, NetObject *obj);
typedef void(*append_destory_info_cb)(NetMessage*, void*);
typedef void* (*process_destroy_info_cb)(NetMessage*, void*);
typedef void(*get_snapshot_info_cb)(void*, void*);
typedef void(*append_snapshot_info_cb)(std::vector<std::string>*, void*);
typedef void* (*process_snapshot_info_cb)(NetMessage*, void* obj);
typedef void(*apply_snapshot_info_cb)(void*, void*); //snapshot and local object
typedef bool(*compare_snapshot_info_cb)(void*, void*); //snapshot and local object
typedef void(*set_snapshot_equal_cb)(void*, void*); //snapshot and local object

class NetObjectTypeDefinition
{
public:
	append_create_info_cb append_create_info;
	process_create_info_cb process_create_info;
	append_destory_info_cb append_destroy_info;
	process_destroy_info_cb process_destroy_info;
	get_snapshot_info_cb   get_snapshot_info;
	append_snapshot_info_cb append_snapshot_info;
	process_snapshot_info_cb process_snapshot_info;
	apply_snapshot_info_cb  apply_snapshot_info;
	compare_snapshot_info_cb compare_snapshot_info;
	set_snapshot_equal_cb equal_snapshot_info;

public:
	size_t		snapShotSize;
	int			netType;
};


class NetObject
{
public:
	NetObject(NetObjectTypeDefinition *defn)
		: definition(defn)
		, net_id(INVALID_NETWORK_ID)
		, local_object(nullptr)
		, current_SnapShot(nullptr)
		, lastReceivedSnapshot(nullptr)
		,noChange(false)
	{}

	~NetObject()
	{}

public:
	//std::vector<NetConnection*> knownConnections;
	std::vector<bool> didConnectionChange;
	std::vector<void*> LastSentSnapShots;
	uint16_t net_id;
	NetObjectTypeDefinition *definition;
	bool  noChange;

	void *local_object;
	void *current_SnapShot;
	void *lastReceivedSnapshot;
	float m_SnapShotDeltaTime = 0;
};

void CreateObject(void* user_arg, void* msg_arg);
void DeleteObject(void* user_arg, void* msg_arg);


uint16_t NetObjectGetUnusedID();
void ReplicateAllKnownObjects(int connection_index);
NetObject* NetObjectReplicate(void *object_ptr, NetworkObjectType type_id);
void OnReceiveNetObjectCreate(NetMessage *msg);


void NetObjectStopRelication(uint16_t net_id, NetworkObjectType type_id);
void NetObjectReceiveDestroy(NetMessage *msg);

NetObjectTypeDefinition* NetObjectFindDefinition(uint8_t type_id);
NetObjectTypeDefinition* NetObjectFindDefinition(NetworkObjectType type_id);
void NetObjectRegister(NetObject * nop);
NetObject* NetObjectFind(uint16_t net_id);
void NetObjectUnregister(NetObject * nop);

NetSession* NetObjectSession();

void NetObjectSystemRegisterType(NetworkObjectType objectType, NetObjectTypeDefinition defn);


void append_destroy_info(NetMessage* msg, void * local_object);
void process_destroy_info(NetMessage * msg, void * local_object);


