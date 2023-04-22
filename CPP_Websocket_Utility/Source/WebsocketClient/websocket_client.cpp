#include "websocket_client.h"

namespace Essentials
{
	WebsocketClient::WebsocketClient()
	{

	}

	WebsocketClient::WebsocketClient(const std::string& address, const uint16_t port)
	{
		this->mAddress = address;
		this->mPort = port;
	}

	WebsocketClient::~WebsocketClient()
	{

	}

	int8_t WebsocketClient::setAddress(const std::string& address)
	{
		this->mAddress = address;

		if (mAddress == address)
		{
			return 0;
		}

		return -1;
	}

	int8_t WebsocketClient::setPort(const uint16_t port)
	{
		this->mPort = port;

		if (mPort == port)
		{
			return 0;
		}

		return -1;
	}

	int8_t WebsocketClient::start()
	{
		return 0;
	}
}