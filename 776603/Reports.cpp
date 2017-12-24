#include <vcl.h>
#include <vector>
#include <stack>
#include <algorithm> // std::max()
#pragma hdrstop

#include "Reports.h"
#include "Main.h"
#include "StaffReport.h"
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
	// загружаем данные из базы в ADOTable
	/*FMain->adoTable->Active = false;
	FMain->adoTable->TableName = "Должность";
	FMain->adoTable->Active = true;
	frame->tblMain->DataSource = FMain->dataSource;
	frame->dbnMain->DataSource = FMain->dataSource;*/
	// отображаем созданный фрейм
	FMain->AddFrame(frame);
}
//---------------------------------------------------------------------------

