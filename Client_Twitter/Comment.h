#pragma once
#include<iostream>
#include<vector>
#include <chrono>

class Comment  
{
public:
	Comment() = default;
	Comment(const std::string& text, uint32_t userID, std::chrono::system_clock::time_point dateAndTimeOfCreation,
		const std::string& username, uint32_t commentId);
	Comment(Comment&& comment)noexcept;
	~Comment() = default;
	Comment(const Comment& comment);
	Comment& operator=(const Comment& comment);
	Comment& operator=(Comment&& comment)noexcept;

	const std::string& GetText()const;
	const std::string& GetUsername()const;
	const uint32_t& GetUserID()const;
	const uint32_t& GetCommentID()const;
	const std::chrono::system_clock::time_point& GetDateAndTimeOfCreation()const;
	friend std::ostream& operator << (std::ostream& out, const Comment& comment);

private:
	std::string m_text;
	uint32_t m_userID;
	uint32_t m_commentID;
	std::string m_username;
	std::chrono::system_clock::time_point m_dateAndTimeOfCreation;

	static const uint16_t m_textCharAllowed = 140;
};

