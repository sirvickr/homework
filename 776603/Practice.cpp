#include <vcl.h>
#include <vector>
#include <stack>
#include <algorithm>
#pragma hdrstop

#include "Practice.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFPractice *FPractice;
//---------------------------------------------------------------------------
__fastcall TFPractice::TFPractice(TComponent* Owner)
	: TFrame(Owner)
{
}
//---------------------------------------------------------------------------
