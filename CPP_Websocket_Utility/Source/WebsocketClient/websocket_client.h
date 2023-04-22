
#include <string>
#include <stdint.h>

namespace Essentials
{
	class WebsocketClient
	{
	public:
		WebsocketClient();
		WebsocketClient(const std::string& address, const uint16_t port);
		~WebsocketClient();
		int8_t setAddress(const std::string& address);
		int8_t setPort(const uint16_t port);
		int8_t start();
	protected:
	private:
		std::string		mAddress;
		uint16_t		mPort;
	};
}