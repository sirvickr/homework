#include <vcl.h>
#include <vector>
#include <stack>
#include <algorithm>
#pragma hdrstop

#include "Payments.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFPayments *FPayments;
//---------------------------------------------------------------------------
__fastcall TFPayments::TFPayments(TComponent* Owner)
	: TFrame(Owner)
{
}
//---------------------------------------------------------------------------
