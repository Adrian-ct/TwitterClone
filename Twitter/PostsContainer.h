#pragma once
#include <iostream>
#include <vector>
#include "nlohmann.hpp"
#include <unordered_map>
#include <set>
using namespace nlohmann;

class PostsContainer
{
public:
	using PostPtr = std::shared_ptr<json>;
	struct compare
	{
		bool operator()(PostPtr j1, PostPtr j2)const {
			return std::stoi(static_cast<std::string>(j1.get()->at("post_id"))) >
				std::stoi(static_cast<std::string>(j2.get()->at("post_id")));
		}
	};
public:
	PostsContainer() = default;
	void Clear();
	void AddPost(PostPtr post);
	const std::set<PostPtr, compare> GetPostsWithHashTag(const json& hashTag)const;
	const std::set < PostPtr, compare>& GetPostsOf(uint32_t userId) const;
	std::map<uint32_t, PostPtr, std::greater<int>>& GetAllPosts();
private:
	void CreateInvertedIndex(PostPtr post);
private:

	std::map<uint32_t, PostPtr, std::greater<int>> m_posts;
	std::map<uint32_t, std::set < PostPtr, compare>> m_postsOf;
	std::unordered_map < std::string, std::set < PostPtr, compare>>  m_invertedIndex;
};

