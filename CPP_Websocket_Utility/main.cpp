// CPP_Websocket_Utility.cpp : Defines the entry point for the application.
//

#include <iostream>
#include "Source/WebsocketServer/websocket_server.h"


int main()
{
	std::cout << "Hello CMake.\n";
	Essentials::WebsocketServer ws;
	ws.start();

	return 0;
}
