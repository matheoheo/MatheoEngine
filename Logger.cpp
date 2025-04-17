#include "pch.h"
#include "Logger.h"

ksn::Logger::Logger(const std::string_view pathToFile)
	:mSaveToFile(false),
	mPrintToConsole(true)
{
	setLogFile(pathToFile);
}

ksn::Logger::~Logger()
{
	std::scoped_lock<std::mutex> lock(mLoggerMutex);
	if(mFile.is_open())
		mFile.close();
}

void ksn::Logger::setSaveToFile(bool saveToFile)
{
	std::scoped_lock lock(mLoggerMutex);
	mSaveToFile = saveToFile;
}

void ksn::Logger::setLogFile(const std::string_view pathToFile)
{
	if (pathToFile.empty())
		return;

	std::scoped_lock lock(mLoggerMutex);
	if (mFile)
		mFile.close();
	mFile.open(pathToFile, std::ios::out | std::ios::app);
}

void ksn::Logger::setPrintToConsole(bool printToConsole)
{
	std::scoped_lock lock(mLoggerMutex);
	mPrintToConsole = printToConsole;
}
