#pragma once

class Coffee;

// ������� ������ ��� ������
enum CashValue {
	coin1 = 1,
	coin2 = 2,
	coin5 = 5,
	coin10 = 10,
	note50 = 50,
	note100 = 100,
	note500 = 500,
};

class CoffeeMashine
{
public:
	enum CoffeeKind {
		Cappuccino,
		Espresso,
		Americano
	};
	
	typedef std::map<CashValue, int> Cash;
	typedef std::list<Coffee> CoffeeAvail;

	class Error {
	public:
		Error(CoffeeKind kind) : kind(kind) {
		}
	private:
		// ��� ����
		CoffeeKind kind;
	};

	class NotAvail : public Error {
	public:
		NotAvail(CoffeeKind kind) : Error(kind) {
		}
	};

	class NotEnoughMoney : public Error {
	public:
		NotEnoughMoney(CoffeeKind kind, double diff) : Error(kind), diff(diff) {
		}
		// ������� �� �������
		double diff;
	};

	class NoChange : public Error {
	public:
		NoChange(CoffeeKind kind, double remainder) : Error(kind), remainder(remainder) {
		}
	private:
		// ��� ����
		CoffeeKind kind;
		// ������� �� ������� ���������
		double remainder;
	};

	// �����������
	CoffeeMashine();
	// ������������� ����:
	// ��������� ��� ���� � ����� �����
	// ���������� ������ ���� � �����
	std::pair<Coffee, CoffeeMashine::Cash> Cook(CoffeeKind kind, double sum);
private:
	// ��������� ������ ����
	CoffeeAvail coffeeAvail;
	// ��������� ���������� (���� �������-����������)
	Cash cashAvail;
};

