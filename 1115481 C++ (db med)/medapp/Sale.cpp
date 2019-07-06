#include "pch.h"
#include "Sale.h"

Sale::Sale(int id, const std::string& date) : UniqueItem(id), mDate(date) {
}

void Sale::serialize(std::ostream& stream) const {
	UniqueItem::serialize(stream);
	stream << mDate << '\t' << mState << '\t';
}

bool Sale::deserialize(std::istream& stream) {
	if (!UniqueItem::deserialize(stream))
		return false;
	if (!(stream >> mDate))
		return false;
	if (!(stream >> mState))
		return false;
	return true;
}

void Details::serialize(std::ostream& stream) const {
	stream << mIdSale << '\t' << mIdProduct << '\t' << mIdCustomer << '\t' << mCount << '\t';
}

bool Details::deserialize(std::istream& stream) {
	if (!(stream >> mIdSale))
		return false;
	if (!(stream >> mIdProduct))
		return false;
	if (!(stream >> mIdCustomer))
		return false;
	if (!(stream >> mCount))
		return false;
	return true;
}
