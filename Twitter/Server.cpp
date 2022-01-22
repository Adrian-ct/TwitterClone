#include "Server.h"
#include "DatabaseQueries.h"
#include "Util.h"
#include "Logger.h"
#include <fstream>

Server::Server() :m_keepRunning(true)
{
	try
	{
		m_db.Open("twitter.db");
		m_socialGraph.FetchData(m_db);
		
		std::cout << "Starting server" << std::endl;
		listener.Listen(27015);
		LoadPostContainer();
		std::cout << "All posts loaded in memory\n";
		//this->GenerateFeedFor(1);
		Start();

		std::cout << "Waiting for clients to connect" << std::endl;
	}
	catch (const char* message)
	{
		std::cout << message << "\n";
	}
}

Server::~Server()
{
	m_db.Close();

	//if is there any other thread still doing work when closing the program, wait for them to finish
	for (auto& t : m_threads)
	{
		if (t.joinable())
			t.join();
	}
}

void Server::Start()
{
	int activeThreads = 0;
	for (int i = 0; i < Max_Nr_Threads; i++)
		m_threads.emplace_back(std::thread(&Server::ThreadFunction, this));

	while (m_keepRunning)
	{
		std::unique_lock<std::mutex> lock(m_waitFreeThreads); 

		ClientPtr client = ClientPtr(listener.Accept());

		if (activeThreads >= Max_Nr_Threads) 
		{
			m_availableThreads.wait(lock);
			--activeThreads;
		}

		m_workMutex.lock();
		m_clients.push(std::move(client));
		m_workMutex.unlock();

		m_canWork.notify_one();
		++activeThreads;
	}
}

void Server::acceptRequest(ClientPtr client, std::mutex& m)
{
	std::chrono::steady_clock::time_point start, end;

	while (1)
	{
		std::array<char, 10000> receiveBuffer;
		int received;

		try {
			client->Receive(receiveBuffer.data(), receiveBuffer.size(), received);

			start = std::chrono::steady_clock::now();
		}
		catch (const char* msg)
		{
			std::cout << msg << "\n";
			break;
		}

		std::string receivedMessage(receiveBuffer.begin(), receiveBuffer.begin() + received);

		json command = json::parse(receivedMessage);

		json messageToClient;

		m_databaseQuerie.lock();
		chooseComand(command, messageToClient);
		m_databaseQuerie.unlock();

		client->Send(messageToClient.dump().c_str(), messageToClient.dump().size());
		end = std::chrono::steady_clock::now();

		std::cout << "Package sent to socket: " << client->getSocketName() <<
			" in " << std::chrono::duration_cast<std::chrono::microseconds> (end - start).count() << " microseconds \n";
	}
}

void Server::ThreadFunction()
{
	while (1)
	{
		std::unique_lock<std::mutex> lock(m_workMutex);
		ClientPtr client = nullptr;

		m_availableThreads.notify_all();

		if (m_clients.empty())
			m_canWork.wait(lock);

		client = std::move(m_clients.front());
		m_clients.pop();
		lock.unlock();

		if (client != nullptr)
			acceptRequest(std::move(client), std::ref(m_writeInConsole));
	}
}

void Server::chooseComand(json& msg, json& messageToUser)
{
	uint8_t intCommand = msg["command"];
	Command command = static_cast<Command>(intCommand);

	std::ofstream logFile;

	m_writeInConsole.lock();
	logFile.open("logs.txt", std::ios::app);

	Logger logger(logFile);

	logger.log(msg.dump(),Logger::Level::Info);

	m_writeInConsole.unlock();

	switch (command)
	{
	case Server::Command::SIGN_UP:
	{
		std::string username = msg["username"];

		if (dbq::ExistsUser(m_db, msg["username"]))
		{
			messageToUser["message"] = "Username alredy exists";
		}
		else
		{
			dbq::InsertUser(m_db, msg);
			messageToUser["message"] = "User created succesfully";
			std::vector<json> users = m_db.Select("user_id from User Order By user_id DESC Limit 1");
			m_socialGraph.InsertUser(std::stoi(static_cast<std::string>(users[0]["user_id"])),
				msg["username"]);
		}
		break;
	}
	case Server::Command::LOG_IN:
	{
		messageToUser = dbq::LogIn(m_db, msg["username"], msg["password"]);
		break;
	}
	case Server::Command::LOAD_POSTS:
	{
		auto& v = m_postContainer.GetPostsOf(msg["user_id"].get<int>());

		LikePosts(v, messageToUser, msg, m_postContainer);
	
		break;
	}
	case Server::Command::POSTS_WITH_HASHTAG:
	{
		auto& v = m_postContainer.GetPostsWithHashTag(msg["strings"]);

		LikePosts(v, messageToUser, msg, m_postContainer);

		break;
	}
	case Server::Command::POST:
	{
		dbq::MakePost(m_db, msg);
		msg.erase("command");
		std::vector<json> likes, dislikes;

		std::vector<json> p = m_db.Select("post_id, text, posted_by, parent_post, post_time, user.username FROM Post INNER JOIN User on posted_by = user.user_id ORDER BY post_id DESC LIMIT 1");
		p[0]["likes"] = "0";
		p[0]["dislikes"] = "0";

		m_postContainer.AddPost(std::make_shared<json>(std::move(p[0])));
		break;
	}
	case Server::Command::COMMENT:
	{
		uint32_t post_id = std::stoi(msg["comment"].at(0)["posted_on"].get<std::string>());
		auto& p = m_postContainer.GetAllPosts()[post_id];
		if (p)
		{
			std::string statement = "INTO Comment values(null, '";
			statement += msg["comment"].at(0).at("text");
			statement += "', ";
			statement += msg["comment"].at(0).at("posted_by");
			statement += ", ";
			statement += msg["comment"].at(0).at("posted_on");
			statement += ", '";
			statement += msg["comment"].at(0).at("post_time");
			statement += "')";

			m_db.Insert(statement);
			std::vector<json> comment = m_db.Select("comment.comment_id, comment.text, comment.post_time, user.username, user.user_id from Comment INNER JOIN User on user.user_id = commented_by Order By comment_id DESC Limit 1");
			messageToUser = comment[0];

			json postComments;
			PostsContainer::PostPtr postC = p;
			postComments.push_back(messageToUser);

			if ((*p).find("comments") == (*p).end())
				(*p)["comments"];

			for (auto& it : (*p)["comments"])
				postComments.push_back(it);

			(*p)["comments"] = (postComments);
		}
	}
	break;
	case Server::Command::REACT:
	{
		UpdateReaction(msg);
		break;
	}
	case Server::Command::FOLLOW:
	{
		dbq::Follow(m_db, msg["follower_id"], msg["followed_id"]);
		m_socialGraph.AddFollow(msg["follower_id"], msg["followed_id"]);
		break;
	}
	case Server::Command::UNFOLLOW:
	{
		dbq::Unfollow(m_db, msg["follower_id"], msg["followed_id"]);
		m_socialGraph.RemoveFollow(msg["follower_id"], msg["followed_id"]);
		break;
	}
	case Server::Command::IS_FOLLOWING:
	{
		messageToUser = m_socialGraph.IsFollowing(msg["follower_id"], msg["followed_id"]);
		break;
	}
	case Server::Command::FEED:
	{
		std::vector<PostsContainer::PostPtr> posts = GenerateFeedFor(msg["user_id"]);
		LikePosts(posts, messageToUser, msg, m_postContainer);

		break;
	}
	case Server::Command::USERNAME_SEARCH:
	{
		auto users = m_socialGraph.GetBestMatches(msg["username"], msg["how_many"]);
		messageToUser = users;
		break;
	}
	case Server::Command::GET_USER:
	{
		messageToUser = dbq::FetchUser(m_db, msg["user_id"].get<int>());
		messageToUser["follow_status"] = m_socialGraph.IsFollowing(msg["connected_user_id"], msg["user_id"]);
		break;
	}
	default:
		break;
	}
}

void Server::LoadPostContainer()
{
	m_postContainer.Clear();

	json posts = dbq::FetchAllPosts(m_db);
	for (auto& j : posts)
	{
		PostsContainer::PostPtr newPtr = std::make_shared<json>(std::move(j));
		m_postContainer.AddPost(newPtr);
	}
}

std::vector<PostsContainer::PostPtr> Server::GenerateFeedFor(uint32_t userId)
{
	using PostWithPriority = std::pair<PostsContainer::PostPtr, uint64_t>;

	auto cmp = [](PostWithPriority firstPost, PostWithPriority secondPost)
	{
		return firstPost.second > secondPost.second;
	};
	std::priority_queue< PostWithPriority, std::vector<PostWithPriority>, std::function<bool(PostWithPriority, PostWithPriority)>> q(cmp);

	auto followedFriends = m_socialGraph.GetFollowedUsers(userId, 2);


	auto now = std::chrono::system_clock::now();

	for (size_t depth = 1; depth < followedFriends.size(); depth++)
	{
		for (uint32_t friendId : followedFriends[depth])
		{
			const auto& posts = m_postContainer.GetPostsOf(friendId);
			if (posts.empty())
				continue;

			for (auto& post : posts)
			{
				auto distanceInTime = std::chrono::duration_cast<std::chrono::minutes> (now - util::StringToTimePoint(post->at("post_time")));
				uint64_t priority = depth * depth * distanceInTime.count();

				q.emplace(post, priority);

			}
		}
	}

	std::vector<PostsContainer::PostPtr> posts;

	while (!q.empty())
	{
		posts.emplace_back(q.top().first);
		q.pop();
	}

	return posts;
}

void Server::UpdateReaction(const json& msg)
{
	auto& p = m_postContainer.GetAllPosts()[std::stoi(static_cast<std::string>(msg["post_id"]))];
	if (p)
	{
		if (msg["action"] == "-1")
		{
			if (msg["reaction"] == "1")
			{
				uint32_t likes = std::stoi(static_cast<std::string>((*p)["likes"]));
				--likes;
				(*p)["likes"] = std::to_string(likes);
			}
			else
				if (msg["reaction"] == "2")
				{
					uint32_t dislikes = std::stoi(static_cast<std::string>((*p)["dislikes"]));
					--dislikes;
					(*p)["dislikes"] = std::to_string(dislikes);
				}

			m_db.Delete("FROM PostReaction where reacted_on = " + static_cast<std::string>((*p)["post_id"]) + std::string(" and reacted_by = ") +
				static_cast<std::string>(msg["user_id"]));
		}
		else
		{
			if (msg["reaction"] == "1")
			{
				int32_t likes = std::stoi(static_cast<std::string>((*p)["likes"]));
				++likes;
				(*p)["likes"] = std::to_string(likes);
			}
			else
				if (msg["reaction"] == "2")
				{
					uint32_t dislikes = std::stoi(static_cast<std::string>((*p)["dislikes"]));
					++dislikes;
					(*p)["dislikes"] = std::to_string(dislikes);
				}

			m_db.Insert("INTO PostReaction values (null, " + static_cast<std::string>(msg["post_id"]) + "," +
				static_cast<std::string>(msg["user_id"]) + "," + static_cast<std::string>(msg["reaction"]) + ")");

		}
	}
}
