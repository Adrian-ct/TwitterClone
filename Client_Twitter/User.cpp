#include "User.h"
#include "Socket.h"

//User::User(const std::string& username, const std::string& password)
//	:m_username(username)
//	, m_password(password)
//	, m_profile(nullptr)
//{
//	//should create here a new profile
//}

User::User(uint32_t id, const std::string& username, const std::string& firstName, const std::string& lastName,
	const std::tm& birthday, const std::chrono::system_clock::time_point& creationTime)
	: m_id(id)
	, m_username(username)
	, m_firstName(firstName)
	, m_lastName(lastName)
	, m_birthday(birthday)
	, m_creationTime(creationTime)
{
}

//User::User(User&& other) noexcept
//{
//	m_username = std::move(other.m_username);
//	m_password = std::move(other.m_password);
//	m_profile = other.m_profile;
//
//	other.m_username = "";
//	other.m_password = "";
//}
//
//User& User::operator=(User&& other) noexcept
//{
//	if (this != &other)
//	{
//		m_username = std::move(other.m_username);
//		m_password = std::move(other.m_password);
//		m_profile = other.m_profile;
//
//		other.m_username = "";
//		other.m_password = "";
//	}
//
//	return *this;
//}
//
//
//
//

const std::string& User::GetUsername() const
{
	return m_username;
}

const std::string& User::GetFirstName() const
{
	return m_firstName;
}

const std::string& User::GetLastName() const
{
	return m_lastName;
}

const std::tm& User::GetBirthday()
{
	return m_birthday;
}


const std::chrono::system_clock::time_point& User::GetCreationTime()
{
	return m_creationTime;
}

uint32_t User::GetId() const
{
	return m_id;
}

void User::SetUsername(const std::string& username)
{
	m_username = username;
}

void User::SetFirstName(const std::string& firstName)
{
	m_firstName = firstName;
}

void User::SetLastName(const std::string& lastName)
{
	m_lastName = lastName;
}

void User::SetBirthday(const std::tm& birthday)
{
	m_birthday = birthday;
}

void User::MakePost(const std::string& text, const  std::optional<uint32_t>& parentPost) const
{
	json post;
	post["text"] = text;
	post["posted_by"] = m_id;
	post["username"] = m_username;
	if (parentPost)
	{
		post["parent_post"] = parentPost.value();
	}
	else
	{
		post["parent_post"] = nullptr;
	}
	post["command"] = Socket::Command::POST;
	Socket::Get()->SendReceiveRequest(post);
}

void User::Follow(uint32_t userId)
{
	json follow;
	follow["follower_id"] = m_id;
	follow["followed_id"] = userId;
	follow["command"] = Socket::Command::FOLLOW;

	Socket::Get()->SendReceiveRequest(follow);
}

void User::Unfollow(uint32_t userId)
{
	json unfollow;
	unfollow["follower_id"] = m_id;
	unfollow["followed_id"] = userId;
	unfollow["command"] = Socket::Command::UNFOLLOW;

	Socket::Get()->SendReceiveRequest(unfollow);
}


std::vector<Post>& User::Posts()
{
	return m_posts;
}

std::unordered_map<uint32_t, PostWithUser>& User::ParentPosts()
{
	return m_parentPosts;
}

void User::LoadPosts(uint32_t loggedUser)
{
	json message;
	message["command"] = Socket::Command::LOAD_POSTS;
	message["user_id"] = static_cast<int>(m_id);
	if (m_id != loggedUser)
		message["loggedUser"] = loggedUser;

	json receivedMessage = Socket::Get()->SendReceiveRequest(message);

	if (receivedMessage.empty())
		return;


	std::vector<json> posts = receivedMessage["posts"];
	std::vector<json> parentPosts = receivedMessage["parent_posts"];

	//std::cout << std::setw(4) << receivedMessage << "\n";

	if (!m_posts.empty())
		m_posts.clear();

	for (const json& it : posts)
	{
		//std::cout << it.dump(4) << "\n";
		std::optional<int> parentId;

		if (it["parent_post"] == nullptr)
			parentId = std::nullopt;
		else
			parentId = std::stoi(static_cast<std::string>(it["parent_post"]));

		PostWithUser post(static_cast<std::string>(it["text"]), std::stoi(static_cast<std::string>(it["likes"])),
			std::stoi(static_cast<std::string>(it["dislikes"])), parentId, util::StringToTimePoint(it["post_time"]),
			static_cast<Post::Reaction>(std::stoi(static_cast<std::string>(it["reaction"]))),
			std::stoi(static_cast<std::string>(it["posted_by"])),
			static_cast<std::string>(it["username"]), std::stoi(static_cast<std::string>(it["post_id"])));

		if (it.find("comments") != it.end())
			for (auto& jComment : it["comments"])
			{
				Comment comment(static_cast<std::string>(jComment["text"]), std::stoi(static_cast<std::string>(jComment["user_id"])),
					util::StringToTimePoint(jComment["post_time"]), static_cast<std::string>(jComment["username"]),
					std::stoi(static_cast<std::string>(jComment["comment_id"])));

				post.AddComment(comment);
			}
		m_posts.push_back(post);
		//std::cout << post << "\n";
	}

	for (const json& it : parentPosts)
	{
		std::optional<int> parentId;
		//std::cout << it.dump(4) << "\n";

		if (it["parent_post"] == nullptr)
			parentId = std::nullopt;
		else
			parentId = std::stoi(static_cast<std::string>(it["parent_post"]));

		PostWithUser post(static_cast<std::string>(it["text"]), std::stoi(static_cast<std::string>(it["likes"])),
			std::stoi(static_cast<std::string>(it["dislikes"])), parentId, util::StringToTimePoint(it["post_time"]),
			static_cast<Post::Reaction>(std::stoi(static_cast<std::string>(it["reaction"]))),
			std::stoi(static_cast<std::string>(it["posted_by"])),
			static_cast<std::string>(it["username"]), std::stoi(static_cast<std::string>(it["post_id"])));

		if (it.find("comments") != it.end())
			for (auto& jComment : it["comments"])
			{
				Comment comment(static_cast<std::string>(jComment["text"]), std::stoi(static_cast<std::string>(jComment["user_id"])),
					util::StringToTimePoint(jComment["post_time"]), static_cast<std::string>(jComment["username"]),
					std::stoi(static_cast<std::string>(jComment["comment_id"])));

				post.AddComment(comment);
			}
		m_parentPosts.insert({ post.GetId(),post });
	}
}

json User::MembersToJson() const
{
	json members;
	members["username"] = m_username;

	members["password"] = "null";

	members["first_name"] = m_firstName;
	members["last_name"] = m_lastName;
	members["birthday"] = util::DateStructToString(m_birthday);
	members["creation_time"] = util::TimePointToString(m_creationTime, "UTC: %Y-%m-%d %H:%M:%S");
	return members;
}
