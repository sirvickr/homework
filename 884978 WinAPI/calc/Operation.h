#pragma once

// Типы операций
enum Operations {
	opNop, opSum, opSub, opMul, opDiv, opSqrt, opPow, opMod
};

// Класс, моделирующий операцию калькулятора
class Operation {
public:
	// Конструктор 
	Operation(Operation* l = nullptr, Operation* r = nullptr, Operations code = opNop, double value = 0.0);
	// Деструктор
	~Operation();
	// Метод доступа к типу операции
	inline Operations Code() const {
		return code;
	}
	// Модификатор типа операции
	inline void Code(Operations val) {
		code = val;
	}
	// Метод доступа к текущему значению
	inline double Value() const {
		return value;
	}
	// Модификатор текущего значения
	inline void Value(double val) {
		value = val;
	}
	// Метод доступа к левому операнду
	inline Operation* Left() {
		return left;
	}
	// Метод доступа к правому операнду
	inline Operation* Right() {
		return right;
	}
	// Вычисление результатов операции
	double Calculate();
private:
	// Левый операнд
	Operation* left;
	// Правый операнд
	Operation* right;
	// Тип операции
	Operations code;
	// Текущее значение (либо значение операнда, либо результат вычислений)
	double value;
};
