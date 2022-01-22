#pragma once
#include "MyDB.h"
#include<string>
class Validation
{
public:
	static bool ExistsUser(const MyDB& database,const std::string& username);
	static bool VerifyPassword(const MyDB& database,const std::string& username,const std::string& password);
};