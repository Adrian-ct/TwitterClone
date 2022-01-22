#pragma once
#include <string>
#include <chrono>
#include "IEntityC.h"
#include "Post.h"
#include "PostWithUser.h"

class User :
	public IEntityC
{
public:
	User() = default;
	User(uint32_t id, const std::string& username, const std::string& firstName, const std::string& lastName,
		const std::tm& birthday, const std::chrono::system_clock::time_point& creationTime);
	//User(User&& other) noexcept;
	//User& operator=(User&& other) noexcept;

	const std::string& GetUsername() const;
	const std::string& GetFirstName() const;
	const std::string& GetLastName() const;
	const std::tm& GetBirthday();
	const std::chrono::system_clock::time_point& GetCreationTime();
	uint32_t GetId() const;

	void SetUsername(const std::string& username);
	void SetFirstName(const std::string& firstName);
	void SetLastName(const std::string& lastName);
	void SetBirthday(const std::tm& birthday);

	void MakePost(const std::string& text, const  std::optional<uint32_t>& parentPost = std::nullopt) const;
	void Follow(uint32_t userId);
	void Unfollow(uint32_t userId);

	//const PostsContainer& GetPosts() const;
	std::vector<Post>& Posts();
	std::unordered_map<uint32_t, PostWithUser>& ParentPosts();


	void LoadPosts(uint32_t loggedUser);

	json MembersToJson()const override;
private:
	uint32_t m_id;
	std::string m_username;
	std::string m_firstName;
	std::string m_lastName;
	std::tm m_birthday;
	std::chrono::system_clock::time_point m_creationTime;
	//PostsContainer m_postsContainer;
	std::vector<Post> m_posts;
	std::unordered_map<uint32_t, PostWithUser> m_parentPosts;

};

