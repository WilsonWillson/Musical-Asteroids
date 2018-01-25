#pragma once
#include "Engine/Core/Event.hpp"

#include <functional>

class NetMessage;
// net/messagedefn.h"
class NetMessageDefinition
{
public:
	uint8_t type_index;

	EventV0* eventHandler;
	//std::function<void(NetMessage*)> handler;
};