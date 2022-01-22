#pragma once
#include <vector>
#include <unordered_map>
#include "MyDB.h"
#include <utility>
#include "Alg.h"
#include <queue>
#include <unordered_set>

class SocialGraph
{
public:

	using  UserNameAndFriends = std::pair<std::string, std::vector<uint32_t>>;
	using UserId = uint32_t;


	SocialGraph() = default;
	SocialGraph(const MyDB& db);
	void FetchData(const MyDB& db);
	void PrintData();

	std::vector<std::pair<SocialGraph::UserId, std::string>> GetBestMatches(const std::string& username, uint32_t howMany = 1);
	std::vector<std::vector<UserId>> GetFollowedUsers(UserId userId, uint32_t depth);
	void AddFollow(UserId firstUserId, UserId secondUserId);
	void RemoveFollow(UserId firstUserId, UserId secondUserId);
	bool IsFollowing(UserId follower, UserId followed);
	void InsertUser(uint32_t user_id, const std::string& username);

private:

	struct Profile
	{
		std::string name;
		std::unordered_set<uint32_t> friends;

		Profile() = default;
		Profile(const std::string& name);
	};

	std::unordered_map<UserId, Profile> m_profiles;
};