#include "websocket_server.h"

namespace Essentials
{
	WebsocketServer::WebsocketServer(const std::string& address, const uint16_t port)
	{
		this->mAddress = address;
		this->mPort = port;
	}

	WebsocketServer::~WebsocketServer()
	{

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
		std::string fullAddress = mAddress + ":" + std::to_string(mPort);
		printf("Listerning on: %s\n", fullAddress.c_str());

		return 0;
	}
}