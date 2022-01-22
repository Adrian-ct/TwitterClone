#pragma once
#include <iostream>
#include <sqlite3.h>
#include <vector>
#include "nlohmann.hpp"
#include "TcpSocket.h"
#include <array>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include "MyDB.h"
#include "SocialGraph.h"
#include "PostsContainer.h"
using namespace nlohmann;

class Server
{
public:
	using Record = std::vector<std::string>;
	using Records = std::vector<Record>;
	using ClientPtr = std::unique_ptr<TcpSocket>;

	const int Max_Nr_Threads = 1;

	enum class Command : uint8_t
	{
		SIGN_UP,
		LOG_IN,
		POST,
		COMMENT,
		REACT,
		FOLLOW,
		UNFOLLOW,
		IS_FOLLOWING,
		LOAD_POSTS,
		POSTS_WITH_HASHTAG,
		FEED,
		USERNAME_SEARCH,
		GET_USER
	};

public:
	Server();
	~Server();
	void Start();

private:
	std::vector<std::thread> m_threads;
	std::queue<ClientPtr> m_clients;

	TcpSocket listener;

	std::mutex m_workMutex;
	std::mutex m_waitFreeThreads;
	std::mutex m_writeInConsole;
	std::mutex m_databaseQuerie;

	std::condition_variable m_canWork;
	std::condition_variable m_availableThreads;

	MyDB m_db;
	SocialGraph m_socialGraph;
	PostsContainer m_postContainer;

	bool m_keepRunning;
private:

	void acceptRequest(ClientPtr client, std::mutex& m);
	void ThreadFunction();
	void chooseComand(json& command, json& messageToUser);

	void LoadPostContainer();

	template <class T>
	void LikePosts(T& container, json& messageToUser, json& msg, PostsContainer& m_postContainer);

private:
	std::vector<PostsContainer::PostPtr> GenerateFeedFor(uint32_t userId);
	void UpdateReaction(const json& msg);
};

template<class T>
inline void Server::LikePosts(T& m_posts, json& messageToUser, json& msg, PostsContainer& m_postContainer)
{
	if (m_posts.empty())
		return;

	std::vector<json> postsLiked;

	std::vector<json> parent_posts;
	std::vector<json> posts;
	int loggedUser;

	if (msg.find("loggedUser") == msg.end())
		loggedUser = msg["user_id"].get<int>();
	else
		loggedUser = msg["loggedUser"].get<int>();

	postsLiked = m_db.Select("reacted_on, reaction_type FROM PostReaction where reacted_by=" +
		std::to_string(loggedUser));

	std::unordered_map<int, uint8_t> postsMap;

	for (auto& it : postsLiked)
		postsMap.insert({ std::stoi(static_cast<std::string>(it["reacted_on"])),
			std::stoi(static_cast<std::string>(it["reaction_type"])) });

	std::for_each(m_posts.begin(), m_posts.end(), [&posts, &parent_posts, &postsMap, &m_postContainer](PostsContainer::PostPtr ptr)
		{
			json aux;

			aux = (*ptr);
			aux["reaction"];

			if (postsMap.find(std::stoi(static_cast<std::string>((*ptr)["post_id"]))) != postsMap.end())
				aux.at("reaction") = std::to_string(postsMap[std::stoi(static_cast<std::string>((*ptr)["post_id"]))]);
			else
				aux.at("reaction") = "0";

			if (aux["parent_post"] != nullptr)
			{
				parent_posts.push_back(*(m_postContainer.GetAllPosts().at(std::stoi(static_cast<std::string>(aux["parent_post"])))));

				if (postsMap.find(std::stoi(static_cast<std::string>(parent_posts.back()["post_id"]))) != postsMap.end())
					parent_posts.back()["reaction"] = std::to_string(postsMap[std::stoi(static_cast<std::string>(parent_posts.back()["post_id"]))]);
				else
					parent_posts.back()["reaction"] = "0";
			}
			posts.push_back(aux);

		});

	messageToUser["posts"] = posts;
	messageToUser["parent_posts"] = parent_posts;
}
