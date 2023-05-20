///////////////////////////////////////////////////////////////////////////////
//!
//! @file		web_server.h
//! 
//! @brief		A cross platform class to spawn a web server.
//! 
//! @author		Chip Brommer
//!
///////////////////////////////////////////////////////////////////////////////
#pragma once
///////////////////////////////////////////////////////////////////////////////
//
//  Includes:
//          name                        reason included
//          --------------------        ---------------------------------------
#ifdef WIN32

#else

#endif
#include <string>						// Strings
#include <map>							// Maps
#include "../Mongoose/mongoose.h"		// Mongoose functionality
#include <iostream>						// @todo remove
//
//	Defines:
//          name                        reason defined
//          --------------------        ---------------------------------------
#ifndef     CPP_WEB_SERVER				// Define the cpp web server class. 
#define     CPP_WEB_SERVER
//
///////////////////////////////////////////////////////////////////////////////

namespace Essentials
{
	namespace Communications
	{
		const static uint8_t WEB_SERVER_VERSION_MAJOR = 0;
		const static uint8_t WEB_SERVER_VERSION_MINOR = 1;
		const static uint8_t WEB_SERVER_VERSION_PATCH = 0;
		const static uint8_t WEB_SERVER_VERSION_BUILD = 0;

		/// @brief Printable string of the web server version
		const static std::string WebServerVersion = "Web Server v" +
					std::to_string(WEB_SERVER_VERSION_MAJOR) + "." +
					std::to_string(WEB_SERVER_VERSION_MINOR) + "." +
					std::to_string(WEB_SERVER_VERSION_PATCH) + " - b" +
					std::to_string(WEB_SERVER_VERSION_BUILD) + ".\n";

		/// @brief Web server enum for error codes
		enum class WebServerError : uint8_t
		{
			NONE,
			SERVER_ALREADY_STARTED,
			DATA_NOT_SET,
			LISTENER_INIT_FAILURE,
		};

		/// @brief Error enum to readable string conversion map
		static std::map<WebServerError, std::string> WebServerErrorMap
		{
			{WebServerError::NONE,
			std::string("Error Code " + std::to_string((uint8_t)WebServerError::NONE)					+ ": No error.")},
			{WebServerError::SERVER_ALREADY_STARTED,
			std::string("Error Code " + std::to_string((uint8_t)WebServerError::SERVER_ALREADY_STARTED) + ": Server already started.")},
			{WebServerError::DATA_NOT_SET,
			std::string("Error Code " + std::to_string((uint8_t)WebServerError::DATA_NOT_SET)			+ ": Server info not set.")},
			{WebServerError::LISTENER_INIT_FAILURE,
			std::string("Error Code " + std::to_string((uint8_t)WebServerError::LISTENER_INIT_FAILURE)	+ ": Failed to initalize http listener.")},
		};

		/// @brief Web server class
		class Web_Server
		{
		public:
			/// @brief Default constructor
			Web_Server();

			/// @brief Default deconstructor
			~Web_Server();

			/// @brief Configure the Web Server client
			/// @param address - [in] - Address to spawn the server on
			/// @param port - [in] - Port to spawn the server on
			/// @param root - [in] - Root directory of the web files
			void Configure(const std::string& address, const int16_t port, const std::string& root);

			/// @brief Starts the web server
			/// @return -1 on fail, 0 on success
			int8_t Start();

			/// @brief Stops the web server
			void Stop();

			/// @brief Get the last error
			/// @return String containing information on the last error
			std::string GetLastError();

		protected:
		private:

			bool IsDataSet();

			static void eventCallback(mg_connection* conn, int event, void* eventData, void* funcData)
			{
				if (event == MG_EV_OPEN)
				{
					std::cout << "Opened!\n";
				}
				else if (event == MG_EV_HTTP_MSG)
				{
					std::cout << "HTTP EVENT!\n";
				}
				else if (event == MG_EV_WS_MSG)
				{
					std::cout << "WS EVENT!\n";
				}
			}

			std::string		mAddress;				// Address to spawn the server on.
			int16_t			mPort;					// Port to spawn the server on.
			std::string		mRootDirectory;			// Root directory for the server files. 
			WebServerError	mLastError;				// Last errror for web server class.
			bool			mRunning;				// Bool if server is running. 
			mg_mgr			mManager;				// Mongoose server manager.
			mg_connection* mConnection;				// Mongoose server connection.
		};

	}	// end Communications
}	// end Essentials

#endif // CPP_WEB_SERVER+