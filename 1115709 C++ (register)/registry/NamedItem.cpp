#include "stdafx.h"
#include "NamedItem.h"

NamedItem::NamedItem(int id, const tstring& name) : UniqueItem(id), _name(name) {
}

void NamedItem::serialize(tostream& stream) const
{
	UniqueItem::serialize(stream);
	stream << _name << '\t';
}

bool NamedItem::deserialize(tistream& stream)
{
	return UniqueItem::deserialize(stream) && (stream >> _name);
}

