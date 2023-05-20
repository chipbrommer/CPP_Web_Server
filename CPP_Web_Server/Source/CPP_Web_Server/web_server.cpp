///////////////////////////////////////////////////////////////////////////////
//!
//! @file		web_server.cpp
//! 
//! @brief		Implementation of the web server class
//! 
//! @author		Chip Brommer
//!
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//
//  Includes:
//          name                        reason included
//          --------------------        ---------------------------------------
#include	"web_server.h"				// Web Server Class
//
///////////////////////////////////////////////////////////////////////////////

namespace Essentials
{
	namespace Communications
	{
		Web_Server::Web_Server()
		{
			mAddress		= "";
			mPort			= -1;
			mRootDirectory	= "";
			mLastError		= WebServerError::NONE;
			mRunning		= false;
			mg_mgr_init(&mManager);
			mConnection		= NULL;
		}

		Web_Server::~Web_Server()
		{
			Stop();
			mg_mgr_free(&mManager);
		}

		void Web_Server::Configure(const std::string& address, const int16_t port, const std::string& root)
		{
			mAddress		= address;
			mPort			= port;
			mRootDirectory	= root;
		}

		int8_t Web_Server::Start()
		{
			if (mRunning)
			{
				mLastError = WebServerError::SERVER_ALREADY_STARTED;
				return -1;
			}

			if (!IsDataSet())
			{
				mLastError = WebServerError::DATA_NOT_SET;
				return -1;
			}

			std::string fullAddress = mAddress + ":" + std::to_string(mPort);

			mConnection = mg_http_listen(&mManager, fullAddress.c_str(), eventCallback, NULL);
			if (mConnection == NULL)
			{
				mLastError = WebServerError::LISTENER_INIT_FAILURE;
				return -1;
			}

			// Display some connection related data. 
			MG_INFO(("Mongoose version : v%s", MG_VERSION));
			MG_INFO(("Listening on     : %s", fullAddress.c_str()));
			MG_INFO(("Web root         : [%s]", mRootDirectory.c_str()));

			mRunning = true;
			return 0;
		}

		void Web_Server::Stop()
		{
			mRunning = false;
		}

		std::string Web_Server::GetLastError()
		{
			return WebServerErrorMap[mLastError];
		}

		bool Web_Server::IsDataSet()
		{
			if (mAddress.empty())
			{
				return false;
			}

			if (mPort == -1)
			{
				return false;
			}

			if (mRootDirectory.empty())
			{
				return false;
			}

			return true;
		}

	}
}