#include "PersonalProfile.h"

void PersonalProfile::LoadPosts(uint16_t userId)
{
	json message;
	message["command"] = Socket::Command::LOAD_POSTS;
	message["user_id"] =userId;
	json receivedMessage = Socket::Get()->SendReceiveRequest(message);
	//std::cout << std::setw(4) << receivedMessage << "\n";
	std::vector<Post> newPosts;
	if (receivedMessage.empty())
		return;

	if (!m_posts.empty())
		m_posts.clear();

	for (auto& it : receivedMessage)
	{
		std::optional<int> parentId;

		if (it["parent_post"] == nullptr)
			parentId = std::nullopt;
		else
			parentId = std::stoi(static_cast<std::string>(it["parent_post"]));

		Post post(static_cast<std::string>(it["text"]), std::stoi(static_cast<std::string>(it["likes"])),
			std::stoi(static_cast<std::string>(it["dislikes"])), parentId, (util::StringToTimePoint(it["post_time"])),
			static_cast<Post::Reaction>(std::stoi(static_cast<std::string>(it["reaction"]))),
			std::stoi(static_cast<std::string>(it["post_id"])));

		for (auto& it2 : it["comments"])
		{
			Comment comment(static_cast<std::string>(it2["text"]), std::stoi(static_cast<std::string>(it2["user_id"])),
				util::StringToTimePoint(it2["post_time"]), static_cast<std::string>(it2["username"]),
				std::stoi(static_cast<std::string>(it2["comment_id"])));

			post.AddComment(comment);
		}
		m_posts.push_back(post);
	}

	/*for (auto& it : m_posts)
		std::cout << it << "\n\n";*/
} 
bool PersonalProfile::HasNext(int16_t index)
{
	return ((index + 1) < m_posts.size());
}

bool PersonalProfile::HasPrevious(int16_t index)
{
	return ((index - 1) >= 0);
}

Post& PersonalProfile::ReturnPost(int16_t index)
{
	return m_posts[index];
}
bool PersonalProfile::HasPosts()
{
	return (m_posts.size() != 0);
}
void PersonalProfile::Clear()
{
	m_posts.clear();
}
