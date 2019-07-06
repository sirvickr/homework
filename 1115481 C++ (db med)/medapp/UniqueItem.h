#pragma once

#include "DbItem.h"

class UniqueItem : public DbItem
{
public:
	UniqueItem() = default;
	UniqueItem(int id);
	~UniqueItem() = default;

	void serialize(std::ostream& stream) const override;
	bool deserialize(std::istream& stream) override;

	void Id(int value) {
		mId = value;
	}
	int Id() const {
		return mId;
	}

private:
	int mId = 0;
};

