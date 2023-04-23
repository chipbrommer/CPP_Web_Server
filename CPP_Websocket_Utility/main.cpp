#include <iostream>
#include "Source/WebsocketServer/websocket_server.h"

int main()
{
	Essentials::WebsocketServer ws;
	ws.start();

	return 0;
}
