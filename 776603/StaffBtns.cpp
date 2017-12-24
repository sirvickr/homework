#include <vcl.h>
#include <vector>
#include <stack>
#include <algorithm> // std::max()
#pragma hdrstop

#include "StaffBtns.h"
#include "Main.h"
#include "Staff.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFStaffBtns *FStaffBtns;
//---------------------------------------------------------------------------
__fastcall TFStaffBtns::TFStaffBtns(TComponent* Owner)
	: TFrame(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFStaffBtns::cmdFormsClick(TObject *Sender)
{
	FMain->AddFrame(new TFStaff(NULL));
}
//---------------------------------------------------------------------------

