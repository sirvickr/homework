#ifndef CRC8H
#define CRC8H
//---------------------------------------------------------------------------
#include "CrcBase.h"
//---------------------------------------------------------------------------
class CrcCalc8 : public CrcBase {
public:
	CrcCalc8(void);
	virtual int Calculate(const BYTE* data, size_t size);
private:
	static const BYTE start = 0x00;
	static const BYTE sht75_crc_table[];
};

//---------------------------------------------------------------------------
#endif
