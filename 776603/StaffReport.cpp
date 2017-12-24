#include <vcl.h>
#include <vector>
#include <stack>
#include <algorithm> // std::max()
#pragma hdrstop

#include "StaffReport.h"
#include "Main.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFStaffReport *FStaffReport;
//---------------------------------------------------------------------------
__fastcall TFStaffReport::TFStaffReport(TComponent* Owner)
	: TFrame(Owner)
{
}
//---------------------------------------------------------------------------
