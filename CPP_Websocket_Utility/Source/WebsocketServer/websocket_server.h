
#include <string>
#include <stdint.h>
#include "websocket_server_config.h"
#include "../Mongoose/mongoose.h"

namespace Essentials
{
	class WebsocketServer
	{
	public:
		WebsocketServer(const std::string& address = DEFAULT_ADDRESS, const uint16_t port = DEFAULT_PORT);
		~WebsocketServer();
		int8_t setAddress(const std::string& address);
		int8_t setPort(const uint16_t port);
		int8_t start();
	protected:
	private:
		std::string		mAddress;
		uint16_t		mPort;
	};
}