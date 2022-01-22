#pragma once
#include <chrono>
#include <iomanip>
#include <sstream>
#include <regex>
#include <type_traits>
#include <iostream>
#include <string>

namespace util
{
	std::string TimePointToString(const std::chrono::system_clock::time_point&, const std::string& format = "%Y-%m-%d %H:%M:%S");
	std::chrono::system_clock::time_point StringToTimePoint(const std::string&, const std::string& format = "%Y-%m-%d %H:%M:%S");
	std::tm StringToDateStruct(const std::string& birthday);
	std::string DateStructToString(const std::tm& birthdayStruct);
	bool IsTimePoint(const std::string&);
}