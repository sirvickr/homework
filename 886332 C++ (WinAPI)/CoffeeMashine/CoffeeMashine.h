#pragma once

class Coffee;

// Номинал купюры или монеты
enum CashValue {
	coin1 = 1,
	coin2 = 2,
	coin5 = 5,
	coin10 = 10,
	note50 = 50,
	note100 = 100,
	note500 = 500,
};

// Кофейный автомат
class CoffeeMashine
{
public:
	// Типы кофейных напитков
	enum CoffeeKind {
		Cappuccino,
		Espresso,
		Americano
	};
	
	typedef std::map<CashValue, int> Cash;
	typedef std::list<Coffee> CoffeeAvail;

	// Классы исключений:
	// Базовый класс всех исключений
	class Error {
	public:
		Error(CoffeeKind kind) : kind(kind) {
		}
	private:
		// тип кофе
		CoffeeKind kind;
	};
	// Порций данного типа нет в наличии
	class NotAvail : public Error {
	public:
		NotAvail(CoffeeKind kind) : Error(kind) {
		}
	};
	// На выбранную порцию не хватает внесённой суммы
	class NotEnoughMoney : public Error {
	public:
		NotEnoughMoney(CoffeeKind kind, double diff) : Error(kind), diff(diff) {
		}
		// сколько не хватает
		double diff;
	};
	// Нет такого набора купюр, чтобы выдать сдачу с внес1нной суммы
	class NoChange : public Error {
	public:
		NoChange(CoffeeKind kind, double remainder) : Error(kind), remainder(remainder) {
		}
	private:
		// тип кофе
		CoffeeKind kind;
		// сколько не удалось разменять
		double remainder;
	};

	// Конструктор
	CoffeeMashine();
	// Приготовление кофе:
	// принимает тип кофе и сумму денег
	// возвращает порцию кофе и сдачу
	std::pair<Coffee, CoffeeMashine::Cash> Cook(CoffeeKind kind, double sum);
private:
	// доступные порции кофе
	CoffeeAvail coffeeAvail;
	// доступная наличность (пары номинал-количество)
	Cash cashAvail;
};
