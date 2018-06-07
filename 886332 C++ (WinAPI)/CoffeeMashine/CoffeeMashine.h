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
	typedef std::map<CashValue, int> Cash;
	typedef std::map<std::string, Coffee> CoffeeAvail;

	// Классы исключений:
	// Базовый класс всех исключений
	class Error {
	public:
		Error(const std::string& kind) : kind(kind) {
		}
	private:
		// тип кофе
		std::string kind;
	};
	// Порций данного типа нет в наличии
	class NotAvail : public Error {
	public:
		NotAvail(const std::string& kind) : Error(kind) {
		}
	};
	// На выбранную порцию не хватает внесённой суммы
	class NotEnoughMoney : public Error {
	public:
		NotEnoughMoney(const std::string& kind, double diff) : Error(kind), diff(diff) {
		}
		// сколько не хватает
		double diff;
	};
	// Нет такого набора купюр, чтобы выдать сдачу с внес1нной суммы
	class NoChange : public Error {
	public:
		NoChange(const std::string& kind, double remainder) : Error(kind), remainder(remainder) {
		}
	private:
		// сколько не удалось разменять
		double remainder;
	};

	// Конструктор
	CoffeeMashine(const std::string& coffeeFileName = "", const std::string& cashFileName = "");

	// Просмотр ассортимента с ценником:
	// возвращает перечень имеющеегося в наличии кофе
	const CoffeeAvail& getCoffeeAvail() const {
		return coffeeAvail;
	}

	// Приготовление кофе:
	// принимает тип кофе и сумму денег
	// возвращает порцию кофе и сдачу
	std::pair<Coffee, CoffeeMashine::Cash> Cook(const std::string& kind, double sum);

	// Позволяет получить ассортимент кофе
	const CoffeeAvail& getCoffeeAvailable() const {
		return coffeeAvail;
	}
private:
	// доступные порции кофе
	CoffeeAvail coffeeAvail;
	// доступная наличность (пары номинал-количество)
	Cash cashAvail;
};
