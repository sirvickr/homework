#ifndef CRC16H
#define CRC16H
//---------------------------------------------------------------------------
#include "CrcBase.h"
//---------------------------------------------------------------------------
class CrcCalc16 : public CrcBase
{
public:
	CrcCalc16(void);
	virtual int Calculate(const BYTE* data, size_t size);
private:
	static const unsigned short polynomial = 0xA001;
	static bool crc_tab_init;
	static DWORD crc_tab[];
private:
	static void InitLookupTable();
};

//---------------------------------------------------------------------------
#endif
