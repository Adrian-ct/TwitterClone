#pragma once
#include "MyDB.h"
#include "nlohmann.hpp"

class IEntity
{
public:
	virtual void Insert(MyDB& db, const json& entity) const = 0;
	virtual json Fetch(MyDB& db,  const uint32_t id) const = 0;
};

