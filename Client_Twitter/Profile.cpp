#include "Profile.h"
#include "Socket.h"

Profile::Profile(User* user, QWidget* parent)
	: QDialog(parent),
	m_userInView(user),
	m_indexPost(0)
{
	ui.setupUi(this);
	ConnectMyButtons();

}
Profile::Profile() :
	m_userInView(nullptr),
	m_indexPost(0),
	m_checkReaction(false)
{
}
Profile::Profile(const Profile& profile)
{
	m_checkReaction = profile.m_checkReaction;
	m_indexPost = profile.m_indexPost;
	m_showComments = profile.m_showComments;

	if (m_userInView != nullptr && m_userInView != App::Get()->GetUser())
		delete m_userInView;

	m_userInView = profile.m_userInView;
	std::copy(profile.m_comments.begin(), profile.m_comments.end(), m_comments.begin());
}
void Profile::ConnectMyButtons()
{
	connect(ui.nextPostButton, SIGNAL(clicked()), this, SLOT(ShowNextPost()));
	connect(ui.previousPostButton, SIGNAL(clicked()), this, SLOT(ShowPreviousPost()));
	connect(ui.likeButton, SIGNAL(clicked()), this, SLOT(LikeButtonPressed()));
	connect(ui.dislikeButton, SIGNAL(clicked()), this, SLOT(DislikeButtonPressed()));
	connect(ui.followButton, SIGNAL(clicked()), this, SLOT(FollowButtonPressed()));
	connect(ui.postCommentButton, SIGNAL(clicked()), this, SLOT(UploadComment()));
	connect(ui.feedButton, SIGNAL(clicked()), this, SLOT(GoToFeedButton()));
	connect(ui.referenceUsername, SIGNAL(clicked()), this, SLOT(SeeReferencedTweet()));
	connect(ui.commentLine, &QLineEdit::textChanged, this, &Profile::CountChar);
	connect(ui.commentLine, &QLineEdit::textChanged, this, &Profile::textChanged);
	ui.commentLine->setMaxLength(140);
	CountChar("");
}

void Profile::Init()
{
	ui.previousPostButton->setEnabled(true);
	ui.nextPostButton->setEnabled(true);

	if (m_followStatus == true)
	{
		ui.followButton->setText("Unfollow");
	}
	else
	{
		ui.followButton->setText("Follow");
	}

	
	// reference area styleSheet
	ui.referenceUsername->setStyleSheet("Text-align:top,left; border: 2px solid rgb(85, 170, 255); ");
	ui.referenceTweetText->setFontPointSize(10);
	
	
	//ui.referenceUsername.hide();
	//ui.referenceTweetText->hide();
	
	ui.postTextBrowser->setFontPointSize(12);
	ui.followButton->hide();
	
	if (m_userInView != App::Get()->GetUser())
		ui.followButton->show();

	ui.usernamePostLabel->setText(QString::fromStdString(m_userInView->GetUsername()));
	ui.nameInitLabel->setText(QString::fromStdString(m_userInView->GetUsername()).at(0).toUpper());
	m_indexPost = 0;
	if (m_userInView->Posts().empty())
	{
		DefaultPost();
	}
	else {
		//uploading the Post in profile
		UploadPost();
		ui.postCommentButton->setEnabled(true);
	}
}

void Profile::UploadPost()
{
	//m_showComments.clear();
	/*for (auto& child : m_showComments.data()->children()) {
		delete child;
	 }*/


	 //checking if the user did reacted already on post
	switch (m_userInView->Posts().at(m_indexPost).GetReaction())
	{

	case Post::Reaction::ELike:
	{
		/*if (m_userInView->ParentPosts().find(m_userInView->Posts().at(m_indexPost).GetId()) != m_userInView->ParentPosts().end())
			m_userInView->ParentPosts().at(m_userInView->Posts().at(m_indexPost).GetId()).SetReaction(Post::Reaction::ELike);*/
		
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
		//if (m_userInView->ParentPosts().find(m_userInView->Posts().at(m_indexPost).GetId()) != m_userInView->ParentPosts().end())
		//	m_userInView->ParentPosts().at(m_userInView->Posts().at(m_indexPost).GetId()).SetReaction(Post::Reaction::EDislike);
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
		/*if (m_userInView->ParentPosts().find(m_userInView->Posts().at(m_indexPost).GetId()) != m_userInView->ParentPosts().end())
			m_userInView->ParentPosts().at(m_userInView->Posts().at(m_indexPost).GetId()).SetReaction(Post::Reaction::EDefault);*/
		m_checkReaction = false;
		ui.likeButton->setEnabled(true);
		ui.dislikeButton->setEnabled(true);
		ui.likeButton->setStyleSheet("background-color:  rgb(30, 101, 255); color:rgb(255, 255, 255); border-radius: 15px; ");
		ui.dislikeButton->setStyleSheet(" background-color:  rgb(30, 101, 255); color:rgb(255, 255, 255); border-radius: 15px;");

		break;
	}
	}
	//displaying the text of post
	
	ui.postTextBrowser->setText(QString::fromStdString(m_userInView->Posts().at(m_indexPost).GetText()));
	
	//upload with the reference tweet with user username
	//if its a referenced tweet then we upload the retweet area 
	if (m_userInView->Posts()[m_indexPost].GetParentId().has_value())
	{   
		
		ui.referenceUsername->show();
		ui.referenceTweetText->show();
		UploadReferencedTweet();
	}
	else {
		ui.referenceUsername->hide();
		ui.referenceTweetText->hide();
		ui.referenceTweetText->clear();
		ui.referenceUsername->setText("");
	}
	/*ui.referenceUsername->setText("the retweeted tweet username and when was posted"); 
	ui.referenceTweetText->setText("0123456789101112131415161718192021222324252627282930313233343536373839404142434445464748495051525354555657585960616263646566676869707172737475767778798081828384858687888990919293949596979899100101102103104105106107108109110111112113114115116117118119120121122123124125126127128129130131132133134135136137138139");
	*/
	//display date creation
	std::string time = util::TimePointToString(m_userInView->Posts().at(m_indexPost).GetDateAndTimeOfCreation());
	ui.postArea->setTitle(QString::fromStdString("posted on " + time.substr(0, time.size() - 8)));

	//display likes
	ui.likeButton->setText(QString::fromStdString(" " + std::to_string(m_userInView->Posts().at(m_indexPost).GetLikes())));
	ui.dislikeButton->setText(QString::fromStdString(" " + std::to_string(m_userInView->Posts().at(m_indexPost).GetDislikes())));

	PostMyComment();

}
void Profile::UploadComment()
{
	std::string time = util::TimePointToString(std::chrono::system_clock::now(), "%Y-%m-%d%H:%M:%S");
	json com, msg;

	com["text"] = ui.commentLine->text().toStdString();
	com["post_time"] = time.substr(0, time.size() - 8);
	com["posted_on"] = std::to_string(m_userInView->Posts().at(m_indexPost).GetId());
	com["posted_by"] = std::to_string(App::Get()->GetUser()->GetId());

	msg["command"] = Socket::Command::COMMENT;
	msg["reaction"] = std::to_string(static_cast<int>(m_userInView->Posts().at(m_indexPost).GetReaction()));
	msg["comment"].push_back(com);

	json it2 = Socket::Get()->SendReceiveRequest(msg);
	//std::cout << it2.dump(4) << "\n";

	Comment newComment = Comment(static_cast<std::string>(it2["text"]), std::stoi(static_cast<std::string>(it2["user_id"])),
		util::StringToTimePoint(it2["post_time"]), static_cast<std::string>(it2["username"]),
		std::stoi(static_cast<std::string>(it2["comment_id"])));

	m_userInView->Posts().at(m_indexPost).AddComment(newComment);
	PostMyComment();

	ui.commentLine->clear();
}

void Profile::CountChar(const QString& text)
{
	QString text_label = QString("%1/%2").arg(text.size()).arg(ui.commentLine->maxLength());
	ui.char_counter->setText(text_label);
}
void Profile::ResetPostIndex()
{
	m_indexPost = 0;
}
void Profile::SetUser(User* user)
{
	if (m_userInView != nullptr && m_userInView != App::Get()->GetUser())
		delete m_userInView;
	m_userInView = user;
}
void Profile::SetFollowStatus(bool followStatus)
{
	m_followStatus = followStatus;
}


void Profile::UploadReferencedTweet()
{
	//ui.referenceTweetText
	ui.referenceTweetText->setText(QString::fromStdString(m_userInView->ParentPosts()[ m_userInView->Posts().at(m_indexPost).GetParentId().value()].GetText()));
	ui.referenceUsername->setText(QString::fromStdString(m_userInView->ParentPosts()[m_userInView->Posts().at(m_indexPost).GetParentId().value()].GetUsername()));
}

void Profile::SeeReferencedTweet()
{
	PostWithUser postWithUser = m_userInView->ParentPosts()[m_userInView->Posts().at(m_indexPost).GetParentId().value()];
	m_referenceTweet.SetReferencedTweet(postWithUser);
	//m_referenceTweet.UploadPost();
	m_referenceTweet.setModal(true);
	m_referenceTweet.exec();
}

void Profile::PostMyComment()
{
	delete m_showComments;
	for (auto& lables : m_comments)
	{
		lables.data()->deleteLater();
	}
	m_comments.clear();
	m_showComments = new QVBoxLayout(this);


	std::string newTime = util::TimePointToString(m_userInView->Posts().at(m_indexPost).GetDateAndTimeOfCreation());

	for (const Comment& iterator : m_userInView->Posts().at(m_indexPost).GetComments()) {
		QPointer<QLabel> comment = new QLabel(this);
		comment->setText(QString::fromStdString(iterator.GetUsername() + " commented on " + util::TimePointToString(iterator.GetDateAndTimeOfCreation()).substr(0, newTime.size() - 8)
			+ " : \n" + " --> " + iterator.GetText()));
		m_showComments->addWidget(comment);
		m_comments.push_back(std::move(comment));
	}
	ui.commentContentScrollArea->setLayout(m_showComments);
}
void Profile::DefaultPost()
{
	delete m_showComments;
	for (auto& lables : m_comments)
	{
		lables.data()->deleteLater();
	}
	m_comments.clear();
	m_showComments = new QVBoxLayout(this);


	ui.referenceTweetText->hide();
	ui.referenceUsername->hide();
	ui.postCommentButton->setEnabled(false);
	ui.likeButton->setEnabled(false);
	ui.dislikeButton->setEnabled(false);
	
	ui.likeButton->setStyleSheet(" background-color:  rgb(30, 101, 255); color:rgb(255, 255, 255); border-radius: 15px;");
	ui.dislikeButton->setStyleSheet(" background-color:  rgb(30, 101, 255); color:rgb(255, 255, 255); border-radius: 15px;");
	
	ui.nextPostButton->setEnabled(false);
	ui.previousPostButton->setEnabled(false);

	ui.postTextBrowser->setText("You havent posted anything yet ^-^");

	/*ui.likedByLabel->setText("Liked by: 0 peoples");
	ui.dislikedByLabel->setText("Disliked by: 0 peoples");*/
}
void Profile::UpdateLikesDislikes(QPushButton* likes, QPushButton* dislikes, Post& post)
{
	likes->setText(QString::fromStdString("  "+ std::to_string(post.GetLikes())));
	dislikes->setText(QString::fromStdString("  " + std::to_string(post.GetDislikes())));
}
void Profile::UpdateReaction(Post::Reaction reaction, const std::string& action, Post& post)
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
User* Profile::GetUser()const
{
	return m_userInView;
}
void Profile::ShowNextPost() {

	if (m_indexPost < m_userInView->Posts().size() - 1) {
		m_indexPost++;
		ui.previousPostButton->setEnabled(true);
		UploadPost();
	}
	else
		ui.nextPostButton->setEnabled(false);
}
void Profile::ShowPreviousPost() {

	if (m_indexPost > 0) {
		m_indexPost--;
		ui.nextPostButton->setEnabled(true);
		UploadPost();
	}
	else
	{
		ui.previousPostButton->setEnabled(true);
	}
}
void Profile::LikeButtonPressed()
{


	if (m_checkReaction) {
		//already clicked
		m_checkReaction = false;

		ui.likeButton->setStyleSheet(" background-color: rgb(30, 101, 255); color: rgb(255, 255, 255); border-radius: 15px; ");
		ui.dislikeButton->setEnabled(true);

		//m_userInView->GetPosts().at(m_indexPost)
		m_userInView->Posts().at(m_indexPost).SetReaction(Post::Reaction::EDefault, -1, 0);
		UpdateLikesDislikes(ui.likeButton, ui.dislikeButton, m_userInView->Posts().at(m_indexPost));

		UpdateReaction(Post::Reaction::ELike, "-1", m_userInView->Posts().at(m_indexPost));
		//std::cout << message["reaction"].at(0) << "\n" << message["reaction"].at(1) << "\n";
	}
	else {
		m_checkReaction = true;

		ui.likeButton->setStyleSheet(" background-color: rgb(1, 221, 255); color:rgb(255, 255, 255); border-radius: 15px; ");
		ui.dislikeButton->setEnabled(false);

		m_userInView->Posts().at(m_indexPost).SetReaction(Post::Reaction::ELike, 1, 0);
		UpdateLikesDislikes(ui.likeButton, ui.dislikeButton, m_userInView->Posts().at(m_indexPost));

		UpdateReaction(Post::Reaction::ELike, "+1", m_userInView->Posts().at(m_indexPost));
	}
}
void Profile::DislikeButtonPressed()
{
	if (m_checkReaction) {
		m_checkReaction = false;

		ui.dislikeButton->setStyleSheet("background-color:  rgb(30, 101, 255);color:rgb(255, 255, 255); border-radius: 15px;");
		ui.likeButton->setEnabled(true);

		m_userInView->Posts().at(m_indexPost).SetReaction(Post::Reaction::EDefault, 0, -1);
		UpdateLikesDislikes(ui.likeButton, ui.dislikeButton, m_userInView->Posts().at(m_indexPost));

		UpdateReaction(Post::Reaction::EDislike, "-1", m_userInView->Posts().at(m_indexPost));
	}
	else {
		m_checkReaction = true;

		ui.dislikeButton->setStyleSheet(" background-color:  rgb(1, 221, 255); color:rgb(255, 255, 255); border-radius: 15px;");
		ui.likeButton->setEnabled(false);

		m_userInView->Posts().at(m_indexPost).SetReaction(Post::Reaction::EDislike, 0, 1);
		UpdateLikesDislikes(ui.likeButton, ui.dislikeButton, m_userInView->Posts().at(m_indexPost));

		UpdateReaction(Post::Reaction::EDislike, "+1", m_userInView->Posts().at(m_indexPost));
	}
}

void Profile::GoToFeedButton()
{
	this->close();
}

void Profile::FollowButtonPressed()
{
	if (m_followStatus == true)
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
	}
}

Profile::Profile(Profile&& profile) :
	m_checkReaction(profile.m_checkReaction),
	m_indexPost(profile.m_indexPost),
	m_showComments(profile.m_showComments)
{
	delete m_userInView;
	m_userInView = profile.m_userInView;
	std::swap(m_comments, profile.m_comments);
	profile.m_comments.clear();
}

Profile& Profile::operator=(Profile&& profile)
{
	if (this != &profile)
	{
		m_checkReaction = profile.m_checkReaction;
		m_indexPost = profile.m_indexPost;
		m_showComments = profile.m_showComments;

		if (m_userInView != nullptr && m_userInView != App::Get()->GetUser())
			delete m_userInView;

		m_userInView = profile.m_userInView;
		std::swap(m_comments, profile.m_comments);
		profile.m_comments.clear();
	}
	return *this;
}

Profile::~Profile()
{
}