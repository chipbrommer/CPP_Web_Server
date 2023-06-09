///////////////////////////////////////////////////////////////////////////////
//!
//! @file        web_server.cpp
//! 
//! @brief        Implementation of the web server class
//! 
//! @author        Chip Brommer
//!
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//
//  Includes:
//          name                        reason included
//          --------------------        ---------------------------------------
#include    "web_server.h"                // Web Server Class
//
///////////////////////////////////////////////////////////////////////////////

namespace Essentials
{
    namespace Communications
    {
        // Initialize static class variables.
        Web_Server* Web_Server::mInstance = NULL;

        // PUBLIC FUNCTION DEFINITIONS

        Web_Server* Web_Server::GetInstance()
        {
            if (mInstance == NULL)
            {
                mInstance = new Web_Server;
            }

            return mInstance;
        }

        void Web_Server::ReleaseInstance()
        {
            if (mInstance != NULL)
            {
                delete mInstance;
                mInstance = NULL;
            }
        }

        void Web_Server::Configure(const std::string& address, const int16_t port, const std::string& root)
        {
            this->mAddress            = address;
            this->mPort                = port;
            this->mRootDirectory    = root;
            rootAddress                = root; // Forward the new address to the rootAddress static address holder. 
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

            mConnection = mg_http_listen(&mManager, fullAddress.c_str(), eventCallback, static_cast<void*>(this));
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
            SetServerThreadPriority(WebServerThreadPriority::NORMAL);
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

        int8_t Web_Server::SetServerThreadPriority(WebServerThreadPriority priority)
        {
#ifdef WIN32
            if (!SetThreadPriority(mThread.native_handle(), (int)priority))
            {
                mLastError = WebServerError::THREAD_PRIORITY_SET_FAILURE;
                return -1;
            }
#else
            int8_t minPriority = GetMinThreadPriorityValue();
            int8_t maxPriority = GetMaxThreadPriorityValue();

            if ((uint8_t)priority < minPriority || (uint8_t)priority > maxPriority)
            {
                return -1;
            }

            int policy = SCHED_OTHER;
            sched_param schedule{};

            if (pthread_getschedparam(mThread.native_handle(), &policy, &schedule) != 0)
            {
                return -1;
            }

            schedule.sched_priority = (uint8_t)priority;

            if (pthread_setschedparam(mThread.native_handle(), policy, &schedule) != 0)
            {
                return -1;
            }
#endif
            return 0;
        }

#ifndef WIN32
        int8_t Web_Server::GetMinThreadPriorityValue()
        {
            return sched_get_priority_min(SCHED_OTHER);
        }

        int8_t Web_Server::GetMaxThreadPriorityValue()
        {
            return sched_get_priority_max(SCHED_OTHER);
        }
#endif

        int8_t Web_Server::AddPublishedFunction(const PublishedFunction function)
        {
            PublishedFunction temp;
            temp = function;

            // Check if the unique_name already exists using this callable lambda function
            if (std::any_of(mFunctions.begin(), mFunctions.end(),
                [&](const PublishedFunction& existing)
                {
                    return existing.unique_name == temp.unique_name;
                })) 
            {
                // Duplicate found, return failure
                return -1;
            }

            // No duplicate found, add the function to the vector
            mFunctions.push_back(temp);
            return 0;
        }

        int8_t Web_Server::AddPublishedData(PublishedData data)
        {
            PublishedData temp;
            temp = data;

            // Check if the unique_name already exists using this callable lambda function
            if (std::any_of(mDatas.begin(), mDatas.end(),
                [&](const PublishedData& existing)
                {
                    return existing.unique_name == temp.unique_name;
                }))
            {
                // Duplicate found, return failure
                return -1;
            }

            // No duplicate found, add the function to the vector
            mDatas.push_back(temp);
            return 0;
        }

        int8_t Web_Server::AddPublishedGraphData(PublishedGraphData graph)
        {
            PublishedGraphData temp;
            temp = graph;

            // Check if the unique_name already exists using this callable lambda function
            if (std::any_of(mGraphDatas.begin(), mGraphDatas.end(),
                [&](const PublishedGraphData& existing)
                {
                    return existing.unique_name == temp.unique_name;
                }))
            {
                // Duplicate found, return failure
                return -1;
            }

            // No duplicate found, add the function to the vector
            mGraphDatas.push_back(temp);
            return 0;
        }

        int8_t Web_Server::GetNumberOfPublishedFunctions()
        {
            return static_cast<int8_t>(mFunctions.size());
        }

        int8_t Web_Server::GetNumberOfPublishedDatas()
        {
            return static_cast<int8_t>(mDatas.size());
        }

        int8_t Web_Server::GetNumberOfPublishedGraphDatas()
        {
            return static_cast<int8_t>(mGraphDatas.size());
        }

        std::vector<std::string> Web_Server::GetNamesOfPublishedFunctions() const
        {
            std::vector<std::string> functionNames;
            functionNames.reserve(mFunctions.size());

            for (const auto& function : mFunctions)
            {
                functionNames.push_back(function.unique_name);
            }

            return functionNames;
        }

        std::vector<std::string> Web_Server::GetNamesOfPublishedDatas() const
        {
            std::vector<std::string> dataNames;
            dataNames.reserve(mDatas.size());

            for (const auto& data : mDatas)
            {
                dataNames.push_back(data.unique_name);
            }

            return dataNames;
        }

        std::vector<std::string> Web_Server::GetNamesOfPublishedGraphDatas() const
        {
            std::vector<std::string> graphNames;
            graphNames.reserve(mGraphDatas.size());

            for (const auto& graph : mGraphDatas)
            {
                graphNames.push_back(graph.unique_name);
            }

            return graphNames;
        }

        std::string Web_Server::GetLastError()
        {
            return WebServerErrorMap[mLastError];
        }

        // PRIVATE FUNCTION DEFINITIONS

        Web_Server::Web_Server()
        {
            mAddress = "";
            mPort = -1;
            mRootDirectory = "";
            mLastError = WebServerError::NONE;
            mRunning = false;
            mg_mgr_init(&mManager);
            mConnection = nullptr;
            mWebsocketConnetion = nullptr;
            mUpgraded = false;

#ifdef CPP_TERMINAL
            mTerminal = new Essentials::Utilities::Terminal;
#endif

#ifdef CPP_LOGGER
            mLog = Essentials::Utilities::Log::GetInstance();
#endif
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

        int8_t Web_Server::SendConsoleLog(const std::string& message)
        {
            if (this->mWebsocketConnetion && this->mUpgraded)
            {
                mg_ws_send(this->mWebsocketConnetion, message.c_str(), message.size(), WEBSOCKET_OP_TEXT);
                return 0;
            }

            // WebSocket connection not available or not upgraded
            return -1;
        }
    }
}