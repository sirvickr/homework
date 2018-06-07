#pragma once

// ������ ����
class Coffee
{
public:
	// ����� ���������� (������������ ������)
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
	Coffee(const std::string& kind = "", double price = 0.0, int count = 0);
	inline std::string getKind() const { return kind; }
	inline void setKind(const std::string& value) { kind = value; }
	inline double getPrice() const { return price; }
	inline void setPrice(double value) { price = value; }
	inline int getCount() const { return count; }
	inline void setCount(int value) { count = value; }
	// ������� ��������� ������ (���������� ���������� ����������)
	int takeSome(int count);
	// �������� ��������� ������ (���������� ���������� ����������)
	int addSome(int count);
private:
	// ��� ����
	std::string kind;
	// ���� ������
	double price;
	// ���������� ������
	int count;
};

