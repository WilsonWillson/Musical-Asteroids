#include "Engine/Core/Win32Includes.hpp"
#include <vector>

#pragma comment(lib, "Ws2_32.lib")
class TCPSocket;

void UpdateNetSystem();
bool NetSystemHostConnection();
bool NetSystemConnectToHost(char const *hostname, uint16_t port);
void NetSystemShutdown();

void PingHost();


extern TCPSocket* hostSocket;
extern std::vector<TCPSocket*> listenedSockets; 
extern bool startedNetSystem;