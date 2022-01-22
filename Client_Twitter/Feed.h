#pragma once

#include <QWidget>
#include "ui_Feed.h"
#include "Profile.h"	
#include <qvboxlayout>
#include "Socket.h"
#include <iostream>
#include "Post.h"
#include "PostWithUser.h"
#include "MakePost.h"
#include"ReferenceTweet.h"
class Feed : public QWidget
{

	Q_OBJECT

public:
	//using QVBoxLayoutPtr = std::shared_ptr<QVBoxLayout>;

	Feed(QWidget *parent = Q_NULLPTR);
	void DisplayPosts();
	void DefaultPost();
	void UploadComments();
	void UploadRetweet();
	void ConnectMyButtons();
	std::vector<std::pair<uint32_t, std::string>> SearchUsername(const std::string &username,uint32_t howMany);
	void SearchPost(const std::string& postTag);
	~Feed();
private slots:
	//make a comment
	void PostMyComment();
	//showing a user profile
	void ShowUserProfile();
    // show user profile
	void ShowProfile();
	//show searched posts
	void SearchButtonPressed();
	//show next post
	void ShowNextPost();
    //show previous post
	void ShowPreviousPost();
	//show my posts (works as refresh button)
	void UploadFeedPosts();
	//create a new post 
	void MakeNewPost();
	//action when the like button is pressed
	void LikeButtonPressed();
	//action when dislike button is pressed
	void DislikeButtonPressed();

	void SeeReferenceTweet();
	void Retweet();
	//Action when an item from search results it's clicked
	//void FoundUserItemClicked(QListWidgetItem* item);
	void CountChar(const QString& text);
signals:
	void textChanged(const QString& text);
	
private:
	void InsertPostsInContainer(const json& serverMg);

private:
	Ui::Feed ui;

	QPointer<QVBoxLayout> m_showComments;
	QList<QPointer<QLabel>> m_comments;
	bool m_checkReaction;
	
	MakePost m_makePost;
	Profile* m_profile;
	ReferenceTweet m_referenceTweet;
	std::vector<uint32_t> m_searchedUsersID;
	std::vector<PostWithUser> m_searchedPosts;
	std::unordered_map<uint32_t, PostWithUser> m_searchedParentPosts;
	uint16_t m_postIndex;

	bool searchUser;
};
