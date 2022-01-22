#pragma once
#include <vector>
#include "Post.h"
#include "Socket.h"
#include <iostream>
class PersonalProfile
{
public:
	PersonalProfile() = default;
	void LoadPosts(uint16_t userId);
	// true if there are any next posts left, false otherwise
	bool HasNext(int16_t index);
	// true if there are any previous posts left, false otherwise
	bool HasPrevious(int16_t index);
	// return the next post from posts
	Post& ReturnPost(int16_t index);
	
	// return true if m_posts has Posts in it
	bool HasPosts();
	void Clear();

private:

	std::vector<Post> m_posts;
	uint16_t m_userId;

};

