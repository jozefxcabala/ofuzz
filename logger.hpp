#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>
#include <mutex>
#include <vector>
#include <ctime>
#include <assert.h>

enum LogPriority
{
	TracePriority, DebugPriority, InfoPriority, WarnPriority, ErrorPriority, CriticalPriority, CrashPriority
};

class Logger
{
	LogPriority priority_ = CrashPriority; 
	std::mutex logMutex_;

	public:
		std::vector<std::string> Colour{
			"\x1b[31m",
			"\x1b[32m",
			"\x1b[33m",
			"\x1b[34m",
			"\x1b[35m",
			"\x1b[36m",
			"\x1b[0m"
		};

	public:
		static void SetPriority(LogPriority newPriority)
		{
			instance().priority_ = newPriority;
		}

		template<typename... Args>
		static void Trace(int line, const char* sourceFile, int id,  const char* message, Args... args)
		{
			instance().log(line, sourceFile, "[Trace]\t", TracePriority, id, message, args...);
		}

		template<typename... Args>
		static void Debug(int line, const char* sourceFile, int id,  const char* message, Args... args)
		{
			instance().log(line, sourceFile, "[Debug]\t", DebugPriority, id, message, args...);
		}

		template<typename... Args>
		static void Info(int line, const char* sourceFile, int id,  const char* message, Args... args)
		{
			instance().log(line, sourceFile, "[Info]\t", InfoPriority, id, message, args...);
		}

		template<typename... Args>
		static void Warn(int line, const char* sourceFile, int id,  const char* message, Args... args)
		{
			instance().log(line, sourceFile, "[Warn]\t", WarnPriority, id, message, args...);
		}

		template<typename... Args>
		static void Error(int line, const char* sourceFile, int id, const char* message, Args... args)
		{
			instance().log(line, sourceFile, "[Error]\t", ErrorPriority, id, message, args...);
		}

		template<typename... Args>
		static void Critical(int line, const char* sourceFile, int id, const char* message, Args... args)
		{
			instance().log(line, sourceFile, "[Critical]\t", CriticalPriority, id, message, args...);
		}

		template<typename... Args>
		static void Crash(int line, const char* sourceFile, int id, const char* message, Args... args)
		{
			instance().log(line, sourceFile, "[Crash]\t", CrashPriority, id, message, args...);
		}

	private:
		Logger() {}

		Logger(const Logger&) = delete;
		Logger& operator= (const Logger&) = delete;

		~Logger() {}

		static Logger& instance()
		{
			static Logger logger;
			return logger;
		}

		template<typename... Args>
		void log(int lineNumber, const char* sourceFile, const char* messagePriorityStr, LogPriority messagePriority, int id, const char* message, Args... args)
		{
			if (priority_ <= messagePriority)
			{
				std::time_t currentTime = std::time(0);
				std::tm* timestamp = std::localtime(&currentTime);
				char buffer[80];
				strftime(buffer, 80, "%c", timestamp);

				std::scoped_lock lock(logMutex_);
				printf("%s", Colour[id % 7].c_str());
				printf("%s\t", buffer);
				printf("%s", messagePriorityStr);
				printf(message, args...);
				printf(" on line %d in %s", lineNumber, sourceFile);
				printf("\n");
				printf("%s", Colour[6].c_str());
			}
		}
};


#define LOG_TRACE(Id, Message, ...) (Logger::Trace(__LINE__, __FILE__,  Id,  Message, ## __VA_ARGS__))
#define LOG_DEBUG(Id, Message, ...) (Logger::Debug(__LINE__, __FILE__, Id,  Message, ## __VA_ARGS__))
#define LOG_INFO(Id, Message, ...) (Logger::Info(__LINE__, __FILE__, Id,  Message, ## __VA_ARGS__))
#define LOG_WARN(Id, Message, ...) (Logger::Warn(__LINE__, __FILE__, Id,  Message, ## __VA_ARGS__))
#define LOG_ERROR(Id, Message, ...) (Logger::Error(__LINE__, __FILE__, Id,  Message, ## __VA_ARGS__))
#define LOG_CRITICAL(Id, Message, ...) (Logger::Critical(__LINE__, __FILE__, Id,  Message, ## __VA_ARGS__))
#define LOG_APP(Id, Message, ...) (Logger::Crash(__LINE__, __FILE__, Id,  Message, ## __VA_ARGS__))

#endif // LOGGER_H

// zdroj: https://github.com/danielblagy/Yelloger/tree/master/src