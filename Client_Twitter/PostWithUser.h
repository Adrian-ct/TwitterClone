#pragma once
#include "Post.h"

class PostWithUser :
	public Post
{
public:
	PostWithUser(const std::string& text, uint32_t likes, uint32_t dislikes, std::optional<uint32_t> parentId,
		std::chrono::system_clock::time_point m_dateAndTimeOfCreation, Post::Reaction reaction, uint16_t idUser,
		const std::string& username, uint32_t post_id);
	PostWithUser() = default;
	PostWithUser(const PostWithUser& post);
	virtual ~PostWithUser() = default;

	friend std::ostream& operator<<(std::ostream& o, const PostWithUser& post);
	uint16_t GetIdUser()const;
	const std::string& GetUsername();

	void Print(std::ostream& o)const override;
private:
	uint16_t m_idUser;
	std::string m_username;
};

