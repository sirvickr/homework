#pragma once

static const UINT maxFract = 8;
static const TCHAR* szRealFmt = _T("%.8f");

template<typename T, size_t N> 
const size_t arrayLength(T (&array)[N])
{
  return N;
}

// �����, ������������ ������� ������������
class Display
{
public:
	// ����������� ������
	Display(HWND hDlg, int nCtlID);
	// ���������� �����
	void AddDigit(int digit);
	// ���������� ���������� �����
	void AddDecimalDot();
	// �������� �������� ������ �������
	void Backspace();
	// ���������� �� maxFract ������ ����� �������
	static inline double Round(double d) {
		double p = pow(10.0, double(maxFract));
		return int64_t(d * p + 0.5) / p;
	}
	// ����� ������� � ���������� ��������
	inline UINT Count() const {
		return count;
	}
	// ����� ������� � �������� ��������
	inline double Value() const {
		return value;
	}
	// ����������� �������� ��������
	void Value(double val);
	// ����������� �������� �������� �� �������
	void Show(bool cut = false);
	// ����� ����� �������� ��������
	void InvertSign();
	// ����� � ��������� ���������
	void Reset();
private:
	// �������� ��������� ����� ��� �����������
	TCHAR buffer[256];
	// ��������� ����� ��� ���������-�������� ��������������
	TCHAR str[32];
	// ���������� ������������� �������� ��������
	double value;
	// ���������� ������������ � ������ ������ ��������
	UINT count;
	// ���������� �������� ����
	HWND hDlg;
	// ������������� �������� ���������� ��� ����������� �������
	int nCtlID;
};
