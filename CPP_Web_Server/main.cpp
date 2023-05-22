#include <iostream>
#include "Source/CPP_Web_Server/web_server.h"

int main()
{
	std::string address = "http://127.0.0.1";
	int16_t port = 80;

#ifdef WIN32
	std::string root = "C:/website";
#else
	std::string root = "/mnt/c/website";
#endif

	Essentials::Communications::Web_Server* ws = ws->GetInstance();

	ws->Configure(address, port, root);

	if (ws->Start() < 0)
	{
		std::cout << ws->GetLastError();
	}

#ifdef WIN32
	ws->SetServerThreadPriority(Essentials::Communications::WebServerThreadPriority::NORMAL);
#else
	ws->SetServerThreadPriority(0);
#endif
	int count = 0;
	while (ws->IsRunning())
	{
		count++;
		ws->SendConsoleLog("Log: " + std::to_string(count));
		
		Sleep(2000);
	}

	return 0;
}
