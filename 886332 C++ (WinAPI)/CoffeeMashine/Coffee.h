#pragma once

// Порция кофе
class Coffee
{
public:
	// класс исключения (недостаточно порций)
	class NotEnough {
	public:
		NotEnough(int count) : count(count) {
		}
		int getCount() const {
			return count;
		}
	private:
		int count;
	};
	Coffee(const tstring& kind = _T(""), double price = 0.0, int count = 0);
	inline tstring getKind() const { return kind; }
	inline void setKind(const tstring& value) { kind = value; }
	inline double getPrice() const { return price; }
	inline void setPrice(double value) { price = value; }
	inline int getCount() const { return count; }
	inline void setCount(int value) { count = value; }
	// забрать несколько порций (возвращает оставшееся количество)
	int takeSome(int count);
	// добавить несколько порций (возвращает оставшееся количество)
	int addSome(int count);
private:
	// тип кофе
	tstring kind;
	// цена порции
	double price;
	// количество порций
	int count;
};

