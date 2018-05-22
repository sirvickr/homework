#include "stdafx.h"
#pragma hdrstop
//---------------------------------------------------------------------------
#include "CRC16.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

using namespace std;

DWORD CrcCalc16::crc_tab[256];
bool CrcCalc16::crc_tab_init = false;

CrcCalc16::CrcCalc16(void) : CrcBase(16, 0x0000)
{
	if(!crc_tab_init) {
		InitLookupTable();
	}
}

int CrcCalc16::Calculate(const BYTE* data, size_t size)
{
	if(data == NULL)
		return 0;
	WORD crc = start;
	for (size_t a = 0; a < size; ++a) {
		WORD short_c = 0x00FF & WORD(*data);
		WORD tmp     =  crc ^ short_c;
		crc = (crc >> 8) ^ crc_tab[ tmp & 0xff ];
		++data;
	}
	return crc;
}

void CrcCalc16::InitLookupTable() {
	for(WORD i = 0; i < 256; ++i) {
		WORD crc = 0;
		WORD c = i;
		for(WORD j = 0; j < 8; ++j) {
			if ((crc ^ c) & 1) crc = ( crc >> 1 ) ^ polynomial;
			else               crc =   crc >> 1;
			c = c >> 1;
		}
		crc_tab[i] = crc;
	}
	crc_tab_init = true;
}

