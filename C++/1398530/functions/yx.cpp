#include "pch.h"
#include "yx.h"

#include <cmath>

double yx(double x) {
	double y = 0.0;
	if(x <= -4.0)
		return 3;
	else if(x <= 4)
		return x * x - 4 * std::abs(x) + 3;
	double t = x - 4;
	return 3 - t * t;
}
