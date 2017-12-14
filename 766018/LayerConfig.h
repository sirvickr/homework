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
	TLayerConfig(TWinControl* owner, int index, double h = 0.0, double n = 0.0, double angle = 0.0);
	double getN() const;
	double getH() const; // в метрах
	inline int getTop() const { return top; }
	inline int getHeight() const { return height; }
	inline int getBottom() const { return bottom; }
	inline void SetBounds(int top, int height) {
		this->top = top;
		this->height = height;
		this->bottom = top + height - 1;
	}
	inline double getAngle() const { return angle; }
	inline void setAngle(double value) { angle = value; }
private:
	int index;
	// элементы управлени€ настройками (удал€ютс€ владельцем)
	TWinControl* owner;
	TGroupBox* gbxFrame;
	TEdit* txtN;
	TEdit* txtH;
	// границы отображени€ сло€ (в пиксел€х)
	int top;
	int height;
	int bottom;
	double angle;
};
//---------------------------------------------------------------------------
typedef std::list< TLayerConfig* > TLayers;
//---------------------------------------------------------------------------
#endif
