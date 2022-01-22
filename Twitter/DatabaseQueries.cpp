#include "DatabaseQueries.h"


void dbq::InsertUser(MyDB& db, const json& user)
{
	std::string statement;
	statement += "into User values(null,'";
	statement += user["username"];
	statement += "','";
	statement += user["password"];
	statement += "','";
	statement += user["first_name"];
	statement += "','";
	statement += user["last_name"];
	statement += "','";
	statement += user["birthday"];
	statement += "',CURRENT_TIMESTAMP)";

	db.Insert(statement);
}

json dbq::FetchUser(MyDB& db, const uint32_t id)
{
	std::vector<json> records = db.Select
	("user_id,username,first_name,last_name,birthday,account_creation_time FROM User WHERE user_id=" + std::to_string(id));
	json user;


	if (!records.empty())
	{
		user = records[0];
		user["user_id"] = std::stoi(static_cast<std::string>(user["user_id"]));
		user["success"] = true;
	}
	else
	{
		user["success"] = false;
	}


	return user;
}

json dbq::FetchUser(MyDB& db, const std::string& username)
{
	std::vector<json> records = db.Select
	("user_id,password,username,first_name,last_name,birthday,account_creation_time FROM User WHERE username='" + username + "'");
	json user;

	if (!records.empty())
	{
		user = records[0];
		user["user_id"] = std::stoi(static_cast<std::string>(user["user_id"]));
		user["success"] = 1;
	}
	else
	{
		user["success"] = 0;
	}

	return user;
}

std::vector<json> dbq::FetchAllUserIdsAndUsernames(const MyDB& db)
{
	std::vector<json> users = db.Select("user_id,username FROM User");

	for (json& user : users)
	{
		user["user_id"] = std::stoi(static_cast<std::string>(user["user_id"]));
	}

	return users;
}

json dbq::LogIn(MyDB& db, const std::string& username, const std::string& password)
{
	json user = FetchUser(db, username);

	if (user["success"] == 1)
	{
		if (user["password"] == password)
		{
			user.erase("password");
		}
		else
		{
			user.clear();
			user["succes"] = 0;
		}
	}

	return user;
}

json dbq::FetchAllPosts(MyDB& db)
{
	std::vector<json> postsAsVector = db.Select("post_id,text,posted_by,parent_post,post_time, user.username FROM Post INNER JOIN User on Post.posted_by = User.user_id");

	json posts;

	for (json& post : postsAsVector)
	{
		std::vector<json> likes, dislikes, comments;

		likes = db.Select("count(reaction_type) from PostReaction where reaction_type = 1 and reacted_on = " + static_cast<std::string>(post["post_id"]));
		dislikes = db.Select("count(reaction_type) from PostReaction where reaction_type = 2 and reacted_on = " + static_cast<std::string>(post["post_id"]));
		comments = db.Select("comment.comment_id, comment.text, comment.post_time, user.username, user.user_id from Comment INNER JOIN User on user.user_id = commented_by WHERE commented_on = " + static_cast<std::string>(post["post_id"]));

		for (auto& it : comments)
		{
			post["comments"].push_back(it);
		}

		post["likes"] = likes[0]["count(reaction_type)"];
		post["dislikes"] = dislikes[0]["count(reaction_type)"];

		posts.push_back(post);
	}

	return posts;
}

json dbq::FetchOnePost(MyDB& db, std::string idPost)
{
	std::vector<json> postsAsVector =
		db.Select("post_id,text,posted_by,parent_post,post_time,\
				user.username FROM Post INNER JOIN User on Post.posted_by = User.user_id where post_id = " +
			idPost);

	std::vector<json> likes, dislikes, comments;
	json post = postsAsVector[0];
	likes = db.Select("count(reaction_type) from PostReaction where reaction_type = 1 and reacted_on = " + static_cast<std::string>(post["post_id"]));
	dislikes = db.Select("count(reaction_type) from PostReaction where reaction_type = 2 and reacted_on = " + static_cast<std::string>(post["post_id"]));
	comments = db.Select("comment.comment_id, comment.text, comment.post_time, user.username, user.user_id from Comment INNER JOIN User on user.user_id = commented_by WHERE commented_on = " +
		static_cast<std::string>(post["post_id"]));

	for (auto& it : comments)
	{
		post["comments"].push_back(it);
	}

	post["likes"] = likes[0]["count(reaction_type)"];
	post["dislikes"] = dislikes[0]["count(reaction_type)"];

	return post;
}

void dbq::MakePost(MyDB& db, json post)
{
	std::string statement;
	statement += "into Post values(null,'";
	statement += post["text"];
	statement += "',";
	statement += std::to_string(static_cast<uint32_t>(post["posted_by"]));
	statement += ",";

	if (post["parent_post"] == nullptr)
	{
		statement += "null";
	}
	else
	{
		statement += std::to_string(static_cast<uint32_t> (post["parent_post"]));
	}

	statement += ",CURRENT_TIMESTAMP)";

	db.Insert(statement);
}

void dbq::Follow(MyDB& db, uint32_t followerId, uint32_t followedId)
{
	db.Insert("INTO Follow values(" + std::to_string(followerId) + "," + std::to_string(followedId) + ")");
}
void dbq::Unfollow(MyDB& db, uint32_t followerId, uint32_t followedId)
{
	db.Delete("FROM Follow WHERE follower='" + std::to_string(followerId) + "'and followed='" + std::to_string(followedId) + "'");
}

bool dbq::IsFollowing(MyDB& db, uint32_t followerId, uint32_t followedId)
{
	auto records =  db.Select("FROM Follow WHERE follower='" + std::to_string(followerId) + 
		"'and followed='" + std::to_string(followedId) + "'");

	if (records.empty())
	{
		return false;
	}

	return true;
}

bool dbq::ExistsUser(const MyDB& database, const std::string& username)
{
	std::string statement = "* FROM User WHERE username='" + username + "'";
	std::vector<json> check = database.Select(statement);

	return !check.empty();
}

bool dbq::VerifyPassword(const MyDB& database, const std::string& username, const std::string& password)
{
	std::string statement = "password FROM User WHERE username='" + username + "'";
	std::vector<json> passwordDB = database.Select(statement);

	/*if (passwordDB.size() == 0)
		return true;*/

	if (passwordDB[0]["password"] != password)
		return false;

	return true;
}
/*void Follow(MyDb& db, uint32_t followerId, uint32_t followedId)
{

}*/
