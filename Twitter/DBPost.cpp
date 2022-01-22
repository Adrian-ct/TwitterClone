#include "DBPost.h"
json DBPost::FetchAll(MyDB& db)
{
	std::vector<json> postsAsVector = db.Select("post_id,text,posted_by,parent_post,post_time FROM Post");

	json posts;

	for (json& post : postsAsVector)
	{
		std::vector<json> likes, dislikes;

		likes = db.Select("count(reaction_type) from PostReaction where reaction_type = 1 and reacted_on = " + static_cast<std::string>(post["post_id"]));
		dislikes = db.Select("count(reaction_type) from PostReaction where reaction_type = 2 and reacted_on = " + static_cast<std::string>(post["post_id"]));

		post["likes"] = likes[0]["count(reaction_type)"];
		post["dislikes"] = dislikes[0]["count(reaction_type)"];

		posts[static_cast<std::string>(post["post_id"])] = post;
	}

	return posts;
}
json DBPost::FetchAll(MyDB& db, const uint32_t& userId) const
{
	std::vector<json> postsAsVector = db.Select("post_id,text,parent_post,post_time FROM Post WHERE posted_by = " + std::to_string(userId));

	json posts;

	for (json& post : postsAsVector)
	{
		std::vector<json> likes, dislikes;

		likes = db.Select("count(reaction_type) from PostReaction where reaction_type = 1 and reacted_on = " + static_cast<std::string>(post["post_id"]));
		dislikes = db.Select("count(reaction_type) from PostReaction where reaction_type = 2 and reacted_on = " + static_cast<std::string>(post["post_id"]));

		post["likes"] = likes[0]["count(reaction_type)"];
		post["dislikes"] = dislikes[0]["count(reaction_type)"];

		posts[static_cast<std::string>(post["post_id"])] = post;
	}

	//std::cout << std::setw(4) << posts;

	return posts;
}
