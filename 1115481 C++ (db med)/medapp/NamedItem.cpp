#include "pch.h"
#include "NamedItem.h"

NamedItem::NamedItem(int id, const std::string& name) : UniqueItem(id), mName(name) {
}

void NamedItem::serialize(std::ostream& stream) const
{
	UniqueItem::serialize(stream);
	stream << mName << '\t';
}

bool NamedItem::deserialize(std::istream& stream)
{
	return UniqueItem::deserialize(stream) && (stream >> mName);
}

