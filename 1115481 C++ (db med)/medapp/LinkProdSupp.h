#pragma once

#include "DbItem.h"

class LinkProdSupp : public DbItem
{
public:
	LinkProdSupp() = default;
	~LinkProdSupp() = default;

	void serialize(std::ostream& stream) const override;
	bool deserialize(std::istream& stream) override;

	void IdProduct(int value) {
		mIdProduct = value;
	}
	int IdProduct() const {
		return mIdProduct;
	}

	void IdSupplier(int value) {
		mIdSupplier = value;
	}
	int IdSupplier() const {
		return mIdSupplier;
	}

private:
	int mIdProduct = 0;
	int mIdSupplier = 0;
};

