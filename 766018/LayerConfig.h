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
	// ���������� ����������� ����
	double getN() const;
	void setN(double value);
	// ������ ���� �������� � ��������� ���� � ��
	double getHm() const; // � ������
	double getHsm() const; // � �����������
	void setHsm(double value); // � �����������
	// ���� ���������� ���� ����� ����
	inline double getAngle() const { return angle; }
	void setAngle(double value);
	// �������������� ����������� ����
	inline int getTop() const { return top; }
	inline int getHeight() const { return height; }
	inline int getBottom() const { return bottom; }
	inline void SetBounds(int top, int height) {
		this->top = top;
		this->height = height;
		this->bottom = top + height - 1;
	}
	//bool current;// ��������, ��� ���������
private:
	int index;
	// �������� ���������� ����������� (��������� ����������)
	TWinControl* owner;
	TGroupBox* gbxFrame;
	TEdit* txtN;
	TEdit* txtH;
	TLabel* lblN;
	TLabel* lblH;
	TLabel* lblA;
	TEdit* txtA;
	// ������� ����������� ���� (� ��������)
	int top;
	int height;
	int bottom;
	// ���� ����������� ���� � ����
	double angle;
};
//---------------------------------------------------------------------------
#endif
