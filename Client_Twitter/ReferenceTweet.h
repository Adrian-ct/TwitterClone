#pragma once

#include "qdialog.h"
#include "ui_ReferenceTweet.h"
#include "PostWithUser.h"
#include <QPointer>
#include <qvboxlayout>
#include "Socket.h"
#include "App.h"

class ReferenceTweet : public QDialog
{
	Q_OBJECT

public:
	ReferenceTweet(QWidget* parent = Q_NULLPTR);
	~ReferenceTweet();

	//void UpdateReaction(Post::Reaction reaction, const std::string& action, Post& post);
	//void UpdateLikesDislikes(QLabel* likes, QLabel* dislikes, Post& post);
	void ConnectMyButtons();
	void UploadPost();

	void SetReferencedTweet(const PostWithUser& referencedTweet);
private slots:

	void LikeButtonPressed();
	//action when dislike button is pressed
	void DislikeButtonPressed();
	//take it from Profile.h
	// void PostMyComment();
	void FollowButtonPressed();
	void UpdateLikesDislikes(QLabel* likes, QLabel* dislikes, Post& post);
	void UpdateReaction(Post::Reaction reaction, const std::string& action, Post& post);
	void UploadComment();
private:
	Ui::ReferenceTweet ui;
	
	PostWithUser m_referencedTweet;
	
	//std::string m_username;

	QPointer<QVBoxLayout> m_showComments;
	QVector<QPointer<QLabel>> m_comments;


	bool m_checkReaction;
	bool m_followStatus;
};
