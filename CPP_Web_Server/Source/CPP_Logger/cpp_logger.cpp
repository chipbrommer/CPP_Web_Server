///////////////////////////////////////////////////////////////////////////////
//!
//! @file		cpp_logger.cpp
//! 
//! @brief		Implementation of singleton Log class 
//! 
//! @author		Chip Brommer
//! 
//! @date		< 12 / 15 / 2022 > Initial Start Date
//!
/*****************************************************************************/

///////////////////////////////////////////////////////////////////////////////
//
//  Includes:
//          name                        reason included
//          --------------------        ---------------------------------------
#include	"cpp_logger.h"						// Log Class
//
///////////////////////////////////////////////////////////////////////////////

namespace Essentials
{
	namespace Utilities
	{
		// Initialize static class variables.
		Log* Log::mInstance = NULL;
		std::mutex Log::mMutex;

		Log* Log::GetInstance()
		{
			std::lock_guard<std::mutex> lock(Log::mMutex);
			if (mInstance == NULL)
			{
				mInstance = new Log;
			}

			return mInstance;
		}

		void Log::ReleaseInstance()
		{
			if (mInstance != NULL)
			{
				delete mInstance;
				mInstance = NULL;
			}
		}

		int Log::Initialize(bool enableConsoleLogging, bool enableFileLogging, std::string filename)
		{
			// Catch if already initialized. 
			if (mRunning)
			{
				return 0;
			}

			this->mUser					= "Log";
			this->mOutputFile			= filename;
			this->mConsoleOutputEnabled = enableConsoleLogging;
			this->mFileOutputEnabled	= enableFileLogging;
			this->mMaxConsoleLogLevel	= LOG_LEVEL::LOG_DEBUG;
			this->mMaxFileLogLevel		= LOG_LEVEL::LOG_DEBUG;
			this->mTimestampLevel		= LOG_TIME::LOG_MSEC;

#ifdef NO_TIMER
			uint32_t initStart			= 0;
#elif defined CPP_TIMER
			Timer* timer				= Timer::GetInstance();
			uint32_t initStart			= timer->GetMSecTicks();
#elif defined C_TIMER
			uint32_t initStart			= TIMER_GetMsecTicks();
#endif
			// Set up the output file if the logging is enabled. 
			if (enableFileLogging)
			{
				size_t i = filename.rfind('/', filename.length());
				if (i == std::string::npos)
				{
					printf_s("log path is not a valid path.\n");
					return -1;
				}
				std::string directoryPath = filename.substr(0, i);

				// make the directory if it doesn't exist
				struct stat st = { 0 };
				if (stat(directoryPath.c_str(), &st) == -1)
				{
					int made = 0;
#ifdef _WIN32
					made = _mkdir(directoryPath.c_str());
#else
					made = mkdir(directoryPath.c_str(), 0777);
#endif
					if (made == -1)
					{
						char buffer[256];
						strerror_s(buffer, sizeof(buffer), errno); // get string message from errno, XSI-compliant version
						printf_s("Error %s\n", buffer);
					}
				}

				// Create the datetime stamp for the file creation
				auto now = std::chrono::system_clock::now();
				char time_str[] = "yyy.mm.dd.HH-MM.SS.fff";
				time_t ttime_t = std::chrono::system_clock::to_time_t(now);
				std::tm ttm = { 0 };
				localtime_s(&ttm, &ttime_t);
				strftime(time_str, strlen(time_str), "%Y.%m.%d-%H.%M.%S", &ttm);
				std::chrono::system_clock::time_point tp_sec = std::chrono::system_clock::from_time_t(ttime_t);
				int64_t ms = std::chrono::duration_cast<std::chrono::milliseconds>(now - tp_sec).count();

				std::string milliseconds = std::to_string(ms);
				if (milliseconds.length() < 3)
				{
					milliseconds.insert(0, 3 - milliseconds.length(), '0');
				}

				// Create the file and verify its open - if successful start the writing thread.
				mFile.open(filename + "_" + std::string(time_str) + "." + milliseconds + ".txt");
				if (!mFile.is_open())
				{
					printf_s("Error creating log file [%s].\n", filename.c_str());
				}
			}

			// Set up the logging thread.
			mThread = new std::thread(&Log::WriteOut, this);

			// Successful initialization
			mRunning = true;

#ifdef NO_TIMER
			AddEntry(LOG_LEVEL::LOG_INFO, mUser, "Initialize Complete - Using NO_TIMER.");
#elif defined CPP_TIMER
			AddEntry(LOG_LEVEL::LOG_INFO, mUser, "Initialize Complete - Using CPP_TIMER.");
#elif defined C_TIMER
			AddEntry(LOG_LEVEL::LOG_INFO, mUser, "Initialize Complete - Using C_TIMER.");
#endif
			std::string logString = enableFileLogging ? LevelMap[mMaxFileLogLevel].c_str() : "Not Enabled!";
			AddEntry(LOG_LEVEL::LOG_INFO, mUser, "File Log Level:    %s", logString);
			logString = enableConsoleLogging ? LevelMap[mMaxConsoleLogLevel].c_str() : "Not Enabled!";
			AddEntry(LOG_LEVEL::LOG_INFO, mUser, "Console Log Level: %s", logString);
			AddEntry(LOG_LEVEL::LOG_INFO, mUser, "Time Type:         %s", TimeMap[mTimestampLevel].c_str());

			return 1;
		}

		bool Log::AddEntry(LOG_LEVEL level, std::string user, std::string format, ...)
		{
			va_list args;
			char msg[MAX_LOG_MESSAGE_LENGTH + 1];
			char ts[20];

			// Format the message timestamp
			switch (mTimestampLevel)
			{
#if !defined NO_TIMER
			case LOG_TIME::LOG_MSEC:
			{
#if defined CPP_TIMER
				Timer* timer = Timer::GetInstance();
				snprintf(ts, sizeof(ts), "[%7u] ", (unsigned int)timer->GetMSecTicks());
#elif defined C_TIMER
				snprintf(ts, sizeof(ts), "[%7u] ", (unsigned int)TIMER_GetMsecTicks());
#endif
				break;
			}
			case LOG_TIME::LOG_USEC:
			{
#if defined CPP_TIMER
				Timer* timer = Timer::GetInstance();
				uint32_t t = timer->GetUSecTicks();
				snprintf(ts, sizeof(ts), "[%7u.%03u] ", t / 1000, t % 1000);
#elif defined C_TIMER
				uint32_t t = TIMER_GetUsecTicks();
				snprintf(ts, sizeof(ts), "[%7u.%03u] ", t / 1000, t % 1000);
#endif
				break;
			}
#endif
			default:
			{
#ifdef _WIN32
				strcpy_s(ts, sizeof(ts), "");
#else
				strcpy(ts, "");
#endif
			}
			}

			// Format the message with args
			va_start(args, format);
#ifdef _WIN32
			vsnprintf_s(msg, sizeof(msg), MAX_LOG_MESSAGE_LENGTH, format.c_str(), args);
#else
			vsnprintf(msg, MAX_LOG_MESSAGE_LENGTH, format.c_str(), args);
#endif
			msg[sizeof(msg) - 1] = '\0';
			va_end(args);

			// Log to console if enabled and within the max
			if (mConsoleOutputEnabled && level <= mMaxConsoleLogLevel)
			{
#ifdef _WIN32
				char buf[400];
				snprintf(buf, sizeof(buf), "%s - %s - %s\n", ts, user.c_str(), msg);
				OutputDebugStringA(buf);    // goes to the debug console
				printf_s("%s", buf);
#else
				printf("%s - %s - %s\n", ts, user, msg);
#endif
			}

			// Log to file if enabled, within the max, and file is open. 
			if (	mFileOutputEnabled 
				&& level <= mMaxFileLogLevel
				&& mFile.is_open())
			{
				mMutex.lock();
				char buffer[400];
				snprintf(buffer, sizeof(buffer), "%s - %s - %s", ts, user.c_str(), msg);
				std::string tmp = buffer;
				mQueue.push(tmp);
				mMutex.unlock();
			}

			return true;
		}

		void Log::WriteOut()
		{
			while (mRunning)
			{
				std::string entry;

				mMutex.lock();
				if (mFile.is_open() && !mQueue.empty())
				{
					entry = mQueue.front();
					mQueue.pop();
				}
				mMutex.unlock();

				if (entry.length() > 0)
				{
					mFile << entry << "\n";
					mFile.flush();

					entry.clear();
				}

#if defined NO_TIMER
#ifdef _WIN32
				Sleep(1);
#else
				usleep(1000);
#endif
#elif defined CPP_TIMER
				Timer* timer = Timer::GetInstance();
				timer->MSecSleep(1);
#elif defined C_TIMER
				TIMER_MsecSleep(1);
#endif
			}
		}

		bool Log::SetConsoleLogLevel(LOG_LEVEL level)
		{
			mMaxConsoleLogLevel = level;
			return (level == mMaxConsoleLogLevel);
		}

		bool Log::SetFileLogLevel(LOG_LEVEL level)
		{
			mMaxFileLogLevel = level;
			return (level == mMaxFileLogLevel);
		}

		bool Log::SetLogTimestampLevel(LOG_TIME tsLevel)
		{
			mTimestampLevel = tsLevel;
			return (tsLevel == mTimestampLevel);
		}

		bool Log::LogToConsole(bool enable)
		{
			mConsoleOutputEnabled = enable;
			return (enable == mConsoleOutputEnabled);
		}

		bool Log::LogToFile(bool enable)
		{
			mFileOutputEnabled = enable;
			return (enable == mFileOutputEnabled);
		}

#ifdef WIN32
		int8_t Log::SetLoggerThreadPriority(LogThreadPriority priority)
		{
			if (!SetThreadPriority(mThread->native_handle(), (int)priority))
			{
				return -1;
			}

			return 0;
		}
#else
		int8_t Log::SetServerThreadPriority(int8_t priority)
		{
			int8_t minPriority = GetMinThreadPriorityValue();
			int8_t maxPriority = GetMaxThreadPriorityValue();

			if (priority < minPriority || priority > maxPriority)
			{
				return -1;
			}

			int policy = 0;
			sched_param schedule{};

			if (pthread_getschedparam(mThread->native_handle(), &policy, &schedule) != 0)
			{
				return -1;
			}

			if (pthread_setschedparam(mThread->native_handle(), policy, &schedule) != 0)
			{
				return -1;
			}

			return 0;
		}

		int8_t Log::GetMinThreadPriorityValue()
		{
			return sched_get_priority_min(SCHED_OTHER);
		}

		int8_t Log::GetMaxThreadPriorityValue()
		{
			return sched_get_priority_max(SCHED_OTHER);
		}
#endif

		Log::~Log()
		{
			// Notify close and wait for queue to finish writing to file
			AddEntry(LOG_LEVEL::LOG_INFO, mUser, "Closing.");
			while (!mQueue.empty()) {}

			mRunning = false;
			mThread->join();
			delete mThread;
			mFile.close();
		}

		Log::Log()
		{
			mThread = nullptr;
			mMaxConsoleLogLevel = LOG_LEVEL::LOG_NONE;
			mMaxFileLogLevel = LOG_LEVEL::LOG_NONE;
			mTimestampLevel = LOG_TIME::LOG_NONE;
			mConsoleOutputEnabled = false;
			mFileOutputEnabled = false;
			mOutputFile = "";
			mRunning = false;
			mUser = "";
		}
	} // Utilities
} // Essentials