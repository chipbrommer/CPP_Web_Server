///////////////////////////////////////////////////////////////////////////////
//!
//! @file        terminal.cpp
//! 
//! @brief        Implementation of the terminal class
//! 
//! @author        Chip Brommer
//!
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//
//  Includes:
//          name                        reason included
//          --------------------        ---------------------------------------
#include    "cpp_terminal.h"                // Terminal Class
//
///////////////////////////////////////////////////////////////////////////////

namespace Essentials
{
    namespace Utilities
    {
        Terminal::Terminal()
        {
            mLastError    = TerminalError::NONE;
        }

        Terminal::~Terminal()
        {

        }

        int8_t Terminal::ExecuteCommand(std::string& command, std::string& result)
        {
            char buffer[512] = {0};

            FILE* pipe = popen(command.c_str(), "r");

            if (pipe == nullptr)
            {
                mLastError = TerminalError::POPEN_FAILED;
                return -1;
            }
            try 
            {
                size_t bytesread;
                while ((bytesread = std::fread(buffer, sizeof(buffer[0]), sizeof(buffer), pipe)) != 0) 
                {
                    result += std::string(buffer, bytesread);
                }
            }
            catch (...) // Catch an exception
            {
                pclose(pipe);
                mLastError = TerminalError::EXCEPTION;
                return -1;
            }

            // This returns the exit status of a child process in linux, does nothing in Windows
            // because _popen returns a status value by default. 
            return WEXITSTATUS(pclose(pipe));
        }

        std::string Terminal::ExecuteCommand(std::string& command)
        {
            std::string result;
            if (ExecuteCommand(command,result) < 0)
            {
                return "";
            }

            return result;
        }

        std::string Terminal::GetLastError()
        {
            return TerminalErrorMap[mLastError];
        }
    }
}