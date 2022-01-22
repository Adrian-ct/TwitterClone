#include "PostWithUser.h"

PostWithUser::PostWithUser(const std::string& text, uint32_t likes, uint32_t dislikes, std::optional<uint32_t> parentId, 
    std::chrono::system_clock::time_point m_dateAndTimeOfCreation, Post::Reaction reaction,
    uint16_t idUser, const std::string& username, uint32_t post_id):
    Post(text, likes, dislikes, parentId, m_dateAndTimeOfCreation, reaction, post_id),
    m_idUser(idUser), m_username(username)
{}

PostWithUser::PostWithUser(const PostWithUser& post):
    Post(post)
{
    m_idUser = post.m_idUser;
    m_username = post.m_username;
}

void PostWithUser::Print(std::ostream& o)const
{
    Post::Print(o);
    o << "Id User: " << m_idUser << " " << "\nUsername: " << m_username << "\n";
}

std::ostream& operator<<(std::ostream& o,const PostWithUser& post)
{
    post.Print(o);
    return o;
}

uint16_t PostWithUser::GetIdUser() const
{
    return m_idUser;
}

const std::string& PostWithUser::GetUsername()
{
    return m_username;
}
