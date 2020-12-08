#pragma once

#include <string>

// товар
class Commodity
{
public:
	Commodity(double price = 0.0, const std::string& manufacturer = "") : mPrice(price), mManufacturer(manufacturer) {
	}
	double price() const {
		return mPrice;
	}
	void price(double value) {
		mPrice = value;
	}
	const std::string& manufacturer() const {
		return mManufacturer;
	}
	void manufacturer(const std::string& value) {
		mManufacturer = value;
	}
private:
	// производитель
	std::string mManufacturer;
	// цена
	double mPrice;
};
