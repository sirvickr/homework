#include "stdafx.h"
#include "CoffeeMashine.h"
#include "Coffee.h"

bool CoffeeMashine::loadCoffee(const tstring& fileName)
{
	bool result = false;
	tstring coffeeKind;
	std::ifstream input(fileName);
	if (input) {
		int count;
		double price;
		while (input >> coffeeKind >> price >> count) {
			for (int i = 0; i < count; i++) {
				coffeeAvail.insert(CoffeeAvail::value_type(coffeeKind, Coffee(coffeeKind, price, count)));
			}
		}
		result = true;
	} else {
		// �� ������� ������� ���� - ��������� �� ���������
		coffeeKind = "��������";
		coffeeAvail.insert(CoffeeAvail::value_type(coffeeKind, Coffee(coffeeKind, 50.0, 10)));
		coffeeKind = "��������";
		coffeeAvail.insert(CoffeeAvail::value_type(coffeeKind, Coffee(coffeeKind, 25.0, 10)));
		coffeeKind = "���������";
		coffeeAvail.insert(CoffeeAvail::value_type(coffeeKind, Coffee(coffeeKind, 30.0, 10)));
		result = false;
	}
	return result;
}

bool CoffeeMashine::loadCash(const tstring& fileName)
{
	bool result = false;
	std::ifstream input(fileName);
	if (input) {
		int value, count;
		while (input >> value >> count) {
			cashAvail[static_cast<CashValue>(value)] = count;
		}
		result = true;
	} else {
		// �� ������� ������� ���� - ��������� �� ���������
		cashAvail[note500] = 1;
		cashAvail[note100] = 10;
		cashAvail[note50] = 20;
		cashAvail[coin10] = 50;
		cashAvail[coin5] = 100;
		cashAvail[coin2] = 100;
		cashAvail[coin1] = 200;
		result = false;
	}
	return result;
}

CoffeeMashine::CoffeeMashine(const tstring& coffeeFileName, const tstring& cashFileName)
{
	loadCoffee(coffeeFileName);
	loadCash(cashFileName);
}

std::pair<Coffee, CoffeeMashine::Cash> CoffeeMashine::Cook(const tstring& kind, double sum)
{
	// ����� ��������� � ������� ������
	auto coffee = coffeeAvail.find(kind);
	// ������ ������ ���� ���?
	if (coffee == coffeeAvail.end())
		throw NotAvail(kind);
	// ������ �����
	double change = sum - coffee->second.getPrice();
	if(change < 0)
		throw NotEnoughMoney(kind, -change);

	// �������� ����� �� ��������� ����� � �����
	Cash cash;
	auto it = cashAvail.rbegin();
	// ���������� � ����� (map ������������ �� �����, ��� ��� � ����� �������)
	while (it != cashAvail.rend()) {
		bool ranOut = false;
		while (change >= it->first) {
			change -= it->first;
			cash[it->first]++;
			it->second--;
			if (!it->second) {
				it = static_cast<decltype(it)>(cashAvail.erase(std::next(it).base()));
				ranOut = true;
				if (it == cashAvail.rend()) {
					break;
				}
			}
		}
		if (change == 0) {
			break;
		}
		if (!ranOut) {
			it++;
		}
	}
	if (change != 0)
		throw NoChange(kind, change);
	// �������� ���� ������ ���� �� �������
	std::pair<Coffee, Cash> result(coffee->second, cash);
	if(0 == coffee->second.takeSome(1)) {
		// ������ ����� ���� �����������
		coffeeAvail.erase(coffee);
	}
	return result;
}
