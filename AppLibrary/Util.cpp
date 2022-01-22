#include "Util.h"

namespace util
{
	std::string TimePointToString(const std::chrono::system_clock::time_point& timepoint, const std::string& format)
	{

		auto newTp = std::chrono::system_clock::to_time_t(timepoint);
		auto tp3 = *std::localtime(&newTp);

		tp3.tm_hour += 3;

		auto tp = std::chrono::system_clock::from_time_t(std::mktime(&tp3));
		auto s = std::format("{:%F %T}", tp);

		static_assert(std::is_same_v<decltype(s), std::string>, "");
		return s;
	}

	std::chrono::system_clock::time_point StringToTimePoint(const std::string& dateOfCreation, const std::string& format)
	{
		std::tm tm;
		std::stringstream ss(dateOfCreation);
		ss >> std::get_time(&tm, format.c_str());
		auto tp = std::chrono::system_clock::from_time_t(std::mktime(&tm));
		return tp;
	}

	std::tm StringToDateStruct(const std::string& birthday)
	{
		std::tm tm;
		std::stringstream ss(birthday);
		ss >> std::get_time(&tm, "%Y-%m-%d");
		return tm;
	}

	std::string DateStructToString(const std::tm& birthdayStruct)
	{
		return std::to_string(birthdayStruct.tm_year + 1900) + " " + std::to_string(birthdayStruct.tm_mon + 1) + " "
			+ std::to_string(birthdayStruct.tm_mday);
	}

	bool IsTimePoint(const std::string& timePoint)
	{
		//not working
		return true;
	}
}