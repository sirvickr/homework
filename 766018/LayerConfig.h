#ifndef LayerConfigH
#define LayerConfigH
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
	double getN() const;
	void setN(double value);
	double getH() const; // в метрах
	void setH(double value);
	inline double getAngle() const { return angle; }
	void setAngle(double value);
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
	inline double Round(double value) const {
		return int(value * 100.0 + 0.5) / 100.0;
	}
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
