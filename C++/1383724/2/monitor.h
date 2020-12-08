#pragma once

#include "commodity.h"

#include <string>

// компьютерный монитор
class Monitor : public Commodity
{
public:
	Monitor(double diag = 0.0, double price = 0.0, const std::string& manufacturer = "") : Commodity(price, manufacturer), mDiag(diag) {
	}
	double diag() const {
		return mDiag;
	}
	void diag(double value) {
		mDiag = value;
	}
private:
	// диагональ
	double mDiag;
};
