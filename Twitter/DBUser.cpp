#include "DBUser.h"

void DBUser::Insert(MyDB& db, const json& user)const
{
	std::string statement;
	statement += "into User values(null,'";
	statement += user["username"];
	statement += "','";
	statement += user["password"];
	statement += "','";
	statement += user["first_name"];
	statement += "','";
	statement += user["last_name"];
	statement += "','";
	statement += user["birthday"];
	statement += "',CURRENT_TIMESTAMP)";

	db.Insert(statement);
}

json DBUser::Fetch(MyDB& db, const uint32_t id) const
{

	std::vector<json> records = db.Select
	("user_id,password,username,first_name,last_name,birthday,account_creation_time FROM User WHERE user_id=" + std::to_string(id));
	json user;


	if (!records.empty())
	{
		user = records[0];
		user["user_id"] = std::stoi(static_cast<std::string>(user["user_id"]));
		user["success"] = true;
	}
	else
	{
		user["success"] = false;
	}


	return user;
}

json DBUser::Fetch(MyDB& db, const std::string& username) const
{
	std::vector<json> records = db.Select
	("user_id,password,username,first_name,last_name,birthday,account_creation_time FROM User WHERE username='" + username + "'");
	json user;


	if (!records.empty())
	{
		user = records[0];
		user["user_id"] = std::stoi(static_cast<std::string>(user["user_id"]));
		user["success"] = 1;
	}
	else
	{
		user["success"] = 0;
	}


	return user;
}

std::vector<uint32_t> DBUser::FetchAllIds(const MyDB& db) const
{
	std::vector<json> users = db.Select("user_id FROM User");
	std::vector<uint32_t> userIds;
	userIds.reserve(users.size());

	for (const json& user : users)
	{
		userIds.push_back(std::stoi(static_cast<std::string>(user["user_id"])));
	}

	return userIds;
}

std::vector<json> DBUser::FetchAllIdsAndUsernames(const MyDB& db) const
{
	std::vector<json> users = db.Select("user_id,username FROM User");

	for (json& user : users)
	{
		user["user_id"] = std::stoi(static_cast<std::string>(user["user_id"]));
	}

	return users;
}

json DBUser::LogIn(MyDB& db, const std::string& username, const std::string& password)
{
	json user = Fetch(db, username);

	if (user["success"] == 1)
	{
		if (user["password"] == password)
		{
			user.erase("password");
		}
		else
		{
			user.clear();
			user["succes"] = 0;
		}
	}


	return user;
}
