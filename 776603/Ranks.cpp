#include <vcl.h>
#include <vector>
#include <stack>
#include <algorithm>
#pragma hdrstop

#include "Ranks.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFRanks *FRanks;
//---------------------------------------------------------------------------
__fastcall TFRanks::TFRanks(TComponent* Owner)
	: TFrame(Owner)
{
}
//---------------------------------------------------------------------------
