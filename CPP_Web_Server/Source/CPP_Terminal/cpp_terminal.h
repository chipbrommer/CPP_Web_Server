///////////////////////////////////////////////////////////////////////////////
//!
//! @file		terminal.h
//! 
//! @brief		A cross platform class to communicate to the OS terminal for 
//!				non console applications
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
#include <string>						// Strings
#include <iostream>						// IO
#include <map>							// Error map
#include <thread>						// thread for async reader
//
//	Defines:
//          name                        reason defined
//          --------------------        ---------------------------------------
#ifndef     CPP_TERMINAL				// Define the cpp terminal class. 
#define     CPP_TERMINAL
//
#ifdef WIN32
#define popen _popen
#define pclose _pclose
#define WEXITSTATUS
#endif
// 
///////////////////////////////////////////////////////////////////////////////

namespace Essentials
{
	namespace Utilities
	{
		const static uint8_t TERMINAL_VERSION_MAJOR = 0;
		const static uint8_t TERMINAL_VERSION_MINOR = 1;
		const static uint8_t TERMINAL_VERSION_PATCH = 0;
		const static uint8_t TERMINAL_VERSION_BUILD = 0;

		/// @brief Printable string of the web server version
		const static std::string TerminalVersion = "Terminal v" +
			std::to_string(TERMINAL_VERSION_MAJOR) + "." +
			std::to_string(TERMINAL_VERSION_MINOR) + "." +
			std::to_string(TERMINAL_VERSION_PATCH) + " - b" +
			std::to_string(TERMINAL_VERSION_BUILD) + ".\n";

		/// @brief Web server enum for error codes
		enum class TerminalError : uint8_t
		{
			NONE,
			POPEN_FAILED,
			EXCEPTION,
		};

		/// @brief Error enum to readable string conversion map
		static std::map<TerminalError, std::string> TerminalErrorMap
		{
			{TerminalError::NONE,
			std::string("Error Code " + std::to_string((uint8_t)TerminalError::NONE) + ": No error.")},
			{TerminalError::POPEN_FAILED,
			std::string("Error Code " + std::to_string((uint8_t)TerminalError::POPEN_FAILED) + ": pOpen failed.")},
			{TerminalError::EXCEPTION,
			std::string("Error Code " + std::to_string((uint8_t)TerminalError::EXCEPTION) + ": caught exception.")},
		};

		struct CommandResult 
		{
			std::string output;
			int8_t		exitstatus;

			friend std::ostream& operator<<(std::ostream& os, const CommandResult& result) 
			{
				os << "command exitstatus: " << result.exitstatus << " output: " << result.output;
				return os;
			}

			bool operator==(const CommandResult& rhs) const 
			{
				return	output == rhs.output && exitstatus == rhs.exitstatus;
			}

			bool operator!=(const CommandResult& rhs) const 
			{
				return !(rhs == *this);
			}
		};

		class Terminal
		{
		public:
			Terminal();
			~Terminal();
			int8_t ExecuteCommand(std::string& command, std::string& result);
			std::string ExecuteCommand(std::string& command);
			std::string GetLastError();
		protected:
		private:
			TerminalError	mLastError;
		};
	}
}
#endif