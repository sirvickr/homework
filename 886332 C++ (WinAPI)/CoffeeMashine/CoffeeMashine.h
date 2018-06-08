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
	typedef std::map<tstring, Coffee> CoffeeAvail;

	// ������ ����������:
	// ������� ����� ���� ����������
	class Error {
	public:
		Error(const tstring& kind) : kind(kind) {
		}
	private:
		// ��� ����
		tstring kind;
	};
	// ������ ������� ���� ��� � �������
	class NotAvail : public Error {
	public:
		NotAvail(const tstring& kind) : Error(kind) {
		}
	};
	// �� ��������� ������ �� ������� �������� �����
	class NotEnoughMoney : public Error {
	public:
		NotEnoughMoney(const tstring& kind, double diff) : Error(kind), diff(diff) {
		}
		// ������� �� �������
		double diff;
	};
	// ��� ������ ������ �����, ����� ������ ����� � ����1���� �����
	class NoChange : public Error {
	public:
		NoChange(const tstring& kind, double remainder) : Error(kind), remainder(remainder) {
		}
	private:
		// ������� �� ������� ���������
		double remainder;
	};

	// �����������
	CoffeeMashine(const tstring& coffeeFileName = "", const tstring& cashFileName = "");

	// �������� ������������ � ��������:
	// ���������� �������� ����������� � ������� ����
	const CoffeeAvail& getCoffeeAvail() const {
		return coffeeAvail;
	}

	// ������������� ����:
	// ��������� ��� ���� � ����� �����
	// ���������� ������ ���� � �����
	std::pair<Coffee, CoffeeMashine::Cash> Cook(const tstring& kind, double sum);

private:
	// ��������� ������ ����
	CoffeeAvail coffeeAvail;
	// ��������� ���������� (���� �������-����������)
	Cash cashAvail;

private:
	// �������� ������ ���� �� �����:
	// ���� �� ������� ������� ����, ��������� �� ��������� � ���������� false
	bool loadCoffee(const tstring& fileName);
	// �������� �������/����� �� �����:
	// ���� �� ������� ������� ����, ��������� �� ��������� � ���������� false
	bool loadCash(const tstring& fileName);
};
