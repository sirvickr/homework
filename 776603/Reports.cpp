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
	// создаём фрейм (подчинённую форму)
	TFStaffReport* frame = new TFStaffReport(NULL);
	// загружаем данные из базы в отчёт
	FMain->adoTable->Active = false;
	FMain->adoTable->TableName = "Кадры";
	frame->qrMain->DataSet = FMain->adoTable;
	FMain->adoTable->Active = true;
	// отображаем созданный фрейм
	FMain->AddFrame(frame);
}
//---------------------------------------------------------------------------
void __fastcall TFReports::cmdDirClick(TObject *Sender)
{
	// создаём фрейм (подчинённую форму)
	TFPayments* frame = new TFPayments(NULL);
	// загружаем данные из базы в отчёт
	FMain->adoTable->Active = false;
	FMain->adoTable->TableName = "Выплаты";
	frame->qrMain->DataSet = FMain->adoTable;
	FMain->adoTable->Active = true;
	// отображаем созданный фрейм
	FMain->AddFrame(frame);
}
//---------------------------------------------------------------------------

