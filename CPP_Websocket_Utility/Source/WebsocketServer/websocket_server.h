
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
			if (ev == MG_EV_OPEN)
			{
				// c->is_hexdumping = 1;
			}
			else if (ev == MG_EV_HTTP_MSG)
			{
				struct mg_http_message* hm = (struct mg_http_message*)ev_data;

				if (mg_http_match_uri(hm, "/websocket"))
				{
					// Upgrade to websocket..
					mg_ws_upgrade(c, hm, NULL);
				}
				else if (mg_http_match_uri(hm, "/rest"))
				{
					// Serve REST response
					mg_http_reply(c, 200, "", "{\"result\": %d}\n", 123);
				}
				else if (mg_http_match_uri(hm, "/hello"))
				{
					mg_http_reply(c, 200, "Content-Type: text/plain\r\n", "Hello, %s\n", "world");
				}
				else
				{
					struct mg_http_serve_opts opts;
					memset(&opts, 0, sizeof(opts));
					opts.root_dir = web_root.c_str();
					mg_http_serve_dir(c, reinterpret_cast<mg_http_message*>(ev_data), &opts);
				}
			}
			else if (ev == MG_EV_WS_MSG)
			{
				// Got websocket frame. Received data is wm->data. Echo it back!
				struct mg_ws_message* wm = (struct mg_ws_message*)ev_data;
				mg_ws_send(c, wm->data.ptr, wm->data.len, WEBSOCKET_OP_TEXT);
			}
			(void)fn_data;
		}

		std::string		mAddress;			// Address for the server
		std::string		mRootDirectory;		// Root directory for the webserver content
		uint16_t		mPort;				// Port for the server
		struct mg_mgr	mManager;			// Mongoose Manager 
	};
}