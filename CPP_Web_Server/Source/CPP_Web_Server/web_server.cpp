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

			if (mThread.joinable())
			{
				mThread.join();
			}
		}

		void Web_Server::Configure(const std::string& address, const int16_t port, const std::string& root)
		{
			mAddress		= address;
			mPort			= port;
			mRootDirectory	= root;
			rootAddress		= root; // Forward the new address to the rootAddress static address holder. 
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

			// Set class thread to run the server connection. 
			mThread = std::thread(&Web_Server::Poll, this);

			// Set the default thread priority until user decides to change
#ifdef WIN32
			SetServerThreadPriority(WebServerThreadPriority::NORMAL);
#else
			SetServerThreadPriority(0);
#endif

			return 0;
		}

		void Web_Server::Stop()
		{
			mRunning = false;
		}

		bool Web_Server::IsRunning()
		{
			return mRunning;
		}

#ifdef WIN32
		int8_t Web_Server::SetServerThreadPriority(WebServerThreadPriority priority)
		{
			if (!SetThreadPriority(mThread.native_handle(), (int)priority)) 
			{
				mLastError = WebServerError::THREAD_PRIORITY_SET_FAILURE;
				return -1;
			}

			return 0;
		}
#else
		int8_t Web_Server::SetServerThreadPriority(int8_t priority)
		{
			int8_t minPriority = GetMinThreadPriorityValue();
			int8_t maxPriority = GetMaxThreadPriorityValue();

			if (priority < minPriority || priority > maxPriority)
			{
				mLastError = WebServerError::LINUX_THREAD_PRIORITY_OOR;
				return -1;
			}

			int policy = 0;
			sched_param schedule{};

			if (pthread_getschedparam(mThread.native_handle(), &policy, &schedule) != 0)
			{
				mLastError = WebServerError::THREAD_PRIORITY_GET_FAILURE;
				return -1;
			}

			if (pthread_setschedparam(mThread.native_handle(), policy, &schedule) != 0) 
			{
				mLastError = WebServerError::THREAD_PRIORITY_SET_FAILURE;
				return -1;
			}

			return 0;
		}

		int8_t Web_Server::GetMinThreadPriorityValue()
		{
			return sched_get_priority_min(SCHED_OTHER);
		}

		int8_t Web_Server::GetMaxThreadPriorityValue()
		{
			return sched_get_priority_max(SCHED_OTHER);
		}

#endif

		std::string Web_Server::GetLastError()
		{
			return WebServerErrorMap[mLastError];
		}

		void Web_Server::Poll()
		{
			while (mRunning)
			{
				mg_mgr_poll(&mManager, 100);
			}
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