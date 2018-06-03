#include "stdafx.h"

#include "Operation.h"

Operation::Operation(Operation* l, Operation* r, Operations code, double value) 
	: left(l), right(r), code(code), value(value)
{
}

Operation::~Operation() {
	// Каждая операция управляет памятью своих операндов
	if(left)
		delete left;
	if(right)
		delete right;
}

double Operation::Calculate() {
	if(left && right) {
		// этот экземпляр является операцией - необходимо
		// вычислить текущий результат по двум операндам
		double a = left->Calculate();
		double b = right->Calculate();
		switch(code) {
		case opSum:
			value = a + b;
			break;
		case opSub:
			value = a - b;
			break;
		case opMul:
			value = a * b;
			break;
		case opDiv:
			value = a / b;
			break;
		case opPow:
			value = pow(a, b);
			break;
		case opMod:
			value = (UINT64)(a + 0.5) % (UINT64)(b + 0.5);
			break;
		}
	}
	// если операнды не назначены, то этот экземпляр сам является
	// операндом и нужно просто вернуть текущее значение
	return value;
}
