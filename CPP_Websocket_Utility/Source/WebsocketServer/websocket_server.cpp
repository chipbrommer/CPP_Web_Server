#include "websocket_server.h"

namespace Essentials
{
	static void eventHandler(struct mg_connection* c, int ev, void* ev_data, void* fn_data) 
	{
		struct mg_http_serve_opts opts = { .root_dir = "./website/index.html" };  // Serve local dir

		if (ev == MG_EV_HTTP_MSG) mg_http_serve_dir(c, ev_data, &opts);
	}

	WebsocketServer::WebsocketServer(const std::string& address, const uint16_t port)
	{
		this->mAddress = address;
		this->mPort = port;
		mg_mgr_init(mManager);
	}

	WebsocketServer::~WebsocketServer()
	{
		mg_mgr_free(mManager);
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
		mg_http_listen(mManager, fullAddress.c_str(), eventHandler, mManager);

		for (;;)
		{
			mg_mgr_poll(mManager, 1000);
		}

		return 0;
	}
}