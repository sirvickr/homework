#pragma once

// ��������� ��������������� �����
template<uint32_t n>
class PseudoRandomNumberGenerator {
public:
	typedef uint32_t number_t;
	typedef std::vector<number_t> numbers_t;
	PseudoRandomNumberGenerator(uint32_t a, uint32_t b, uint32_t base)
		: m(1 << n), a(a), b(b), xBase(base), xCurr(base)
	{
	}
	// ������� �������
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
	inline number_t Seed() const {
		return xBase;
	}
	inline number_t Curr() const {
		return xCurr;
	}
	// ��������� ���������� ���������
	inline void SetParams(uint32_t a, uint32_t b, uint32_t base) {
		this->a = a;
		this->b = b;
		this->xBase = base;
		this->xCurr = this->xBase;
	}
	// ������������ � ��������� ��������� ���������� ���������
	inline void createParams() {
		// b: �������� ��������� 32-������ ����� �� ������ �������� �������
		uint32_t b = static_cast<uint32_t>(chrono::system_clock::now().time_since_epoch().count()) & 0xFFFF;
		// ������������ ���������� b
		b |= 1;
		// ���������� �������� �����, ������� b, ���� �� ����� ������� ������� � m
		while (gcd(b, m) != 1)
			b += 2;
		// a: �������� ��������� 32-������ ����� �� ������ �������� �������
		uint32_t a = static_cast<uint32_t>(chrono::system_clock::now().time_since_epoch().count()) & 0xFFFF;
		while ((a & 3) != 1)
			a++;
		// c0: �������� ��������� 32-������ ����� �� ������ �������� �������
		uint32_t c0 = static_cast<uint32_t>(chrono::system_clock::now().time_since_epoch().count()) % m;
		SetParams(a, b, c0);
	}
	// ������������ ���������� ���������������� ��������
	inline number_t NextNumber() {
		// ��� n = ������� ������, �������� (n % M) ����� �������� 
		// ����� ������� ��������� (n & (M - 1))
		// ��������, (n % 4) == (n & 3), (n % 32) == (n & 31) � �.�.
		return xCurr = (a * xCurr + b) & (m - 1);
	}
	// ������������ ��������������� ������������������ �����
	numbers_t NextNumbers(size_t count, bool resetSeed = false) {
		numbers_t result(count);
		if (resetSeed)
			xCurr = xBase;
		for (auto& number : result)
			number = NextNumber();
		if (resetSeed)
			xCurr = xBase;
		return result;
	}
private:
	// ����� ��� (�������� �������� �� ������ ������� ����������)
	int gcd(int a, int b) {
		cout << " gcd " << a << ", " << b;
		int shift;
		if (a == 0)
			return b;
		if (b == 0)
			return a;
		for (shift = 0; ((a | b) & 1) == 0; ++shift) {
			a >>= 1;
			b >>= 1;
		}
		while ((a & 1) == 0)
			a >>= 1;
		do {
			while ((b & 1) == 0)
				b >>= 1;
			if (a > b) {
				int t = b;
				b = a;
				a = t;
			}
			b = b - a;
		} while (b != 0);
		cout << ": " << (a << shift) << endl;
		return a << shift;
	}
private:
	// ��������� ���������
	const uint32_t m;
	uint32_t a, b;
	number_t xBase, xCurr;
};
