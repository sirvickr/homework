#pragma once

// ���� ��������
enum Operations {
	opNop, opSum, opSub, opMul, opDiv, opSqrt, opPow, opMod
};

// �����, ������������ �������� ������������
class Operation {
public:
	// ����������� 
	Operation(Operation* l = nullptr, Operation* r = nullptr, Operations code = opNop, double value = 0.0);
	// ����������
	~Operation();
	// ����� ������� � ���� ��������
	inline Operations Code() const {
		return code;
	}
	// ����������� ���� ��������
	inline void Code(Operations val) {
		code = val;
	}
	// ����� ������� � �������� ��������
	inline double Value() const {
		return value;
	}
	// ����������� �������� ��������
	inline void Value(double val) {
		value = val;
	}
	// ����� ������� � ������ ��������
	inline Operation* Left() {
		return left;
	}
	// ����� ������� � ������� ��������
	inline Operation* Right() {
		return right;
	}
	// ���������� ����������� ��������
	double Calculate();
private:
	// ����� �������
	Operation* left;
	// ������ �������
	Operation* right;
	// ��� ��������
	Operations code;
	// ������� �������� (���� �������� ��������, ���� ��������� ����������)
	double value;
};
