#include "PostsContainer.h"

void PostsContainer::Clear()
{
	m_posts.clear();
}

void PostsContainer::AddPost(PostPtr post)
{
	uint32_t post_id = std::stoi(static_cast<std::string>((*post)["post_id"]));
	m_posts.insert({ post_id, post });
	m_postsOf[std::stoi(static_cast<std::string>(post->at("posted_by")))].insert(post);
	CreateInvertedIndex(post);
}

void PostsContainer::CreateInvertedIndex(PostPtr post)
{
	std::string_view s(post.get()->at("text"));

	size_t found = s.find("#");

	auto notAlfanumeric = [](unsigned char ch) { return !isalnum(ch); };

	while (found != std::string::npos)
	{
		auto endHashtag = std::find_if(s.begin() + found + 1, s.end(),
			notAlfanumeric);

		m_invertedIndex[std::string(s.begin() + found + 1, endHashtag)].insert(post);
		size_t startSearch = std::distance(s.begin(), endHashtag);
		found = s.find("#", startSearch);
	}

}

const std::set<PostsContainer::PostPtr, PostsContainer::compare> PostsContainer::GetPostsWithHashTag(const json& hashTag) const
{
	std::set<PostPtr, compare> allPosts;
	for (const std::string& htg : hashTag)
	{
		if (m_invertedIndex.find(htg) != m_invertedIndex.end())
		{
			for (const auto& post : m_invertedIndex.at(htg))
				allPosts.insert(post);
		}
	}

	return allPosts;
}

const std::set <PostsContainer::PostPtr, PostsContainer::compare>& PostsContainer::GetPostsOf(uint32_t userId) const
{
	try {
		return m_postsOf.at(userId);
	}
	catch (...)
	{
		return std::set <PostsContainer::PostPtr, PostsContainer::compare>();
	}
}

std::map<uint32_t, PostsContainer::PostPtr, std::greater<int>>& PostsContainer::GetAllPosts()
{
	return m_posts;
}
