#ifndef LayerConfigH
#define LayerConfigH
//---------------------------------------------------------------------------
double Round(double value);
//---------------------------------------------------------------------------
class TLayerConfig
{
public:
	static const double minN;
	static const double maxN;
	static const double minH;
	static const double maxH;
	TLayerConfig(TWinControl* owner, Classes::TNotifyEvent OnChange,
				int index, double h = 0.0, double n = 0.0);
	inline int getIndex() const { return index; }
	// показатель преломления слоя
	double getN() const;
	void setN(double value);
	// высота слоя хранится в текстовом поле в см
	double getHm() const; // в метрах
	double getHsm() const; // в сантиметрах
	void setHsm(double value); // в сантиметрах
	// угол прохождния луча через слой
	inline double getAngle() const { return angle; }
	void setAngle(double value);
	// характеристики отображения слоя
	inline int getTop() const { return top; }
	inline int getHeight() const { return height; }
	inline int getBottom() const { return bottom; }
	inline void SetBounds(int top, int height) {
		this->top = top;
		this->height = height;
		this->bottom = top + height - 1;
	}
	//bool current;// временно, для подсветки
private:
	int index;
	// элементы управления настройками (удаляются владельцем)
	TWinControl* owner;
	TGroupBox* gbxFrame;
	TEdit* txtN;
	TEdit* txtH;
	TLabel* lblN;
	TLabel* lblH;
	TLabel* lblA;
	TEdit* txtA;
	// границы отображения слоя (в пикселях)
	int top;
	int height;
	int bottom;
	// угол прохождения луча в слое
	double angle;
};
//---------------------------------------------------------------------------
#endif
