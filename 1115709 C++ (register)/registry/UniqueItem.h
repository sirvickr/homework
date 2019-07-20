#pragma once

#include "DbItem.h"

class UniqueItem : public DbItem
{
public:
	UniqueItem() = default;
	UniqueItem(int id);
	~UniqueItem() = default;

	void serialize(tostream& stream) const override;
	bool deserialize(tistream& stream) override;

	void Id(int value) {
		_id = value;
	}
	int Id() const {
		return _id;
	}

private:
	int _id = 0;
};

