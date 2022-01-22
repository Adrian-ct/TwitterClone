#pragma once
#include <iostream>
#include <vector>
#include <sstream>
#include "Comment.h"
//#include "App.h"
#include <list>
#include "nlohmann.hpp"
using namespace nlohmann;
class Post
{
public:
	enum class Reaction :uint8_t
	{
		EDefault,
		ELike,
		EDislike
	};

public:
	Post() = default;
	Post(const std::string& text, uint32_t likes, uint32_t dislikes, std::optional<uint32_t> parentId,
		std::chrono::system_clock::time_point m_dateAndTimeOfCreation, Reaction reaction, uint32_t post_id);
	Post(Post&& post)noexcept;
	virtual ~Post() = default;
	Post(const Post& post);
	Post& operator=(const Post& post);
	Post& operator=(Post&& post)noexcept;

	const uint32_t GetLikes()const;
	const uint32_t GetDislikes()const;
	const uint32_t GetId()const;
	const std::string& GetText()const;
	const std::list<Comment>& GetComments()const;
	const Reaction GetReaction()const;
	std::optional<uint32_t> GetParentId()const;
	void SetReaction(Reaction reaction);
	void SetReaction(Reaction reaction, int8_t likes, int8_t dislikes);
	json To_Json()const;

	const std::chrono::system_clock::time_point& GetDateAndTimeOfCreation()const;
	void AddComment(const Comment& comment);
	void AddCommentFirst(const Comment& comment);
	virtual void Print(std::ostream& o)const;

	friend std::ostream& operator<<(std::ostream& o, const Post& post);

private:
	std::list<Comment> m_comments;
	std::chrono::system_clock::time_point m_dateAndTimeOfCreation;
	std::string m_text;
	Reaction m_reaction;

	std::optional<uint32_t> m_parentId;

	uint32_t m_likes;
	uint32_t m_dislikes;
	uint32_t m_post_id;

	size_t m_indexComment;

	static const uint16_t m_textCharAllowed = 140;
};

