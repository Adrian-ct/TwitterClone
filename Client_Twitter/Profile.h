#pragma once

// #include <QWidget>
#include "ui_Profile.h"
#include "App.h"
#include <qvboxlayout>
#include <QPointer>
#include<qdialog.h>
#include "ReferenceTweet.h"
class Profile : public QDialog
{
	Q_OBJECT

public:

	Profile(User* user, QWidget* parent = Q_NULLPTR);

	Profile(Profile&& profile);
	Profile();
	Profile(const Profile& profile);
	Profile& operator=(Profile&& profile);

	~Profile();
	//uploading default values for post or the first post
	void Init();
	//making connections of type SIGNAL/SLOT
	void ConnectMyButtons();
	//uploading post from data base
	void UploadPost();
	void PostMyComment();
	//uploads a default Post for when the user doesnt have posts
	void DefaultPost();
	void UpdateLikesDislikes(QPushButton* likes, QPushButton* dislikes, Post& post);
	void UpdateReaction(Post::Reaction reaction, const std::string& action, Post& post);
	void ResetPostIndex();
	void SetUser(User* user);
	void SetFollowStatus(bool followStatus);
	void UploadReferencedTweet();
	
	User* GetUser()const;
private slots:

	//shows the previous post if it exists
	void ShowPreviousPost();
	//show the next post if it exists
	void ShowNextPost();
	//action when the like button is pressed
	void LikeButtonPressed();
	//action when dislike button is pressed
	void DislikeButtonPressed();
	void GoToFeedButton();
	void FollowButtonPressed();
	void SeeReferencedTweet();
	void UploadComment();
	void CountChar(const QString& text);
signals:
	void textChanged(const QString& text);

	

private:
	Ui::Profile ui;

	//true if there is any reaction,false otherwise;

	QPointer<QVBoxLayout> m_showComments;
	QVector<QPointer<QLabel>> m_comments;
	ReferenceTweet m_referenceTweet;
	bool m_checkReaction;
	int16_t m_indexPost;
	User* m_userInView;
	bool m_followStatus;
};
