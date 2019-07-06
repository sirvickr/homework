#pragma once

#include "UniqueItem.h"
#include <list>

class Details : public DbItem {
public:
	Details() = default;
	~Details() = default;

	void serialize(std::ostream& stream) const override;
	bool deserialize(std::istream& stream) override;

	void IdSale(int value) {
		mIdSale = value;
	}
	int IdSale() const {
		return mIdSale;
	}

	void IdProduct(int value) {
		mIdProduct = value;
	}
	int IdProduct() const {
		return mIdProduct;
	}

	void IdCustomer(int value) {
		mIdCustomer = value;
	}
	int IdCustomer() const {
		return mIdCustomer;
	}

	void Count(int value) {
		mCount = value;
	}
	int Count() const {
		return mCount;
	}

private:
	int mIdSale = 0;
	int mIdProduct = 0;
	int mIdCustomer = 0;
	int mCount = 0;
};

class Sale : public UniqueItem
{
public:
	Sale() = default;
	Sale(int id, const std::string& date);
	~Sale() = default;

	void serialize(std::ostream& stream) const override;
	bool deserialize(std::istream& stream) override;

	void Date(const std::string& value) {
		mDate = value;
	}
	std::string Date() const {
		return mDate;
	}

	void State(int value) {
		mState = value;
	}
	int State() const {
		return mState;
	}

private:
	std::string mDate;
	// Статус:
	// 0 - счёт не выставлен
	// 1 - счёт выставлен
	// 2 - счёт оплачен
	int mState = 0;
};

