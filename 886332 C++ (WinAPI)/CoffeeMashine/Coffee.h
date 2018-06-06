#pragma once

// ������ ����
class Coffee
{
public:
	Coffee(int kind = 0, double price = 0.0);
	inline int getKind() const { return kind; }
	inline void setKind(int value) { kind = value; }
	inline double getPrice() const { return price; }
	inline void setPrice(double value) { price = value; }
private:
	// ��� ����
	int kind;
	// ���� ������
	double price;
};

