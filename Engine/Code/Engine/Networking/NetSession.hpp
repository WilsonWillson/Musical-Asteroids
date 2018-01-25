#pragma once
#include "Engine/Networking/net_address_t.hpp"
#include <inttypes.h>
#include <vector>
class NetMessageDefinition;
class NetConnection;
class LoopBackConnection;
class NetMessage;


enum eSessionState
{
	SESSION_DISCONNECTED,
	SESSION_CONNECTING,
	SESSION_CONNECTED,
};

constexpr uint8_t INVALID_CONNECTION_INDEX = 0xff;


class NetSession
{
public:
	NetSession();
	virtual ~NetSession();

	//Virtual
public:
	virtual void host(uint16_t port) = 0;
	virtual bool join(net_address_t &addr) = 0; // production version: session_join_info_t instead
	virtual void leave() = 0;
	virtual void update() = 0;

	virtual bool start_listening() = 0;
	virtual void stop_listening() = 0;
	virtual bool is_listening() = 0;



public:
	bool register_message_definition(uint8_t msg_id, NetMessageDefinition &defn);

	NetMessageDefinition* get_message_definition(uint8_t id) const;

	uint8_t get_free_connection_index() const;
	void join_connection(uint8_t idx, NetConnection*conn);
	void destroy_connection(NetConnection *cp);
	NetConnection* get_connection(uint8_t idx);
	void send_message_to_others(NetMessage const &msg);


	void sendToOnePerson(NetMessage* message, int connectionIndex);
	inline bool am_i_host() const { return (my_connection == host_connection) && (host_connection != nullptr); }

	inline bool am_i_client() const { return (my_connection != host_connection) && (my_connection != nullptr); }		
	
	inline bool is_running() const{ return (my_connection != nullptr); }

	inline bool is_ready() const { return (m_sessionState == SESSION_CONNECTED);}


public:
	// connection info
	NetConnection *connection_list;
	std::vector<NetConnection*> connections;
	NetConnection *my_connection;		// helpers
	NetConnection *host_connection; 	// 
	LoopBackConnection *my_loopBack;// message data;
	float			m_networkTime;
	float			m_AssumedNetworkTime;
	float			m_SnapShotDeltaTime;
	std::vector<NetMessageDefinition*> message_defintions;
	eSessionState m_sessionState;
	unsigned int max_connection_count;
};
