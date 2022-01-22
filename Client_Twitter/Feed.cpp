#include "Feed.h"

Feed::Feed(QWidget* parent) : QWidget(parent),
Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged)
searchUser(false)
{
	ui.setupUi(this);
	m_postIndex = 0;
	ConnectMyButtons();
	ui.foundUsers->hide();
	ui.showUserProfileButton->hide();
	m_profile = new Profile(App::Get()->GetUser());
	UploadFeedPosts();
}

void Feed::DefaultPost()
{
	ui.postTextBrowser->setText("No posts are available yet");
	ui.nameInitLabel->clear();
	ui.usernamePostLabel->clear();
	ui.dislikeButton->setText("");
	ui.likeButton->setText("");
	ui.likeButton->setStyleSheet(" background-color:  rgb(30, 101, 255); color:rgb(255, 255, 255); border-radius: 15px;");
	ui.dislikeButton->setStyleSheet(" background-color:  rgb(30, 101, 255); color:rgb(255, 255, 255); border-radius: 15px;");
	ui.referenceTweetText->hide();
	ui.referenceUsername->hide();
	ui.retweetButton->hide();
	ui.nextPostButton->setEnabled(false);
	ui.previousPostButton->setEnabled(false);
	ui.postCommentButton->setEnabled(false);
	ui.likeButton->setEnabled(false);
	ui.dislikeButton->setEnabled(false);
}
void Feed::UploadComments()
{
	delete m_showComments;
	for (auto& lables : m_comments)
	{
		lables.data()->deleteLater();
	}
	m_comments.clear();
	m_showComments = new QVBoxLayout(this);

	std::string newTime = util::TimePointToString(m_searchedPosts[m_postIndex].GetDateAndTimeOfCreation());

	for (const Comment& iterator : m_searchedPosts[m_postIndex].GetComments()) {
		QPointer<QLabel> comment = new QLabel(this);
		comment->setText(QString::fromStdString(iterator.GetUsername() + " commented on " + util::TimePointToString(iterator.GetDateAndTimeOfCreation()).substr(0, newTime.size() - 8)
			+ " : \n" + " --> " + iterator.GetText()));
		m_showComments->addWidget(comment);
		m_comments.push_back(std::move(comment));
	}
	ui.commentContentScrollArea->setLayout(m_showComments);
}

void Feed::ConnectMyButtons()
{
	connect(ui.postCommentButton, SIGNAL(clicked()), this, SLOT(PostMyComment()));
	connect(ui.searchButton, SIGNAL(clicked()), this, SLOT(SearchButtonPressed()));
	connect(ui.nextPostButton, SIGNAL(clicked()), this, SLOT(ShowNextPost()));
	connect(ui.previousPostButton, SIGNAL(clicked()), this, SLOT(ShowPreviousPost()));
	connect(ui.createPostButton, SIGNAL(clicked()), this, SLOT(MakeNewPost()));
	connect(ui.feedButton, SIGNAL(clicked()), this, SLOT(UploadFeedPosts()));
	connect(ui.likeButton, SIGNAL(clicked()), this, SLOT(LikeButtonPressed()));
	connect(ui.dislikeButton, SIGNAL(clicked()), this, SLOT(DislikeButtonPressed()));
	connect(ui.profileButton, SIGNAL(clicked()), this, SLOT(ShowProfile()));
	connect(ui.foundUsers, SIGNAL(itemClicked(QListWidgetItem*)),
		this, SLOT(FoundUsersItemClicked(QListWidgetItem*)));
	connect(ui.showUserProfileButton, SIGNAL(clicked()), this, SLOT(ShowUserProfile()));
	connect(ui.retweetButton, SIGNAL(clicked()), this, SLOT(Retweet()));
	connect(ui.referenceUsername, SIGNAL(clicked()), this, SLOT(SeeReferenceTweet()));
	connect(ui.commentLine, &QLineEdit::textChanged, this, &Feed::CountChar);
	connect(ui.commentLine, &QLineEdit::textChanged, this, &Feed::textChanged);
	ui.commentLine->setMaxLength(140);
	CountChar("");
}
std::vector<std::pair<uint32_t, std::string>> Feed::SearchUsername(const std::string& username, uint32_t howMany)
{
	json search;
	search["username"] = username;
	search["command"] = Socket::Command::USERNAME_SEARCH;
	search["how_many"] = howMany;

	json jUserMatches = Socket::Get()->SendReceiveRequest(search);
	std::vector<std::pair<uint32_t, std::string>> userMatches = jUserMatches;
	return userMatches;
}
void Feed::SearchPost(const std::string& postTag)
{
	json search;
	search["command"] = Socket::Command::POSTS_WITH_HASHTAG;
	search["user_id"] = App::Get()->GetUser()->GetId();
	std::string s = postTag;
	m_searchedPosts.clear();
	size_t found = s.find("#");

	auto notAlfanumeric = [](unsigned char ch) { return !isalnum(ch); };

	while (found != std::string::npos)
	{
		auto endHashtag = std::find_if(s.begin() + found + 1, s.end(),
			notAlfanumeric);

		search["strings"].push_back(std::string(s.begin() + found + 1, endHashtag));
		size_t startSearch = std::distance(s.begin(), endHashtag);
		found = s.find("#", startSearch);
	}

	json posts = Socket::Get()->SendReceiveRequest(search);
	if (posts.empty())
		return;

	m_postIndex = 0;
	InsertPostsInContainer(posts);
}
void Feed::SearchButtonPressed()
{
	m_searchedUsersID.clear();
	ui.foundUsers->clear();
	if (ui.searchLine->text().isEmpty())
		return;

	if (ui.searchLine->text().toStdString().find('#', 0) == std::string::npos)
	{
		ui.showUserProfileButton->show();
		auto users = SearchUsername(ui.searchLine->text().toStdString(), 5);
		if (users.empty())
			return;
		for (const auto& [userId, username] : users) {

			ui.foundUsers->addItem(QString::fromStdString(username));
			m_searchedUsersID.push_back(userId);

		}
		ui.foundUsers->show();
	}
	else {
		SearchPost(ui.searchLine->text().toStdString());
		ui.foundUsers->hide();
	}
}

void Feed::CountChar(const QString& text)
{
	QString text_label = QString("%1/%2").arg(text.size()).arg(ui.commentLine->maxLength());
	ui.char_counter->setText(text_label);
}
void Feed::ShowNextPost()
{
	if (m_postIndex < m_searchedPosts.size() - 1)
	{
		m_postIndex++;
		DisplayPosts();
	}
}

void Feed::ShowPreviousPost()
{
	if (m_postIndex > 0)
	{
		m_postIndex--;
		DisplayPosts();
	}
}

void Feed::UploadFeedPosts()
{
	json search;
	search["command"] = Socket::Command::FEED;
	search["user_id"] = App::Get()->GetUser()->GetId();
	json posts = Socket::Get()->SendReceiveRequest(search);

	m_postIndex = 0;
	ui.searchLine->setText("");

	InsertPostsInContainer(posts);
}

void Feed::ShowUserProfile()
{
	ui.foundUsers->hide();
	ui.showUserProfileButton->hide();
	ui.searchLine->clear();
	uint32_t currentIndexUser = ui.foundUsers->currentIndex().row();
	//std::cout << currentIndexUser << " \n";
	//if (currentIndexUser != -1)
		//std::cout << m_searchedUsersID[ui.foundUsers->currentIndex().row()] << " \n";

	if (m_searchedUsersID[ui.foundUsers->currentIndex().row()] != App::Get()->GetUser()->GetId())
	{
		json msg;
		msg["command"] = Socket::Command::GET_USER;
		msg["user_id"] = m_searchedUsersID[ui.foundUsers->currentIndex().row()];
		msg["connected_user_id"] = App::GetUser()->GetId();

		msg = Socket::Get()->SendReceiveRequest(msg);
		//std::cout << msg.dump(4);
		User* userInView = new User(msg["user_id"].get<int>(), msg["username"].get<std::string>(), msg["first_name"].get<std::string>(),
			msg["last_name"].get<std::string>(), util::StringToDateStruct(msg["birthday"].get<std::string>()),
			util::StringToTimePoint(msg["account_creation_time"].get<std::string>()));


		m_profile->SetFollowStatus(msg["follow_status"]);
		m_profile->SetUser(userInView);
	}
	else
		m_profile->SetUser(App::Get()->GetUser());
	searchUser = true;
	ShowProfile();
}

void Feed::ShowProfile()
{
	if (!searchUser)
		m_profile->SetUser(App::Get()->GetUser());
	else
		searchUser = false;

	m_profile->ResetPostIndex();
	m_profile->GetUser()->LoadPosts(App::Get()->GetUser()->GetId());
	m_profile->Init();
	m_profile->show();
}
void Feed::MakeNewPost()
{
	m_makePost.Reset();
	m_makePost.setModal(true);
	m_makePost.exec();
}
void Feed::LikeButtonPressed()
{
	if (m_checkReaction) {
		m_checkReaction = false;

		ui.likeButton->setStyleSheet(" background-color: rgb(30, 101, 255); color: rgb(255, 255, 255); border-radius: 15px; ");
		ui.dislikeButton->setEnabled(true);

		m_searchedPosts[m_postIndex].SetReaction(Post::Reaction::EDefault, -1, 0);

		m_profile->UpdateLikesDislikes(ui.likeButton, ui.dislikeButton, m_searchedPosts[m_postIndex]);
		m_profile->UpdateReaction(Post::Reaction::ELike, "-1", m_searchedPosts[m_postIndex]);

	}
	else {
		m_checkReaction = true;

		ui.likeButton->setStyleSheet(" background-color: rgb(1, 221, 255); color:rgb(255, 255, 255); border-radius: 15px; ");
		ui.dislikeButton->setEnabled(false);

		m_searchedPosts[m_postIndex].SetReaction(Post::Reaction::ELike, 1, 0);

		m_profile->UpdateLikesDislikes(ui.likeButton, ui.dislikeButton, m_searchedPosts[m_postIndex]);

		m_profile->UpdateReaction(Post::Reaction::ELike, "+1", m_searchedPosts[m_postIndex]);
	}

}
void Feed::DislikeButtonPressed()
{
	if (m_checkReaction) {
		m_checkReaction = false;

		ui.dislikeButton->setStyleSheet("background-color:  rgb(30, 101, 255);color:rgb(255, 255, 255); border-radius: 15px;");
		ui.likeButton->setEnabled(true);

		m_searchedPosts[m_postIndex].SetReaction(Post::Reaction::EDefault, 0, -1);

		m_profile->UpdateLikesDislikes(ui.likeButton, ui.dislikeButton, m_searchedPosts[m_postIndex]);

		m_profile->UpdateReaction(Post::Reaction::EDislike, "-1", m_searchedPosts[m_postIndex]);
	}
	else {
		m_checkReaction = true;

		ui.dislikeButton->setStyleSheet(" background-color:  rgb(1, 221, 255); color:rgb(255, 255, 255); border-radius: 15px;");
		ui.likeButton->setEnabled(false);

		m_searchedPosts[m_postIndex].SetReaction(Post::Reaction::EDislike, 0, 1);

		m_profile->UpdateLikesDislikes(ui.likeButton, ui.dislikeButton, m_searchedPosts[m_postIndex]);

		m_profile->UpdateReaction(Post::Reaction::EDislike, "+1", m_searchedPosts[m_postIndex]);
	}
}

void Feed::SeeReferenceTweet()
{   
	PostWithUser postWithUser = m_searchedParentPosts[m_searchedPosts.at(m_postIndex).GetParentId().value()];
	m_referenceTweet.SetReferencedTweet(postWithUser);
	
	m_referenceTweet.setModal(true);
	m_referenceTweet.exec();
}

void Feed::Retweet()
{
	m_makePost.setModal(true);
	m_makePost.SetParent(m_searchedPosts[m_postIndex].GetId());
	m_searchedParentPosts.insert({m_searchedPosts[m_postIndex].GetId() , m_searchedPosts[m_postIndex] });
	m_makePost.exec();
}

void Feed::InsertPostsInContainer(const json& postsJ)
{
	if (!m_searchedPosts.empty())
		m_searchedPosts.clear();

	if (!postsJ.empty())
	{
		std::vector<json> posts = postsJ["posts"];
		std::vector<json> parentPosts = postsJ["parent_posts"];

		for (const json& it : posts)
		{
			//std::cout << it.dump(4) << "\n";
			std::optional<int> parentId;

			if (it["parent_post"] == nullptr)
				parentId = std::nullopt;
			else
				parentId = std::stoi(static_cast<std::string>(it["parent_post"]));

			PostWithUser post(static_cast<std::string>(it["text"]), std::stoi(static_cast<std::string>(it["likes"])),
				std::stoi(static_cast<std::string>(it["dislikes"])), parentId, util::StringToTimePoint(it["post_time"]),
				static_cast<Post::Reaction>(std::stoi(static_cast<std::string>(it["reaction"]))),
				std::stoi(static_cast<std::string>(it["posted_by"])),
				static_cast<std::string>(it["username"]), std::stoi(static_cast<std::string>(it["post_id"])));

			if (it.find("comments") != it.end())
				for (auto& jComment : it["comments"])
				{
					Comment comment(static_cast<std::string>(jComment["text"]), std::stoi(static_cast<std::string>(jComment["user_id"])),
						util::StringToTimePoint(jComment["post_time"]), static_cast<std::string>(jComment["username"]),
						std::stoi(static_cast<std::string>(jComment["comment_id"])));

					post.AddComment(comment);
				}
			m_searchedPosts.push_back(post);
		}

		for (const json& it : parentPosts)
		{
			//std::cout << it.dump(4) << "\n";

			std::optional<int> parentId;

			if (it["parent_post"] == nullptr)
				parentId = std::nullopt;
			else
				parentId = std::stoi(static_cast<std::string>(it["parent_post"]));

			PostWithUser post(static_cast<std::string>(it["text"]), std::stoi(static_cast<std::string>(it["likes"])),
				std::stoi(static_cast<std::string>(it["dislikes"])), parentId, util::StringToTimePoint(it["post_time"]),
				static_cast<Post::Reaction>(std::stoi(static_cast<std::string>(it["reaction"]))),
				std::stoi(static_cast<std::string>(it["posted_by"])),
				static_cast<std::string>(it["username"]), std::stoi(static_cast<std::string>(it["post_id"])));

			if (it.find("comments") != it.end())
				for (auto& jComment : it["comments"])
				{
					Comment comment(static_cast<std::string>(jComment["text"]), std::stoi(static_cast<std::string>(jComment["user_id"])),
						util::StringToTimePoint(jComment["post_time"]), static_cast<std::string>(jComment["username"]),
						std::stoi(static_cast<std::string>(jComment["comment_id"])));

					post.AddComment(comment);
				}
			m_searchedParentPosts.insert({ post.GetId(),post });
		}
	}

	m_postIndex = 0;
	if (m_searchedPosts.size() != 0)
	{
		DisplayPosts();
		ui.postCommentButton->setEnabled(true);
	}
	else
		DefaultPost();
}
void Feed::UploadRetweet()
{
	ui.referenceTweetText->setText(QString::fromStdString(m_searchedParentPosts[m_searchedPosts[m_postIndex].GetParentId().value()].GetText()));
	ui.referenceUsername->setText(QString::fromStdString(m_searchedParentPosts[m_searchedPosts[m_postIndex].GetParentId().value()].GetUsername()));
}
void Feed::DisplayPosts()
{
	ui.referenceUsername->setStyleSheet("Text-align:top,left; border: 2px solid rgb(85, 170, 255);");
	ui.postTextBrowser->setFontPointSize(12);
	ui.referenceTweetText->setFontPointSize(10);

	if (m_postIndex > m_searchedPosts.size())
		ui.nextPostButton->setEnabled(false);
	else
		ui.nextPostButton->setEnabled(true);

	if (m_postIndex < m_searchedPosts.size())
		ui.previousPostButton->setEnabled(true);
	else
		ui.previousPostButton->setEnabled(false);

	switch (m_searchedPosts[m_postIndex].GetReaction())
	{

	case Post::Reaction::ELike:
	{
		m_checkReaction = true;
		ui.likeButton->setEnabled(true);
		ui.dislikeButton->setEnabled(false);

		ui.likeButton->setStyleSheet("background-color:  rgb(1, 221, 255); color:rgb(255, 255, 255); border-radius: 15px; ");

		ui.dislikeButton->setStyleSheet(" background-color:  rgb(30, 101, 255); color:rgb(255, 255, 255); border-radius: 15px;");
		break;
	}
	case  Post::Reaction::EDislike:
	{
		m_checkReaction = true;
		ui.likeButton->setEnabled(false);
		ui.dislikeButton->setEnabled(true);

		ui.likeButton->setStyleSheet("background-color:  rgb(30, 101, 255); color:rgb(255, 255, 255); border-radius: 15px; ");
		ui.dislikeButton->setStyleSheet("background-color:  rgb(1, 221, 255);color:rgb(255, 255, 255); border-radius: 15px; ");
		break;
	}
	default: {

		m_checkReaction = false;
		ui.likeButton->setEnabled(true);
		ui.dislikeButton->setEnabled(true);
		ui.likeButton->setStyleSheet("background-color:  rgb(30, 101, 255); color:rgb(255, 255, 255); border-radius: 15px; ");
		ui.dislikeButton->setStyleSheet(" background-color:  rgb(30, 101, 255); color:rgb(255, 255, 255); border-radius: 15px;");

		break;
	}
	}

	if (m_searchedPosts[m_postIndex].GetParentId().has_value())
	{
		ui.retweetButton->hide();
		UploadRetweet();
		ui.referenceUsername->show();
		ui.referenceTweetText->show();
	}
	else {
		ui.retweetButton->show();
		ui.referenceUsername->hide();
		ui.referenceTweetText->hide();
		ui.referenceTweetText->clear();
		ui.referenceUsername->setText("");
	}
	ui.postTextBrowser->setText(QString::fromStdString(m_searchedPosts[m_postIndex].GetText()));
	ui.usernamePostLabel->setText(QString::fromStdString(m_searchedPosts[m_postIndex].GetUsername()));
	ui.nameInitLabel->setText(QString(m_searchedPosts[m_postIndex].GetUsername()[0]).toUpper());

	ui.postTextBrowser->setText(QString::fromStdString(m_searchedPosts[m_postIndex].GetText()));

	std::string time = util::TimePointToString(m_searchedPosts[m_postIndex].GetDateAndTimeOfCreation());
	ui.postArea->setTitle(QString::fromStdString("posted on " + time.substr(0, time.size() - 8)));

	//display likes
	/*ui.likedByLabel->setText(QString::fromStdString("Liked by: " + std::to_string(m_searchedPosts[m_postIndex].GetLikes())));
	ui.dislikedByLabel->setText(QString::fromStdString("Disliked by: " + std::to_string(m_searchedPosts[m_postIndex].GetDislikes())));*/
	ui.likeButton->setText("  " + QString::fromStdString(std::to_string(m_searchedPosts[m_postIndex].GetLikes())));
	ui.dislikeButton->setText("  " + QString::fromStdString(std::to_string(m_searchedPosts[m_postIndex].GetDislikes())));

	//display comments
	UploadComments();
}

void Feed::PostMyComment() {

	if (ui.commentLine->text().isEmpty())
		return;

	std::string time = util::TimePointToString(std::chrono::system_clock::now(), "%Y-%m-%d%H:%M:%S");
	json com, msg;

	com["text"] = ui.commentLine->text().toStdString();
	com["post_time"] = time.substr(0, time.size() - 8);
	com["posted_on"] = std::to_string(m_searchedPosts[m_postIndex].GetId());
	com["posted_by"] = std::to_string(App::Get()->GetUser()->GetId());

	msg["command"] = Socket::Command::COMMENT;
	msg["reaction"] = std::to_string(static_cast<int>(m_searchedPosts[m_postIndex].GetReaction()));
	msg["comment"].push_back(com);

	json it2 = Socket::Get()->SendReceiveRequest(msg);

	Comment newComment = Comment(static_cast<std::string>(it2["text"]), std::stoi(static_cast<std::string>(it2["user_id"])),
		util::StringToTimePoint(it2["post_time"]), static_cast<std::string>(it2["username"]),
		std::stoi(static_cast<std::string>(it2["comment_id"])));

	m_searchedPosts[m_postIndex].AddCommentFirst(newComment);

	UploadComments();
	ui.commentLine->clear();
}

Feed::~Feed()
{

}
