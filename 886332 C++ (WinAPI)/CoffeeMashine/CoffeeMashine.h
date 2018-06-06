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

// �������� �������
class CoffeeMashine
{
public:
	// ���� �������� ��������
	enum CoffeeKind {
		Cappuccino,
		Espresso,
		Americano,
		MaxKindValue
	};
	
	typedef std::map<CashValue, int> Cash;
	typedef std::list<Coffee> CoffeeAvail;

	// ������ ����������:
	// ������� ����� ���� ����������
	class Error {
	public:
		Error(CoffeeKind kind) : kind(kind) {
		}
	private:
		// ��� ����
		CoffeeKind kind;
	};
	// ������ ������� ���� ��� � �������
	class NotAvail : public Error {
	public:
		NotAvail(CoffeeKind kind) : Error(kind) {
		}
	};
	// �� ��������� ������ �� ������� �������� �����
	class NotEnoughMoney : public Error {
	public:
		NotEnoughMoney(CoffeeKind kind, double diff) : Error(kind), diff(diff) {
		}
		// ������� �� �������
		double diff;
	};
	// ��� ������ ������ �����, ����� ������ ����� � ����1���� �����
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
	CoffeeMashine(const std::string& coffeeFileName = "", const std::string& cashFileName = "");
	~CoffeeMashine() {
		fclose(file);
	}
	// ������������� ����:
	// ��������� ��� ���� � ����� �����
	// ���������� ������ ���� � �����
	std::pair<Coffee, CoffeeMashine::Cash> Cook(CoffeeKind kind, double sum);

	// ��������� �������� ����������� ����
	const CoffeeAvail& getCoffeeAvailable() const {
		return coffeeAvail;
	}
private:
	// ��������� ������ ����
	CoffeeAvail coffeeAvail;
	// ��������� ���������� (���� �������-����������)
	Cash cashAvail;
	FILE* file=0;
};
