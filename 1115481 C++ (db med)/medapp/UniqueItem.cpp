#include "pch.h"
#include "UniqueItem.h"

UniqueItem::UniqueItem(int id) : mId(id) {
}

void UniqueItem::serialize(std::ostream& stream) const {
	stream << mId << '\t';
}

bool UniqueItem::deserialize(std::istream& stream) {
	return static_cast<bool>(stream >> mId);
}
