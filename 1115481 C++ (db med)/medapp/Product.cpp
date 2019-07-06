#include "pch.h"
#include "Product.h"

#include <iomanip>

Product::Product(int id, const std::string& name, double price) : NamedItem(id, name), mPrice(price) {
}

void Product::serialize(std::ostream& stream) const {
	NamedItem::serialize(stream);
	stream << std::fixed << std::setprecision(2) << mPrice << '\t';
}

bool Product::deserialize(std::istream& stream) {
	return NamedItem::deserialize(stream) && (stream >> mPrice);
}

