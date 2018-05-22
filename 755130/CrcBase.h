#ifndef CrcBaseH
#define CrcBaseH
//---------------------------------------------------------------------------
class CrcBase
{
public:
	CrcBase(int width, int start = 0x00);
	virtual ~CrcBase();
	virtual int Calculate(const BYTE* data, size_t size) = 0;
	inline const int GetWidth() const {
		return width;
	}
protected:
	int start;
	int width;
};

//---------------------------------------------------------------------------
#endif
