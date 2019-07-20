#include "stdafx.h"
#include "UniqueItem.h"

UniqueItem::UniqueItem(int id) : _id(id) {
}

void UniqueItem::serialize(tostream& stream) const {
	stream << _id << '\t';
}

bool UniqueItem::deserialize(tistream& stream) {
	return static_cast<bool>(stream >> _id);
}
