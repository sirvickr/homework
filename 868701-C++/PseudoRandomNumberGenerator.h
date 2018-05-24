#pragma once

// Генератор псевдослучайных чисел
class PseudoRandomNumberGen {
public:
	PseudoRandomNumberGen(uint32_t n, uint32_t a, uint32_t b, uint32_t base)
		: n(n), m(1 << n), a(a), b(b), xBase(base), xCurr(base)
	{
	}
	inline const uint32_t N() const {
		return n;
	}
	inline const uint32_t M() const {
		return m;
	}
	inline uint32_t A() const {
		return a;
	}
	inline uint32_t B() const {
		return b;
	}
	inline uint32_t Base() const {
		return xBase;
	}
	inline uint32_t Curr() const {
		return xCurr;
	}
	inline void SetParams(uint32_t a, uint32_t b, uint32_t base) {
		this->a = a;
		this->b = b;
		this->xBase = base;
		this->xCurr = this->xBase;
	}
	inline uint32_t NextNumber() {
		// для n = степени двойки, операцию (n % M) можно заменить 
		// более быстрой операцией (n & (M - 1))
		// например, (n % 4) == (n & 3), (n % 32) == (n & 31) и т.д.
		return xCurr = (a * xCurr + b) & (m - 1);
	}
	std::vector<uint32_t> NextNumbers(size_t count, bool reset = false) {
		std::vector<uint32_t> result(count);
		if (reset)
			xCurr = xBase;
		for (auto& number : result)
			number = NextNumber();
		if (reset)
			xCurr = xBase;
		return result;
	}
private:
	// параметры алгоритма
	const uint32_t n;
	const uint32_t m;
	uint32_t a, b;
	uint32_t xBase, xCurr;
};
