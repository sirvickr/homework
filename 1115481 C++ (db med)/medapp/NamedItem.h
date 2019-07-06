#pragma once

#include "UniqueItem.h"

#include <string>

class NamedItem : public UniqueItem
{
public:
	NamedItem() = default;
	NamedItem(int id, const std::string& name);
	virtual ~NamedItem() = default;

	void serialize(std::ostream& stream) const override;
	bool deserialize(std::istream& stream) override;

	void Name(const std::string& value) {
		mName = value;
	}
	std::string Name() const {
		return mName;
	}

private:
	std::string mName;
};

