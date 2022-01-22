#include "Comment.h"
#include "Util.h"

Comment::Comment(const std::string& text, uint32_t userID,
	std::chrono::system_clock::time_point dateAndTimeOfCreation, const std::string& username, uint32_t commentId) :
	m_text(text), m_userID(userID),
	m_dateAndTimeOfCreation(dateAndTimeOfCreation),
	m_username(username),
	m_commentID(commentId)
{
}


Comment::Comment(Comment&& comment) noexcept :
	m_text(std::move(comment.m_text)),
	m_dateAndTimeOfCreation(std::move(comment.m_dateAndTimeOfCreation)),
	m_userID(std::move(comment.m_userID)),
	m_username(std::move(comment.m_username)),
	m_commentID(std::move(comment.m_commentID))
{
}

Comment::Comment(const Comment& comment)
{
	m_text = comment.m_text;
	m_dateAndTimeOfCreation = comment.m_dateAndTimeOfCreation;
	m_userID = comment.m_userID;

	m_username = comment.m_username;
	m_commentID = comment.m_commentID;
}

Comment& Comment::operator=(const Comment& comment)
{
	m_text = comment.m_text;
	m_dateAndTimeOfCreation = comment.m_dateAndTimeOfCreation;
	m_userID = comment.m_userID;
	m_username = comment.m_username;
	m_commentID = comment.m_commentID;
	return *this;
}

Comment& Comment::operator=(Comment&& comment) noexcept
{
	m_text = std::move(comment.m_text);
	m_dateAndTimeOfCreation = std::move(comment.m_dateAndTimeOfCreation);
	m_userID = std::move(comment.m_userID);
	m_username = std::move(comment.m_username);
	m_commentID = std::move(comment.m_commentID);

	comment.m_text = "";
	comment.m_userID = 0;
	comment.m_username = "";
	comment.m_commentID = 0;

	return *this;
}

const std::string& Comment::GetText() const
{
	return m_text;
}

const std::string& Comment::GetUsername() const
{
	return m_username;
}

const uint32_t& Comment::GetUserID() const
{
	return m_userID;
}

const uint32_t& Comment::GetCommentID() const
{
	return m_commentID;
}

const std::chrono::system_clock::time_point& Comment::GetDateAndTimeOfCreation() const
{
	return m_dateAndTimeOfCreation;
}

std::ostream& operator<<(std::ostream& out, const Comment& comment)
{
	out << "Comment Text: " << comment.m_text << "\n";
	out << "User Id: " << comment.m_userID << "\n";
	out << "Username: " << comment.m_username << "\n";
	out << "Date and time of creation: " << util::TimePointToString(comment.m_dateAndTimeOfCreation) << "\n";
	out << "Comment Id: " << comment.m_commentID << "\n";

	return out;
}
