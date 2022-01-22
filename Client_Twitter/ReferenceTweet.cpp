#include "ReferenceTweet.h"
#include "Util.h"
ReferenceTweet::ReferenceTweet( QWidget* parent)
	:  QDialog(parent)
{
	ui.setupUi(this);
	ConnectMyButtons();
}

void ReferenceTweet::ConnectMyButtons()
{
	
	connect(ui.likeButton, SIGNAL(clicked()), this, SLOT(LikeButtonPressed()));
	connect(ui.dislikeButton, SIGNAL(clicked()), this, SLOT(DislikeButtonPressed()));
	//connect(ui.followButton, SIGNAL(clicked()), this, SLOT(FollowButtonPressed()));
	connect(ui.postCommentButton, SIGNAL(clicked()), this, SLOT(UploadComment()));
	ui.usernamePostLabel->setText(QString::fromStdString(m_referencedTweet.GetUsername()));
	ui.nameInitLabel->setText("A");
	
}

void ReferenceTweet::LikeButtonPressed()
{
	if (m_checkReaction) {
		m_checkReaction = false;

		ui.likeButton->setStyleSheet(" background-color: rgb(30, 101, 255); color: rgb(255, 255, 255); border-radius: 15px; ");
		ui.dislikeButton->setEnabled(true);
		m_referencedTweet.SetReaction(Post::Reaction::EDefault, -1, 0);

		UpdateLikesDislikes(ui.likedByLabel, ui.dislikedByLabel, m_referencedTweet);
		UpdateReaction(Post::Reaction::ELike, "-1", m_referencedTweet);

	}
	else {
		m_checkReaction = true;

		ui.likeButton->setStyleSheet(" background-color: rgb(1, 221, 255); color:rgb(255, 255, 255); border-radius: 15px; ");
		ui.dislikeButton->setEnabled(false);

		m_referencedTweet.SetReaction(Post::Reaction::ELike, 1, 0);

		UpdateLikesDislikes(ui.likedByLabel, ui.dislikedByLabel, m_referencedTweet);
		UpdateReaction(Post::Reaction::ELike, "+1", m_referencedTweet);




		//m_searchedPosts[m_postIndex].SetReaction(Post::Reaction::ELike, 1, 0);

		//m_profile->UpdateLikesDislikes(ui.likedByLabel, ui.dislikedByLabel, m_searchedPosts[m_postIndex]);

		//m_profile->UpdateReaction(Post::Reaction::ELike, "+1", m_searchedPosts[m_postIndex]);
	}
}


void ReferenceTweet::DislikeButtonPressed()
{
	if (m_checkReaction) {
		m_checkReaction = false;

		ui.dislikeButton->setStyleSheet("background-color:  rgb(30, 101, 255);color:rgb(255, 255, 255); border-radius: 15px;");
		ui.likeButton->setEnabled(true);

		m_referencedTweet.SetReaction(Post::Reaction::EDefault, 0, -1);

		UpdateLikesDislikes(ui.likedByLabel, ui.dislikedByLabel, m_referencedTweet);
		UpdateReaction(Post::Reaction::EDislike, "-1", m_referencedTweet);

		//m_userInView->Posts().at(m_indexPost).SetReaction(Post::Reaction::EDefault, 0, -1);
		//UpdateLikesDislikes(ui.likedByLabel, ui.dislikedByLabel, m_userInView->Posts().at(m_indexPost));

		//UpdateReaction(Post::Reaction::EDislike, "-1", m_userInView->Posts().at(m_indexPost));
	}
	else {
		m_checkReaction = true;

		ui.dislikeButton->setStyleSheet(" background-color:  rgb(1, 221, 255); color:rgb(255, 255, 255); border-radius: 15px;");
		ui.likeButton->setEnabled(false);

		m_referencedTweet.SetReaction(Post::Reaction::EDislike, 0, 1);

		UpdateLikesDislikes(ui.likedByLabel, ui.dislikedByLabel, m_referencedTweet);
		UpdateReaction(Post::Reaction::EDislike, "+1", m_referencedTweet);
	}

}


void ReferenceTweet::UploadComment()
{

	delete m_showComments;
	for (auto& lables : m_comments)
	{
		lables.data()->deleteLater();
	}
	m_comments.clear();
	m_showComments = new QVBoxLayout(this);

	std::string newTime = util::TimePointToString(m_referencedTweet.GetDateAndTimeOfCreation());

	for (const Comment& iterator : m_referencedTweet.GetComments()) {
		QPointer<QLabel> comment = new QLabel(this);
		comment->setText(QString::fromStdString(iterator.GetUsername() + " commented on " + util::TimePointToString(iterator.GetDateAndTimeOfCreation()).substr(0, newTime.size() - 8)
			+ " : \n" + " --> " + iterator.GetText()));
		m_showComments->addWidget(comment);
		m_comments.push_back(std::move(comment));
	}
	ui.commentContentScrollArea_2->setLayout(m_showComments);
}


void ReferenceTweet::FollowButtonPressed()
{
	/*if (m_followStatus == true)
	{
		ui.followButton->setText("Follow");
		App::GetUser()->Unfollow(m_userInView->GetId());
		m_followStatus = false;
	}
	else
	{
		ui.followButton->setText("Unfollow");
		App::GetUser()->Follow(m_userInView->GetId());
		m_followStatus = true;
	}*/
}

void ReferenceTweet::UpdateLikesDislikes(QLabel* likes, QLabel* dislikes, Post& post)
{
	likes->setText(QString::fromStdString("Liked by: " +
		std::to_string(post.GetLikes())));
	dislikes->setText(QString::fromStdString("Disliked by: " +
		std::to_string(post.GetDislikes())));
}

void ReferenceTweet::UpdateReaction(Post::Reaction reaction, const std::string& action, Post& post)
{
	json message;
	message["user_id"] = std::to_string(App::Get()->GetUser()->GetId());
	message["post_id"] = std::to_string(post.GetId());
	message["command"] = Socket::Command::REACT;

	message["reaction"] = std::to_string(static_cast<int>(reaction));
	message["action"] = action;

	//std::cout << message["reaction"];
	Socket::Get()->SendReceiveRequest(message);
}

void ReferenceTweet::UploadPost()
{
	//m_showComments.clear();
	/*for (auto& child : m_showComments.data()->children()) {
		delete child;
	 }*/
	
	ui.usernamePostLabel->setText(QString::fromStdString(m_referencedTweet.GetUsername()));
	ui.postTextBrowser->setText(QString::fromStdString(m_referencedTweet.GetText()));
	
	 //checking if the user did reacted already on post
	switch (m_referencedTweet.GetReaction())
	{

	case Post::Reaction::ELike:
	{
		//std::cout << m_personalProfile.ReturnPost(m_indexPost) << "\n";


		m_checkReaction = true;
		ui.likeButton->setEnabled(true);
		ui.dislikeButton->setEnabled(false);

		ui.likeButton->setStyleSheet("background-color:  rgb(1, 221, 255); color:rgb(255, 255, 255); border-radius: 15px; ");

		ui.dislikeButton->setStyleSheet(" background-color:  rgb(30, 101, 255); color:rgb(255, 255, 255); border-radius: 15px;");
		break;
	}
	case  Post::Reaction::EDislike:
	{
		//std::cout << m_personalProfile.ReturnPost(m_indexPost) << "\n";

		m_checkReaction = true;
		ui.likeButton->setEnabled(false);
		ui.dislikeButton->setEnabled(true);
		ui.likeButton->setStyleSheet("background-color:  rgb(30, 101, 255); color:rgb(255, 255, 255); border-radius: 15px; ");
     	ui.dislikeButton->setStyleSheet("background-color:  rgb(1, 221, 255);color:rgb(255, 255, 255); border-radius: 15px; ");
		break;
	}

	  // Post::Reaction::EDefault
	default: {
		//std::cout << m_personalProfile.ReturnPost(m_indexPost) << "\n";

		m_checkReaction = false;
		ui.likeButton->setEnabled(true);
		ui.dislikeButton->setEnabled(true);
		ui.likeButton->setStyleSheet("background-color:  rgb(30, 101, 255); color:rgb(255, 255, 255); border-radius: 15px; ");
		ui.dislikeButton->setStyleSheet(" background-color:  rgb(30, 101, 255); color:rgb(255, 255, 255); border-radius: 15px;");

		break;
	}
	}
	////displaying the text of post
	ui.postTextBrowser->setText(QString::fromStdString(m_referencedTweet.GetText()));


	//display date creation
	std::string time = util::TimePointToString(m_referencedTweet.GetDateAndTimeOfCreation());
	ui.postArea->setTitle(QString::fromStdString("posted on " + time.substr(0, time.size() - 8)));

	////display likes
	ui.likedByLabel->setText(QString::fromStdString("Liked by: " + std::to_string(m_referencedTweet.GetLikes())));
	ui.dislikedByLabel->setText(QString::fromStdString("Disliked by: " + std::to_string(m_referencedTweet.GetDislikes())));
	
	UploadComment();
	//PostMyComment();
}

void ReferenceTweet::SetReferencedTweet(const PostWithUser& referencedTweet)
{
	m_referencedTweet = referencedTweet;
	UploadPost();

}

ReferenceTweet::~ReferenceTweet()
{
}
