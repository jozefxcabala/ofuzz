#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>
#include <mutex>
#include <ctime>
#include <assert.h>


enum LogPriority
{
	TracePriority, DebugPriority, InfoPriority, WarnPriority, ErrorPriority, CriticalPriority
};

class Logger
{
	LogPriority priority_ = InfoPriority; 
	std::mutex logMutex_;

	public:
		static void SetPriority(LogPriority newPriority)
		{
			instance().priority_ = newPriority;
		}

		template<typename... Args>
		static void Trace(int line, const char* sourceFile, const char* message, Args... args)
		{
			instance().log(line, sourceFile, "[Trace]\t", TracePriority, message, args...);
		}

		template<typename... Args>
		static void Debug(int line, const char* sourceFile, const char* message, Args... args)
		{
			instance().log(line, sourceFile, "[Debug]\t", DebugPriority, message, args...);
		}

		template<typename... Args>
		static void Info(int line, const char* sourceFile, const char* message, Args... args)
		{
			instance().log(line, sourceFile, "[Info]\t", InfoPriority, message, args...);
		}

		template<typename... Args>
		static void Warn(int line, const char* sourceFile, const char* message, Args... args)
		{
			instance().log(line, sourceFile, "[Warn]\t", WarnPriority, message, args...);
		}

		template<typename... Args>
		static void Error(int line, const char* sourceFile, const char* message, Args... args)
		{
			instance().log(line, sourceFile, "[Error]\t", ErrorPriority, message, args...);
		}

		template<typename... Args>
		static void Critical(int line, const char* sourceFile, const char* message, Args... args)
		{
			instance().log(line, sourceFile, "[Critical]\t", CriticalPriority, message, args...);
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
		void log(int lineNumber, const char* sourceFile, const char* messagePriorityStr, LogPriority messagePriority, const char* message, Args... args)
		{
			if (priority_ <= messagePriority)
			{
				std::time_t currentTime = std::time(0);
				std::tm* timestamp = std::localtime(&currentTime);
				char buffer[80];
				strftime(buffer, 80, "%c", timestamp);

				std::scoped_lock lock(logMutex_);
				printf("%s\t", buffer);
				printf(messagePriorityStr);
				printf(message, args...);
				printf(" on line %d in %s", lineNumber, sourceFile);
				printf("\n");
			}
		}
};


#define LOG_TRACE(Message, ...) (Logger::Trace(__LINE__, __FILE__, Message, ## __VA_ARGS__))
#define LOG_DEBUG(Message, ...) (Logger::Debug(__LINE__, __FILE__, Message, ## __VA_ARGS__))
#define LOG_INFO(Message, ...) (Logger::Info(__LINE__, __FILE__, Message, ## __VA_ARGS__))
#define LOG_WARN(Message, ...) (Logger::Warn(__LINE__, __FILE__, Message, ## __VA_ARGS__))
#define LOG_ERROR(Message, ...) (Logger::Error(__LINE__, __FILE__, Message, ## __VA_ARGS__))
#define LOG_CRITICAL(Message, ...) (Logger::Critical(__LINE__, __FILE__, Message, ## __VA_ARGS__))

#endif // LOGGER_H

// zdroj: https://github.com/danielblagy/Yelloger/tree/master/src