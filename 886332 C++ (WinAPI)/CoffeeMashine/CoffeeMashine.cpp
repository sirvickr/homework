#include "stdafx.h"
#include "CoffeeMashine.h"
#include "Coffee.h"

CoffeeMashine::CoffeeMashine(const std::string& coffeeFileName, const std::string& cashFileName)
{
	file = fopen("log.txt", "w");

	if (coffeeFileName.empty()) {
		// тестовая инициализация доступных порций кофе
		for (int i = 0; i < 10; i++) {
			coffeeAvail.push_back(Coffee(Cappuccino, 50.0));
		}
		for (int i = 0; i < 5; i++) {
			coffeeAvail.push_back(Coffee(Espresso, 25.0));
		}
		for (int i = 0; i < 5; i++) {
			coffeeAvail.push_back(Coffee(Americano, 30.0));
		}
	} else {
		fprintf(file, "Read coffee\n");
		std::ifstream input(coffeeFileName);
		if (input) {
			int kind, count;
			double price;
			while (input >> kind >> price >> count) {
				fprintf(file, " kind %d price %.2f count %d\n", kind, price, count);
				if(kind < MaxKindValue) for (int i = 0; i < count; i++) {
					coffeeAvail.push_back(Coffee(kind, price));
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
		fprintf(file, "Read cash\n");
		std::ifstream input(cashFileName);
		if (input) {
			int value, count;
			while (input >> value >> count) {
				fprintf(file, " value %d count %d\n", value, count);
				cashAvail[static_cast<CashValue>(value)] = count;
			}
		}
	}
}

std::pair<Coffee, CoffeeMashine::Cash> CoffeeMashine::Cook(CoffeeKind kind, double sum)
{
	fprintf(file, "Cook(kind %d sum %.2f)\n", kind, sum);

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
	fprintf(file, "sum %.2f price %.2f change %.2f\n", sum, coffee->getPrice(), change);
	if(change < 0)
		throw NotEnoughMoney(kind, -change);

	fprintf(file, "cashAvail\n");
	for (auto it = cashAvail.rbegin(); it != cashAvail.rend(); it++) {
		fprintf(file, " valule %d count %d\n", (int)it->first, it->second);
	}
	// Набираем сдачу из имеющихся купюр и монет
	Cash cash;
	auto it = cashAvail.rbegin();
	// Перебираем с конца (map отсортирован по ключу, так что в конце крупные)
	//int counter = 0;
	while (it != cashAvail.rend()) {
		bool ranOut = false;
		fprintf(file, " value %d change %.2f", (int)it->first, change);
		while (change >= it->first) {
			change -= it->first;
			fprintf(file, " (->%.2f,notes:%d", change, it->second);
			cash[it->first]++;
			it->second--;
			fprintf(file, "->%d)", it->second);
			if (!it->second) {
				it = static_cast<decltype(it)>(cashAvail.erase(std::next(it).base()));
				ranOut = true;
				fprintf(file, " ran out:");
				if (it == cashAvail.rend()) {
					fprintf(file, " no more cash");
					break;
				} else {
					fprintf(file, " next is {%d: %d}", (int)it->first, it->second);
				}
				//if (++counter == 100) throw Error(kind);
			}
		}
		if (change == 0) {
			fprintf(file, "done\n");
			break;
		}
		if (!ranOut) {
			it++;
		}
		fprintf(file, ": next is {%d: %d}\n", (int)it->first, it->second);
	}
	if (change != 0)
		throw NoChange(kind, change);
	std::pair<Coffee, Cash> result(*coffee, cash);
	coffeeAvail.erase(coffee);
	return result;
}
