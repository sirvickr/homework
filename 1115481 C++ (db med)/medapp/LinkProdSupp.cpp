#include "pch.h"
#include "LinkProdSupp.h"

void LinkProdSupp::serialize(std::ostream& stream) const {
	stream << mIdProduct << '\t' << mIdSupplier << '\t';
}

bool LinkProdSupp::deserialize(std::istream& stream) {
	if (!(stream >> mIdProduct))
		return false;
	if (!(stream >> mIdSupplier))
		return false;
	return true;
}

