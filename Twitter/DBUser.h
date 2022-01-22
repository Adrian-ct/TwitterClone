#pragma once
#include "IEntity.h"
#include <string>
class DBUser: public IEntity
{
public:
	void Insert(MyDB& db, const json& user)const override;
	json Fetch(MyDB& db, const uint32_t id) const override;
	json Fetch(MyDB& db, const std::string& username) const;
	std::vector<uint32_t> FetchAllIds(const MyDB& db) const;
	std::vector<json> FetchAllIdsAndUsernames(const MyDB& db) const;
	json LogIn(MyDB& db, const std::string& username, const std::string& password);

};

