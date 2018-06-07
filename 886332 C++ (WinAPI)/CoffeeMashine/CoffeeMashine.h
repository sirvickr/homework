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
	typedef std::map<CashValue, int> Cash;
	typedef std::map<std::string, Coffee> CoffeeAvail;

	// ������ ����������:
	// ������� ����� ���� ����������
	class Error {
	public:
		Error(const std::string& kind) : kind(kind) {
		}
	private:
		// ��� ����
		std::string kind;
	};
	// ������ ������� ���� ��� � �������
	class NotAvail : public Error {
	public:
		NotAvail(const std::string& kind) : Error(kind) {
		}
	};
	// �� ��������� ������ �� ������� �������� �����
	class NotEnoughMoney : public Error {
	public:
		NotEnoughMoney(const std::string& kind, double diff) : Error(kind), diff(diff) {
		}
		// ������� �� �������
		double diff;
	};
	// ��� ������ ������ �����, ����� ������ ����� � ����1���� �����
	class NoChange : public Error {
	public:
		NoChange(const std::string& kind, double remainder) : Error(kind), remainder(remainder) {
		}
	private:
		// ������� �� ������� ���������
		double remainder;
	};

	// �����������
	CoffeeMashine(const std::string& coffeeFileName = "", const std::string& cashFileName = "");

	// �������� ������������ � ��������:
	// ���������� �������� ����������� � ������� ����
	const CoffeeAvail& getCoffeeAvail() const {
		return coffeeAvail;
	}

	// ������������� ����:
	// ��������� ��� ���� � ����� �����
	// ���������� ������ ���� � �����
	std::pair<Coffee, CoffeeMashine::Cash> Cook(const std::string& kind, double sum);

	// ��������� �������� ����������� ����
	const CoffeeAvail& getCoffeeAvailable() const {
		return coffeeAvail;
	}
private:
	// ��������� ������ ����
	CoffeeAvail coffeeAvail;
	// ��������� ���������� (���� �������-����������)
	Cash cashAvail;
};
