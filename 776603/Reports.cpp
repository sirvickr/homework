#include <vcl.h>
#include <vector>
#include <stack>
#include <algorithm>
#pragma hdrstop

#include "Reports.h"
#include "Main.h"
#include "StaffReport.h"
#include "Payments.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFReports *FReports;
//---------------------------------------------------------------------------
__fastcall TFReports::TFReports(TComponent* Owner)
	: TFrame(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFReports::cmdStaffClick(TObject *Sender)
{
	FMain->adoTable->Active = false;
	FMain->adoTable->TableName = "Кадры";
	FMain->adoTable->Active = true;
	FMain->qrStaff->Preview();
}
//---------------------------------------------------------------------------
void __fastcall TFReports::cmdDirClick(TObject *Sender)
{
	FMain->adoTable->Active = false;
	FMain->adoTable->TableName = "Выплаты";
	FMain->adoTable->Active = true;
	FMain->qrPayments->Preview();
}
//---------------------------------------------------------------------------

