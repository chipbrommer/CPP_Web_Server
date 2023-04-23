#include "websocket_server.h"

namespace Essentials
{
    static void eventCallback(
        struct mg_connection* c,    //!< [In] -- pointer to connection
        int ev,                     //!< [In] -- event number
        void* ev_data,              //!< [In] -- data carried with the event
        void* fn_data               //!< [In] -- user data specified when listening
    )
    {
        // we only handle reception of an HTTP message
        if (ev == MG_EV_HTTP_MSG)
        {
            // event data is a Mongoose HTTP message structure
            struct mg_http_message* hm = (struct mg_http_message*)ev_data;

			if (mg_http_match_uri(hm, "/"))
			{
				// Catch an empty path "localhost:port"
				mg_http_reply(c, 200, "Content-Type: text/plain\r\n", "Hello, %s\n", "world");
			}
			else if (mg_http_match_uri(hm, "/index"))
			{
				// yes, build the configuration web page response
				//
				mg_http_reply(c, 200, "Content-Type: text/plain\r\n", "Hello, %s\n", "index");
			}

            // Serve static files (so they get the ASEI logo)
            // set the root directory
            struct mg_http_serve_opts opts;
            memset(&opts, 0, sizeof(opts));
            opts.root_dir = DEFAULT_WEB_ROOT;
            mg_http_serve_dir(c, reinterpret_cast<mg_http_message*>(ev_data), &opts);
        }
        (void)fn_data;
    }

	WebsocketServer::WebsocketServer(const std::string& address, const uint16_t port)
	{
		setAddress(address);
		setPort(port);

		if (mAddressSet && mPortSet)
		{
			mInitialized = true;
		}

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
			mAddressSet = true;
			return 0;
		}

		return -1;
	}

	int8_t WebsocketServer::setPort(const uint16_t port)
	{
		this->mPort = port;

		if (mPort == port)
		{
			mPortSet = true;
			return 0;
		}

		return -1;
	}

	int8_t WebsocketServer::start()
	{
		std::string fullAddress = mAddress + ":" + std::to_string(mPort);
		printf("Listerning on: %s\n", fullAddress.c_str());
		mg_http_listen(&mManager, fullAddress.c_str(), eventCallback, NULL); // Create HTTP listener

		for(;;)
		{
			mg_mgr_poll(&mManager, 1000);          // Infinite event loop
		}

		return 0;
	}
}