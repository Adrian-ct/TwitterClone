#include "SocialGraph.h"
#include "DatabaseQueries.h"
#include <unordered_set>

SocialGraph::SocialGraph(const MyDB& db)
{
	FetchData(db);
}

void SocialGraph::FetchData(const MyDB& db)
{
	std::vector<json> users = dbq::FetchAllUserIdsAndUsernames(db);


	for (const json& user : users)
	{
		m_profiles.insert({ user["user_id"] ,Profile(user["username"]) });
	}

	std::vector<json> relationships = db.Select("follower,followed FROM Follow");

	for (const json& reationship : relationships)
	{
		m_profiles[std::stoi(static_cast<std::string>(reationship["follower"]))].friends.
			insert(std::stoi(static_cast<std::string>(reationship["followed"])));
	}
}

void SocialGraph::PrintData()
{
	for (const auto& [follower, profile] : m_profiles)
	{
		std::cout << profile.name << " -> ";

		for (SocialGraph::UserId followed : profile.friends)
		{
			std::cout << m_profiles[followed].name << " ";
		}
		std::cout << "\n";
	}
}

std::vector<std::pair<SocialGraph::UserId, std::string>> SocialGraph::GetBestMatches(const std::string& username, uint32_t howMany)
{
	using UserAndPriority = std::pair<std::uint32_t, uint32_t>;

	std::vector<UserAndPriority> users;
	users.reserve(m_profiles.size());

	for (const auto& [user_id, profile] : m_profiles)
	{
		uint32_t distance = alg::LevenshteinDistance(username, profile.name);
		users.emplace_back(user_id, distance);
	}

	auto comp = [](const UserAndPriority& firstUser, const UserAndPriority& secondUser) -> bool {return firstUser.second > secondUser.second; };
	std::priority_queue<UserAndPriority, std::vector<UserAndPriority>, std::function<bool(UserAndPriority, UserAndPriority)>>
		pq(comp, std::move(users));


	std::vector<std::pair<SocialGraph::UserId, std::string>> bestMatches;
	bestMatches.reserve(howMany);

	for (uint32_t i = 0; i < howMany && !pq.empty(); i++)
	{
		bestMatches.push_back({ pq.top().first , m_profiles[pq.top().first].name });
		pq.pop();
	}

	return bestMatches;
}

std::vector<std::vector<SocialGraph::UserId>> SocialGraph::GetFollowedUsers(UserId userId, uint32_t depth)
{

	std::vector<std::vector<UserId>> followed;
	std::unordered_set<UserId> visited;

	followed.emplace_back();
	followed[0].emplace_back(userId);

	visited.insert(userId);

	while (followed.size() <= depth)
	{
		followed.emplace_back();
		const std::vector<UserId>& currentVector = followed[followed.size() - 2];

		for (UserId currentUserId : currentVector)
		{
			for (UserId nextUserId : m_profiles[currentUserId].friends)
			{
				if (visited.find(nextUserId) == visited.end())
				{
					followed.back().emplace_back(nextUserId);
					visited.insert(nextUserId);
				}
			}
		}
	}


	return followed;
}

void SocialGraph::AddFollow(UserId firstUserId, UserId secondUserId)
{
	m_profiles[firstUserId].friends.insert(secondUserId);
}

void SocialGraph::RemoveFollow(UserId firstUserId, UserId secondUserId)
{
	m_profiles[firstUserId].friends.erase(secondUserId);
}

bool SocialGraph::IsFollowing(UserId follower, UserId followed)
{
	if (m_profiles.find(follower)->second.friends.find(followed) == m_profiles.find(follower)->second.friends.end())
	{
		return false;
	}
	return true;
}

void SocialGraph::InsertUser(uint32_t user_id, const std::string& username)
{
	m_profiles.insert({ user_id ,Profile(username) });
}

SocialGraph::Profile::Profile(const std::string& name)
	:name(name)
{
}
