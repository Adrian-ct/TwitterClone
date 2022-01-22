#pragma once
#include "MyDB.h"
#include <unordered_set>

class dbq
{
public:
	static void InsertUser(MyDB& db, const json& user);

	static json FetchUser(MyDB& db, const uint32_t id);
	static json FetchUser(MyDB& db, const std::string& username);
	static std::vector<json> FetchAllUserIdsAndUsernames(const MyDB& db);
	static json LogIn(MyDB& db, const std::string& username, const std::string& password);
	 
	static json FetchAllPosts(MyDB& db);
	static json FetchOnePost(MyDB& db, std::string idPost);
	static void MakePost(MyDB& db, json post);

	static void Follow(MyDB& db, uint32_t followerId, uint32_t followedId);
	static void Unfollow(MyDB& db, uint32_t followerId, uint32_t followedId);
	static bool IsFollowing(MyDB& db, uint32_t followerId, uint32_t followedId);

	static bool ExistsUser(const MyDB& database, const std::string& username);
	static bool VerifyPassword(const MyDB& database, const std::string& username, const std::string& password);
};