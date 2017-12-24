#include <vcl.h>
#include <vector>
#include <stack>
#include <algorithm>
#pragma hdrstop

#include "Contract.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFContract *FContract;
//---------------------------------------------------------------------------
__fastcall TFContract::TFContract(TComponent* Owner)
	: TFrame(Owner)
{
}
//---------------------------------------------------------------------------
