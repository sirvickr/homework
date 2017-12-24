#include <vcl.h>
#include <vector>
#include <stack>
#include <algorithm> // std::max()
#pragma hdrstop

#include "Council.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFCouncil *FCouncil;
//---------------------------------------------------------------------------
__fastcall TFCouncil::TFCouncil(TComponent* Owner)
	: TFrame(Owner)
{
}
//---------------------------------------------------------------------------
