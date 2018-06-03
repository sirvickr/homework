#pragma once

static const UINT maxFract = 8;
static const TCHAR* szRealFmt = _T("%.8f");

template<typename T, size_t N> 
const size_t arrayLength(T (&array)[N])
{
  return N;
}

// Класс, моделирующий дисплей калькулятора
class Display
{
public:
	// Конструктор класса
	Display(HWND hDlg, int nCtlID);
	// Добавление цифры
	void AddDigit(int digit);
	// Добавление десятичной точки
	void AddDecimalDot();
	// Удаление крайнего справа символа
	void Backspace();
	// Округление до maxFract знаков после запятой
	static inline double Round(double d) {
		double p = pow(10.0, double(maxFract));
		return int64_t(d * p + 0.5) / p;
	}
	// Метод доступа к количеству символов
	inline UINT Count() const {
		return count;
	}
	// Метод доступа к текущему значению
	inline double Value() const {
		return value;
	}
	// Модификатор текущего значения
	void Value(double val);
	// Отображение текущего значения на дисплее
	void Show(bool cut = false);
	// Смена знака текущего значения
	void InvertSign();
	// Сброс в начальное состояние
	void Reset();
private:
	// основной текстовый буфер для отображения
	TCHAR buffer[256];
	// локальный буфер для алфавитно-цифровых преобразований
	TCHAR str[32];
	// внутреннее представление текущего значения
	double value;
	// количество отображаемых в данный момент символов
	UINT count;
	// дескриптор главного окна
	HWND hDlg;
	// идентификатор элемента управления для отображения дисплея
	int nCtlID;
};
