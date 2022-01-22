#include "Post.h"
#include "Util.h"

const uint16_t Post::m_textCharAllowed;

Post::Post(const std::string& text, uint32_t likes, uint32_t dislikes, std::optional<uint32_t> parentId,
	std::chrono::system_clock::time_point dateAndTimeOfCreation, Post::Reaction reaction, uint32_t post_id) :
	m_text(text), m_likes(likes), m_dislikes(dislikes), m_parentId(parentId),
	m_dateAndTimeOfCreation(dateAndTimeOfCreation),
	m_reaction(reaction),
	m_post_id(post_id)
{
}

Post::Post(Post&& post)noexcept :
	m_text(std::move(post.m_text)),
	m_dateAndTimeOfCreation(std::move(post.m_dateAndTimeOfCreation)),
	m_dislikes(std::move(post.m_dislikes)),
	m_likes(std::move(post.m_likes)),
	m_parentId(std::move(post.m_parentId)),
	m_reaction(std::move(post.m_reaction)),
	m_post_id(std::move(post.m_post_id))
{
	std::swap(m_comments, post.m_comments);
}

Post::Post(const Post& post)
{
	m_text = post.m_text;
	m_dateAndTimeOfCreation = post.m_dateAndTimeOfCreation;
	m_dislikes = post.m_dislikes;
	m_likes = post.m_likes;
	m_parentId = post.m_parentId;
	m_reaction = post.m_reaction;
	m_post_id = post.m_post_id;

	for (auto& it:post.m_comments)
		m_comments.push_back(it);
}

Post& Post::operator=(const Post& post)
{
	m_text = post.m_text;
	m_dateAndTimeOfCreation = post.m_dateAndTimeOfCreation;
	m_dislikes = post.m_dislikes;
	m_likes = post.m_likes;
	m_parentId = post.m_parentId;
	m_reaction = post.m_reaction;
	m_post_id = post.m_post_id;

	for (auto& it : post.m_comments)
		m_comments.push_back(it);

	return *this;
}

Post& Post::operator=(Post&& post)noexcept
{
	m_text = std::move(post.m_text);
	m_dateAndTimeOfCreation = std::move(post.m_dateAndTimeOfCreation);
	m_dislikes = std::move(post.m_dislikes);
	m_likes = std::move(post.m_likes);
	m_parentId = std::move(post.m_parentId);
	m_reaction = std::move(post.m_reaction);
	m_post_id = std::move(post.m_post_id);

	std::swap(m_comments, post.m_comments);
	post.m_text = "";
	post.m_dislikes = 0;
	post.m_likes = 0;
	post.m_parentId = std::nullopt;
	return *this;
}

const uint32_t Post::GetLikes() const
{
	return m_likes;
}

const uint32_t Post::GetDislikes() const
{
	return m_dislikes;
}

const uint32_t Post::GetId() const
{
	return m_post_id;
}

const std::list<Comment>& Post::GetComments() const
{
	return m_comments;
}

const Post::Reaction Post::GetReaction() const
{
	return m_reaction;
}

std::optional<uint32_t> Post::GetParentId() const
{
	return m_parentId;
}

void Post::SetReaction(Reaction reaction)
{
	m_reaction = reaction;
}

void Post::SetReaction(Reaction reaction, int8_t likes, int8_t dislikes)
{
	m_reaction = reaction;
	if ((likes == -1 && m_likes > 0) || likes > 0)
		m_likes += likes;


	if ((dislikes == -1 && m_dislikes > 0) || dislikes > 0)
		m_dislikes += dislikes;
}

json Post::To_Json() const
{
	//json post;
	//post["parent_post"] = nullptr;
	//post["text"] = m_text;
	//post["likes"] = std::to_string(m_likes);
	//post["dislikes"] = std::to_string(m_dislikes);
	//post["post_time"] = util::TimePointToString(m_dateAndTimeOfCreation);
	//post["reaction"] = std::to_string(static_cast<int>(m_reaction));
	//post["posted_by"] = std::to_string(App::Get()->GetUser()->GetId());
	//post["post_id"] = std::to_string(m_post_id);
		return json();
}

const std::string& Post::GetText() const
{
	return m_text;
}

const std::chrono::system_clock::time_point& Post::GetDateAndTimeOfCreation() const
{
	return m_dateAndTimeOfCreation;
}

void Post::AddComment(const Comment& comment)
{
	m_comments.push_back(comment);
}

void Post::AddCommentFirst(const Comment& comment)
{
	m_comments.push_front(comment);
}

void Post::Print(std::ostream& o)const
{
	o << "Text Post: " << m_text << "\n";
	o << "Likes: " << m_likes << "\n";
	o << "Dislikes: " << m_dislikes << "\n";
	o << "Reaction: " << static_cast<int>(m_reaction) << "\n";
	o << "Parent Post: ";

	if (m_parentId.has_value())
		o << m_parentId.value() << "\n";
	else
		o << "0\n";

	o << "Date and time of creation: " << util::TimePointToString(m_dateAndTimeOfCreation) << "\n";
	o << "Comments: \n";
	for (auto& comment : m_comments)
		o << comment;
}

std::ostream& operator<<(std::ostream& o, const Post& post)
{
	o << "Text Post: " << post.m_text << "\n";
	o << "Likes: " << post.m_likes << "\n";
	o << "Dislikes: " << post.m_dislikes << "\n";
	o << "Reaction: " << static_cast<int>(post.m_reaction) << "\n";
	o << "Parent Post: ";

	if (post.m_parentId.has_value())
		o << post.m_parentId.value() << "\n";
	else
		o << "0\n";

	o << "Date and time of creation: " << util::TimePointToString(post.m_dateAndTimeOfCreation) << "\n";

	o << "Comments: \n";
	for (auto& comment : post.m_comments)
		o << comment;

	return o;
}
