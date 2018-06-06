#include "stdafx.h"
#include "CoffeeMashine.h"
#include "Coffee.h"

CoffeeMashine::CoffeeMashine()
{
	// �������� ������������� ��������� ������ ����
	for(int i = 0; i < 10; i++) {
		coffeeAvail.push_back(Coffee(Cappuccino, 50.0));
	}
	for (int i = 0; i < 5; i++) {
		coffeeAvail.push_back(Coffee(Americano, 30.0));
	}
	for (int i = 0; i < 5; i++) {
		coffeeAvail.push_back(Coffee(Espresso, 25.0));
	}
	// �������� ������������� ��������� ������� � �����
	cashAvail[note500] = 1;
	cashAvail[note100] = 10;
	cashAvail[note50] = 20;
	cashAvail[coin10] = 50;
	cashAvail[coin5] = 100;
	cashAvail[coin2] = 100;
	cashAvail[coin1] = 200;
}

std::pair<Coffee, CoffeeMashine::Cash> CoffeeMashine::Cook(CoffeeKind kind, double sum)
{
	// ����� ��������� � ������� ������
	CoffeeAvail::const_iterator coffee;
	for (coffee = coffeeAvail.begin(); coffee != coffeeAvail.end(); coffee++) {
		if (coffee->getKind() == kind) {
			break;
		}
	}
	// ������ ������ ���� ���
	if (coffee == coffeeAvail.end())
		throw NotAvail(kind);
	// ������ �����
	double change = sum - coffee->getPrice();
	if(change < 0)
		throw NotEnoughMoney(kind, -change);
	// �������� ����� �� ��������� ����� � �����
	Cash cash;
	auto it = cashAvail.rbegin();
	// ���������� � ����� (map ������������ �� �����, ��� ��� � ����� �������)
	while (it != cashAvail.rend()) {
		bool ranOut = false;
		CashValue value = it->first;
		while (change >= value) {
			change -= it->first;
			cash[it->first]++;
			it->second--;
			if (!it->second) {
				it = static_cast<decltype(it)>(cashAvail.erase(std::next(it).base()));
				ranOut = true;
			}
		}
		if (!ranOut) {
			it++;
		}
	}
	if (change != 0)
		throw NoChange(kind, change);
	std::pair<Coffee, Cash> result(*coffee, cash);
	coffeeAvail.erase(coffee);
	return result;
}
