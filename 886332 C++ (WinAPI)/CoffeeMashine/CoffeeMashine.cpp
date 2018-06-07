#include "stdafx.h"
#include "CoffeeMashine.h"
#include "Coffee.h"

CoffeeMashine::CoffeeMashine(const tstring& coffeeFileName, const tstring& cashFileName)
{
	tstring coffeeKind;

	if (coffeeFileName.empty()) {
		// тестовая инициализация доступных порций кофе
		coffeeKind = "Капучино";
		coffeeAvail.insert(CoffeeAvail::value_type(coffeeKind, Coffee(coffeeKind, 50.0, 10)));
		coffeeKind = "Эспрессо";
		coffeeAvail.insert(CoffeeAvail::value_type(coffeeKind, Coffee(coffeeKind, 25.0, 10)));
		coffeeKind = "Американо";
		coffeeAvail.insert(CoffeeAvail::value_type(coffeeKind, Coffee(coffeeKind, 30.0, 10)));
	} else {
		std::ifstream input(coffeeFileName);
		if (input) {
			int count;
			double price;
			while (input >> coffeeKind >> price >> count) {
				for (int i = 0; i < count; i++) {
					coffeeAvail.insert(CoffeeAvail::value_type(coffeeKind, Coffee(coffeeKind, price, count)));
				}
			}
		}
	}
	if (cashFileName.empty()) {
		// тестовая инициализация доступных банкнот и монет
		cashAvail[note500] = 1;
		cashAvail[note100] = 10;
		cashAvail[note50] = 20;
		cashAvail[coin10] = 50;
		cashAvail[coin5] = 100;
		cashAvail[coin2] = 100;
		cashAvail[coin1] = 200;
	} else {
		std::ifstream input(cashFileName);
		if (input) {
			int value, count;
			while (input >> value >> count) {
				cashAvail[static_cast<CashValue>(value)] = count;
			}
		}
	}
}

std::pair<Coffee, CoffeeMashine::Cash> CoffeeMashine::Cook(const tstring& kind, double sum)
{
	// Поиск имеющейся в наличии порции
	auto coffee = coffeeAvail.find(kind);
	// Порций такого кофе нет?
	if (coffee == coffeeAvail.end())
		throw NotAvail(kind);
	// Расчёт сдачи
	double change = sum - coffee->second.getPrice();
	if(change < 0)
		throw NotEnoughMoney(kind, -change);

	// Набираем сдачу из имеющихся купюр и монет
	Cash cash;
	auto it = cashAvail.rbegin();
	// Перебираем с конца (map отсортирован по ключу, так что в конце крупные)
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
	// забираем одну порцию кофе из запасов
	std::pair<Coffee, Cash> result(coffee->second, cash);
	if(0 == coffee->second.takeSome(1)) {
		// порции этого кофе закончились
		coffeeAvail.erase(coffee);
	}
	return result;
}
