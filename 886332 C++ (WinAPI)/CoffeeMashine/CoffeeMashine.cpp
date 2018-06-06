#include "stdafx.h"
#include "CoffeeMashine.h"
#include "Coffee.h"

CoffeeMashine::CoffeeMashine()
{
	// тестовая инициализация доступных порций кофе
	for(int i = 0; i < 10; i++) {
		coffeeAvail.push_back(Coffee(Cappuccino, 50.0));
	}
	for (int i = 0; i < 5; i++) {
		coffeeAvail.push_back(Coffee(Americano, 30.0));
	}
	for (int i = 0; i < 5; i++) {
		coffeeAvail.push_back(Coffee(Espresso, 25.0));
	}
	// тестовая инициализация доступных банкнот и монет
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
	// Поиск имеющейся в наличии порции
	CoffeeAvail::const_iterator coffee;
	for (coffee = coffeeAvail.begin(); coffee != coffeeAvail.end(); coffee++) {
		if (coffee->getKind() == kind) {
			break;
		}
	}
	// Порций такого кофе нет
	if (coffee == coffeeAvail.end())
		throw NotAvail(kind);
	// Расчёт сдачи
	double change = sum - coffee->getPrice();
	if(change < 0)
		throw NotEnoughMoney(kind, -change);
	// Набираем сдачу из имеющихся купюр и монет
	Cash cash;
	auto it = cashAvail.rbegin();
	// Перебираем с конца (map отсортирован по ключу, так что в конце крупные)
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
