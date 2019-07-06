#pragma once

#include <string>
#include <ostream>

struct DbItem
{
	DbItem() = default;
	virtual ~DbItem() = 0;
	virtual void serialize(std::ostream& stream) const = 0;
	virtual bool deserialize(std::istream& stream) = 0;
};

