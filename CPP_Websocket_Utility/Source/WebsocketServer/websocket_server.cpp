#include "websocket_server.h"

namespace Essentials
{
	WebsocketServer::WebsocketServer(const std::string& address, const std::string& root, const uint16_t port)
	{
		setAddress(address);
		setRootDirectory(root);
		setPort(port);
		mg_mgr_init(&mManager);
	}

	WebsocketServer::~WebsocketServer()
	{
		mg_mgr_free(&mManager);
	}

	int8_t WebsocketServer::setAddress(const std::string& address)
	{
		this->mAddress = address;

		if (mAddress == address)
		{
			return 0;
		}

		return -1;
	}

	int8_t WebsocketServer::setRootDirectory(const std::string& root)
	{
		this->mRootDirectory = root;

		if (mRootDirectory == root)
		{
			return 0;
		}

		return -1;
	}

	int8_t WebsocketServer::setPort(const uint16_t port)
	{
		this->mPort = port;

		if (mPort == port)
		{
			return 0;
		}

		return -1;
	}

	int8_t WebsocketServer::start()
	{
		// Set the full address
		std::string fullAddress = mAddress + ":" + std::to_string(mPort);

		// Create the listener
		mg_http_listen(&mManager, fullAddress.c_str(), eventCallback, NULL); // Create HTTP listener

		// Display some connection related data. 
		MG_INFO(("Mongoose version : v%s",	MG_VERSION));
		MG_INFO(("Listening on     : %s",	fullAddress.c_str()));
		MG_INFO(("Web root         : [%s]", mRootDirectory.c_str()));

		// Infinite for loop to poll for events. 
		for(;;)
		{
			mg_mgr_poll(&mManager, 1000);
		}

		return 0;
	}
}