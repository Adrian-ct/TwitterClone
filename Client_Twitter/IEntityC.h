#pragma once
#include "nlohmann.hpp"
#include "Util.h"
using namespace nlohmann;
class IEntityC
{
public:
	virtual json MembersToJson()const = 0;
};

