#pragma once
#include "IEntity.h"
#include <string>
class DBPost
{
public:
	
	json FetchAll(MyDB& db);
	json FetchAll(MyDB& db, const uint32_t& userId) const;

};
