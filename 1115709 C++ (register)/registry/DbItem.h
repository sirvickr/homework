#pragma once

struct DbItem
{
	DbItem() = default;
	virtual ~DbItem() = 0;
	virtual void serialize(tostream& stream) const = 0;
	virtual bool deserialize(tistream& stream) = 0;
};

