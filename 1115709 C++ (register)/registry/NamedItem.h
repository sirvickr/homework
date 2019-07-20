#pragma once

#include "UniqueItem.h"

class NamedItem : public UniqueItem
{
public:
	NamedItem() = default;
	NamedItem(int id, const tstring& name);
	virtual ~NamedItem() = default;

	void serialize(tostream& stream) const override;
	bool deserialize(tistream& stream) override;

	void Name(const tstring& value) {
		_name = value;
	}
	const tstring& Name() const {
		return _name;
	}

private:
	tstring _name;
};

