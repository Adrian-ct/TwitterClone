#include "pch.h"
#include "Logger.h"
#include <iomanip>
Logger::Logger(std::ostream& os, Level minimumLevel)
	: m_os(os), minimumLevel(minimumLevel)
{
}

void Logger::log(const std::string& message, Level level)
{
	auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	if (level >= minimumLevel)
	{
		switch (level)
		{
		case Logger::Level::Info:
			m_os << "[" << std::put_time(std::localtime(&now), "%Y-%m-%d %X") << "] [INFO] "<<message<<"\n";
			break;
		case Logger::Level::Warning:
			m_os << "[" << std::put_time(std::localtime(&now), "%Y-%m-%d %X") << "] [WARNING] " << message<<"\n";
			break;
		case Logger::Level::Error:
			m_os << "[" << std::put_time(std::localtime(&now), "%Y-%m-%d %X") << "] [ERROR] " << message<<"\n";
			break;
		default:
			break;
		}
	}
}

void Logger::setMinimumLogLevel(Level minimumLevel)
{
	this->minimumLevel = minimumLevel;
}

