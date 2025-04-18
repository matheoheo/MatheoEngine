#pragma once
#include <string>
#include <fstream>
#include "ConceptDefinitions.h"

/*
	Logger.hpp
	ksn Engine Logging System.

	Provides simple, thread safe logging with optional file output
	Supports log levels(Info, Warning, Error)
	Example usage:
		ksn::gLogger("Example Log", ksn::LOG_WARNING)
*/
namespace ksn
{
	class Logger
	{
	public:
		enum class LogType
		{
			Info,
			Warning,
			Error
		};

		Logger(const std::string_view pathToFile);
		~Logger();

		//Sets flag that determines if a save to file will be made.
		void setSaveToFile(bool saveToFile);

		//sets logger file. Can be reused multiple times.
		void setLogFile(const std::string_view pathToFile);

		//Sets flag that determines if a print to console is going to be made.
		void setPrintToConsole(bool printToConsole);

		//Param 'message' must satisfy Stremable concept - it requires, that operator '<<' is defined for T.
		//For better explaination of Stremable concept - see ConceptDefinitions.h
		template <Streamable T>
		void operator()(const T& message, LogType type = LogType::Info);
	private:
		std::ofstream mFile;
		std::mutex mLoggerMutex;
		bool mSaveToFile;
		bool mPrintToConsole;
	};

	constexpr Logger::LogType LOG_INFO    = Logger::LogType::Info;
	constexpr Logger::LogType LOG_WARNING = Logger::LogType::Warning;
	constexpr Logger::LogType LOG_ERROR   = Logger::LogType::Error;
	
	template<Streamable T>
	inline void Logger::operator()(const T& message, LogType type)
	{
		std::string_view logTypeMsg =
			(type == LOG_INFO ? "[INFO]" :
			type == LOG_WARNING ? "[WARNING]" : "[ERROR]");

		std::ostringstream oss;
		oss << logTypeMsg << " " << message;
		std::string fullMsg = oss.str();

		{
			std::scoped_lock lock(mLoggerMutex);
			if(mPrintToConsole)
				std::cout << fullMsg;

			if (mSaveToFile && mFile)
			{
				mFile << fullMsg;
				if (type == LOG_ERROR)
					mFile.flush();
			}
		}
	}

	inline Logger gLogger("logs.txt");
}

