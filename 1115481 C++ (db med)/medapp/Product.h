#pragma once

#include "NamedItem.h"

class Product : public NamedItem
{
public:
	Product() = default;
	Product(int id, const std::string& name, double price);
	~Product() = default;

	void serialize(std::ostream& stream) const override;
	bool deserialize(std::istream& stream) override;

	void Price(double value) {
		mPrice = value;
	}
	double Price() const {
		return mPrice;
	}
	
private:
	double mPrice = 0.0;
};

