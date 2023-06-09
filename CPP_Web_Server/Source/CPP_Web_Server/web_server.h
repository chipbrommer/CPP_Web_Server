///////////////////////////////////////////////////////////////////////////////
//!
//! @file        web_server.h
//! 
//! @brief        A cross platform class to spawn a web server.
//! 
//! @author        Chip Brommer
//!
///////////////////////////////////////////////////////////////////////////////
#pragma once
///////////////////////////////////////////////////////////////////////////////
//
//  Includes:
//          name                        reason included
//          --------------------        ---------------------------------------
#ifdef __linux__
#include <sched.h>                          // Thread scheduling
#endif
#include <string>                           // Strings
#include <map>                              // Maps
#include "../Mongoose/mongoose.h"           // Mongoose functionality
#include <thread>                           // Threading
#include <vector>                           // vectors
#include <algorithm>                        // algorithms
#include "publishable_types.h"              // Publishable data types

#ifdef CPP_TERMINAL
#include "../CPP_Terminal/cpp_terminal.h"   // Terminal access
#endif

#ifdef CPP_LOGGER
#include "../CPP_Logger/cpp_logger.h"       // Logger access
#endif
//
//    Defines:
//          name                        reason defined
//          --------------------        ---------------------------------------
#ifndef     CPP_WEB_SERVER                  // Define the cpp web server class. 
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

        /// @brief Temporary solution for static IP in the class event callback
        static std::string rootAddress = "C:/website";

        /// @brief Web server enum for error codes
        enum class WebServerError : uint8_t
        {
            NONE,
            SERVER_ALREADY_STARTED,
            DATA_NOT_SET,
            LISTENER_INIT_FAILURE,
            LINUX_THREAD_PRIORITY_OOR,
            THREAD_PRIORITY_GET_FAILURE,
            THREAD_PRIORITY_SET_FAILURE,
        };

        /// @brief Error enum to readable string conversion map
        static std::map<WebServerError, std::string> WebServerErrorMap
        {
            {WebServerError::NONE,
            std::string("Error Code " + std::to_string((uint8_t)WebServerError::NONE)                    + ": No error.")},
            {WebServerError::SERVER_ALREADY_STARTED,
            std::string("Error Code " + std::to_string((uint8_t)WebServerError::SERVER_ALREADY_STARTED) + ": Server already started.")},
            {WebServerError::DATA_NOT_SET,
            std::string("Error Code " + std::to_string((uint8_t)WebServerError::DATA_NOT_SET)            + ": Server info not set.")},
            {WebServerError::LISTENER_INIT_FAILURE,
            std::string("Error Code " + std::to_string((uint8_t)WebServerError::LISTENER_INIT_FAILURE)    + ": Failed to initalize http listener.")},
            {WebServerError::LINUX_THREAD_PRIORITY_OOR,
            std::string("Error Code " + std::to_string((uint8_t)WebServerError::LINUX_THREAD_PRIORITY_OOR) + ": Provided thread priority is out of range for this distribution/kernel.")},
            {WebServerError::THREAD_PRIORITY_GET_FAILURE,
            std::string("Error Code " + std::to_string((uint8_t)WebServerError::THREAD_PRIORITY_GET_FAILURE) + ": Failed to get thread priority.")},
            {WebServerError::THREAD_PRIORITY_SET_FAILURE,
            std::string("Error Code " + std::to_string((uint8_t)WebServerError::THREAD_PRIORITY_SET_FAILURE) + ": Failed to set thread priority.")},
        };

        enum class WebServerThreadPriority
        {
#ifdef WIN32
            IDLE            = THREAD_PRIORITY_IDLE,
            LOWEST          = THREAD_PRIORITY_LOWEST,
            BELOW_NORMAL    = THREAD_PRIORITY_BELOW_NORMAL,
            NORMAL          = THREAD_PRIORITY_NORMAL,
            ABOVE_NORMAL    = THREAD_PRIORITY_ABOVE_NORMAL,
            HIGH            = THREAD_PRIORITY_HIGHEST,
            TIME_CRITICAL   = THREAD_PRIORITY_TIME_CRITICAL
#else
            IDLE            = 1,
            LOWEST          = 20,
            BELOW_NORMAL    = 40,
            NORMAL          = 0,
            ABOVE_NORMAL    = 60,
            HIGH            = 80,
            TIME_CRITICAL   = 99,
#endif
        };

        /// @brief Web server class
        class Web_Server
        {
        public:

            //! @brief Prevent cloning.
            Web_Server(Web_Server& other) = delete;

            //! @brief Prevent assigning
            void operator=(const Web_Server&) = delete;

            //! @brief Get current instance or creates a new one. 
            static Web_Server* GetInstance();

            //! @brief Release the instance
            static void ReleaseInstance();

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

            /// @brief Bool for if the server is running
            /// @return true if running, false if not.
            bool IsRunning();

            /// @brief Set the server thread priority
            /// @param priority - WebServerThreadPriority enum for thread priority level
            /// @return -1 on error, 0 on success. 
            int8_t SetServerThreadPriority(WebServerThreadPriority priority);

#ifndef WIN32
            /// @brief Get the minimum thread priority value for unix.
            /// @return the value of the minimum priority. 
            int8_t GetMinThreadPriorityValue();

            /// @brief Get the maximum thread priority value for unix.
            /// @return the value of the maximum priority. 
            int8_t GetMaxThreadPriorityValue();
#endif

            /// @brief Add a function to the web server to be accessed
            /// @param function - [in] - A PublishedFunction to be added to the webserver. 
            /// @return -1 on error (@param unique_name already exists, 0 on success
            int8_t AddPublishedFunction(PublishedFunction function);

            /// @brief Add a data to the web server.
            /// @param data - [in] - A PublishedData to be added to the webserver. 
            /// @return -1 on error (@param unique_name already exists, 0 on success
            int8_t AddPublishedData(PublishedData data);

            /// @brief Add a graph data to the web server.
            /// @param graph - [in] - A PublishedGraphData to be added to the webserver.
            /// @return -1 on error (@param unique_name already exists, 0 on success
            int8_t AddPublishedGraphData(PublishedGraphData graph);

            /// @brief Get the number of published functions.
            /// @return 0+ indicating the number of published functions. 
            int8_t GetNumberOfPublishedFunctions();
            
            /// @brief Get the number of published data elements.
            /// @return 0+ indicating the number of published data elements. 
            int8_t GetNumberOfPublishedDatas();

            /// @brief Get the number of published graph data elements.
            /// @return 0+ indicating the number of published graph data elements. 
            int8_t GetNumberOfPublishedGraphDatas();

            /// @brief Get the names of all the published functions.
            /// @return a std::vector of std::strings with names of the published functions. 
            std::vector<std::string> GetNamesOfPublishedFunctions() const;

            /// @brief Get the names of all the published data elements.
            /// @return a std::vector of std::strings with names of the published datas. 
            std::vector<std::string> GetNamesOfPublishedDatas() const;

            /// @brief Get the names of all the published graph data elements.
            /// @return a std::vector of std::strings with names of the published graph datas. 
            std::vector<std::string> GetNamesOfPublishedGraphDatas() const;

            /// @brief Get the last error
            /// @return String containing information on the last error
            std::string GetLastError();

            /// @brief Send a console log message to the websocket. 
            /// @param message - [in] - message to be published.
            /// @return -1 on error, 0 on success
            int8_t SendConsoleLog(const std::string& message);

        protected:
        private:

            /// @brief Hidden constructor
            Web_Server();

            /// @brief Hidden deconstructor
            ~Web_Server();

            /// @brief Blocking function that runs a while loop to poll the web server. 
            void Poll();

            /// @brief Getter to check if all necessary data is set (address, port, and root directory)
            /// @return true or false appropriately. 
            bool IsDataSet();

            /// @brief Event callback for the web server. 
            /// @param conn Mongoose connection
            /// @param event - [in] - event that is happening
            /// @param eventData - [in] - data for the event happening. 
            /// @param funcData - [in] - additional data. 
            static void eventCallback(mg_connection* conn, int event, void* eventData, void* funcData)
            {
                Web_Server* server = static_cast<Web_Server*>(funcData);

                if (event == MG_EV_OPEN)
                {
                    // Event "open" happened.
                }
                else if (event == MG_EV_POLL)
                {
                    // Do nothing - just a heartbeart
                }
                else if (event == MG_EV_HTTP_MSG)
                {
                    mg_http_message* hm = (mg_http_message*)eventData;

                    if (mg_http_match_uri(hm, "/ws"))
                    {
                        // Upgrade to websocket..
                        mg_ws_upgrade(conn, hm, NULL);
                        server->mWebsocketConnetion = conn;
                        server->mUpgraded = true;
                    }
                    else if (mg_http_match_uri(hm, "/hello"))
                    {
                        mg_http_reply(conn, 200, "Content-Type: text/plain\r\n", "Hello, %s\n", "world");
                    }
                    else
                    {
                        struct mg_http_serve_opts opts;
                        memset(&opts, 0, sizeof(opts));
                        opts.root_dir = rootAddress.c_str();
                        mg_http_serve_dir(conn, reinterpret_cast<mg_http_message*>(eventData), &opts);
                    }
                }
                else if (event == MG_EV_WS_MSG)
                {
                    mg_ws_message* wm = (mg_ws_message*)eventData;
                    std::string data = wm->data.ptr;
#ifdef CPP_TERMINAL
                    std::string result;
                    if (server->mTerminal->ExecuteCommand(data, result) >= 0)
                    {
                        std::cout << result << std::endl;
                        server->SendConsoleLog(result);
                    }
#else
                    server->SendConsoleLog("NOTICE: Terminal access not enabled");
#endif
                }
            }

            std::string                     mAddress;               // Address to spawn the server on.
            int16_t                         mPort;                  // Port to spawn the server on.
            std::string                     mRootDirectory;         // Root directory for the server files. 
            WebServerError                  mLastError;             // Last errror for web server class.
            bool                            mRunning;               // Bool if server is running. 
            mg_mgr                          mManager;               // Mongoose server manager.
            mg_connection*                  mConnection;            // Mongoose server connection.
            mg_connection*                  mWebsocketConnetion;    // Holds Mongoose websocket connection after its upgraded
            std::thread                     mThread;                // Thread for the server to run in.
            bool                            mUpgraded;              // flag for if the websocket has been upgraded. 
            static Web_Server*              mInstance;              // Pointer to the instance
            WebServerThreadPriority         mThreadPriority;        // Thread priority for windows.
            std::vector<PublishedFunction>  mFunctions;             // Vector of published functions to the webpage. 
            std::vector<PublishedData>      mDatas;                 // Vector of published data to the webpage. 
            std::vector<PublishedGraphData> mGraphDatas;            // Vector of published graph data to the webpage. 

#ifdef CPP_TERMINAL
            Essentials::Utilities::Terminal* mTerminal;    
#endif

#ifdef CPP_LOGGER
            Essentials::Utilities::Log* mLog;
#endif
        };
    }    // end Communications
}    // end Essentials

#endif // CPP_WEB_SERVER