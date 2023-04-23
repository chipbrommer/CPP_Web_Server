
#include <string>
#include <stdint.h>
#include "websocket_server_config.h"
#include "../Mongoose/mongoose.h"

namespace Essentials
{
	class WebsocketServer
	{
	public:
		/// <summary>Constructor</summary>
		/// <param name="address">- Address for the server</param>
		/// <param name="root">- Root directory for the server content</param>
		/// <param name="port">- Port for the server</param>
		WebsocketServer(const std::string& address = DEFAULT_ADDRESS, const std::string& root = DEFAULT_WEB_ROOT, const uint16_t port = DEFAULT_PORT);
		
		/// <summary>Deconstructor</summary>
		~WebsocketServer();
		
		/// <summary>Set the address for the server</summary>
		/// <param name="address">- Address for the server to start on. </param>
		/// <returns>-1 on failure, 0 on success</returns>
		int8_t setAddress(const std::string& address);
		
		/// <summary>Set the root directory for the server</summary>
		/// <param name="root">- root directory or web server content</param>
		/// <returns>-1 on failure, 0 on success</returns>
		int8_t setRootDirectory(const std::string& root);
		
		/// <summary>Set the port for the server</summary>
		/// <param name="port">- Port to listen on.</param>
		/// <returns>-1 on failure, 0 on success</returns>
		int8_t setPort(const uint16_t port);
		
		/// <summary></summary>
		/// <returns>-1 on failure, 0 on success</returns>
		int8_t start();

	protected:
	private:

		/// <summary>Event handler for the webserver</summary>
		/// <param name="c">- Connection</param>
		/// <param name="ev">- Event number</param>
		/// <param name="ev_data">- Event data</param>
		/// <param name="fn_data">- User data.</param>
		static void eventCallback(struct mg_connection* c, int ev, void* ev_data, void* fn_data)
		{
			// Received an HTTP message
			if (ev == MG_EV_HTTP_MSG)
			{
				// event data is a Mongoose HTTP message structure
				struct mg_http_message* hm = (struct mg_http_message*)ev_data;

				if (mg_http_match_uri(hm, "/"))
				{
					mg_http_reply(c, 200, "Content-Type: text/plain\r\n", "Hello, %s\n", "world");
				}
				else if (mg_http_match_uri(hm, "/index"))
				{
					mg_http_reply(c, 200, "Content-Type: text/plain\r\n", "Hello, %s\n", "index");
				}

				struct mg_http_serve_opts opts;
				memset(&opts, 0, sizeof(opts));
				opts.root_dir = DEFAULT_WEB_ROOT;
				mg_http_serve_dir(c, reinterpret_cast<mg_http_message*>(ev_data), &opts);
			}
			(void)fn_data;
		}

		std::string		mAddress;			// Address for the server
		std::string		mRootDirectory;		// Root directory for the webserver content
		uint16_t		mPort;				// Port for the server
		struct mg_mgr	mManager;			// Mongoose Manager 
	};
}