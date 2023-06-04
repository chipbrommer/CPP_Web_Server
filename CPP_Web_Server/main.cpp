#include <iostream>
#include "Source/CPP_Timer/cpp_timer.h"
#include "Source/CPP_Logger/cpp_logger.h"
#include "Source/CPP_Terminal/cpp_terminal.h"
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

	Essentials::Utilities::Timer*			timer	= timer->GetInstance();
	Essentials::Utilities::Log*				log		= log->GetInstance();
	Essentials::Communications::Web_Server* ws		= ws->GetInstance();

#ifdef WIN32
	log->Initialize(true, true, "C:/AppData/Developement/log_tester.txt");
#else
	log->Initialize(true, true, "/mnt/c/AppData/Developement/log_tester.txt");
#endif

	ws->Configure(address, port, root);

	if (ws->Start() < 0)
	{
		std::cout << ws->GetLastError();
	}

#ifdef WIN32
	ws->SetServerThreadPriority(Essentials::Communications::WebServerThreadPriority::NORMAL);
	//log->SetLoggerThreadPriority(Essentials::Utilities::LogThreadPriority::LOWEST);
#else
	ws->SetServerThreadPriority(0);
	//log->SetLoggerThreadPriority(0);
#endif

	log->AddEntry(Essentials::Utilities::LOG_LEVEL::LOG_INFO, "Main", "Initialized!");

	int count = 0;
	while (ws->IsRunning())
	{
		count++;
		ws->SendConsoleLog("Log: " + std::to_string(count));
		timer->MSecSleep(500);
	}

	log->AddEntry(Essentials::Utilities::LOG_LEVEL::LOG_INFO, "Main", "Closing!");

	return 0;
}
