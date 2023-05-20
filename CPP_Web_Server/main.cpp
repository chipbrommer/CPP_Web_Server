#include <iostream>
#include "Source/CPP_Web_Server/web_server.h"

int main()
{
	std::cout << "Hello CMake." << std::endl;

	Essentials::Communications::Web_Server ws;

	std::string address = "http://0.0.0.0";
	int16_t port = 8000;

#ifdef WIN32
	std::string root = "C:/website";
#else
	std::string root = "/mnt/c/website";
#endif

	ws.Configure(address, port, root);

	if (ws.Start() < 0)
	{
		std::cout << ws.GetLastError();
	}
	return 0;
}
