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
	typedef std::map<tstring, Coffee> CoffeeAvail;

	// Классы исключений:
	// Базовый класс всех исключений
	class Error {
	public:
		Error(const tstring& kind) : kind(kind) {
		}
	private:
		// тип кофе
		tstring kind;
	};
	// Порций данного типа нет в наличии
	class NotAvail : public Error {
	public:
		NotAvail(const tstring& kind) : Error(kind) {
		}
	};
	// На выбранную порцию не хватает внесённой суммы
	class NotEnoughMoney : public Error {
	public:
		NotEnoughMoney(const tstring& kind, double diff) : Error(kind), diff(diff) {
		}
		// сколько не хватает
		double diff;
	};
	// Нет такого набора купюр, чтобы выдать сдачу с внесённой суммы
	class NoChange : public Error {
	public:
		NoChange(const tstring& kind, double remainder) : Error(kind), remainder(remainder) {
		}
	private:
		// сколько не удалось разменять
		double remainder;
	};

	// Конструктор
	CoffeeMashine(const tstring& coffeeFileName = _T(""), const tstring& cashFileName = _T(""));

	// Возвращает текущий баланс
	double getBalance() const {
		return balance;
	}

	// Просмотр ассортимента с ценником:
	// возвращает перечень имеющеегося в наличии кофе
	const CoffeeAvail& getCoffeeAvail() const {
		return coffeeAvail;
	}

	// Внесение предоплаты:
	// принимает сумму денег
	// возвращает сумму депозита
	double depositMoney(CashValue money);

	// Приготовление кофе:
	// принимает тип кофе и сумму денег
	// возвращает порцию кофе и сдачу
	Coffee Cook(const tstring& kind);

	// Возвращает сдачу
	Cash getChange();

private:
	// доступные порции кофе
	CoffeeAvail coffeeAvail;
	// доступная наличность (пары номинал-количество)
	Cash cashAvail;
	// сумма внесённой предоплаты (останок на текущий момент)
	double balance;

private:
	// Загрузка порций кофе из файла:
	// если не удалось открыть файл, заполняет по умолчанию и возвращает false
	bool loadCoffee(const tstring& fileName);
	// Загрузка банкнот/монет из файла:
	// если не удалось открыть файл, заполняет по умолчанию и возвращает false
	bool loadCash(const tstring& fileName);
};
