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
	// ������ ����� (���������� �����)
	TFStaffReport* frame = new TFStaffReport(NULL);
	// ��������� ������ �� ���� � �����
	FMain->adoTable->Active = false;
	FMain->adoTable->TableName = "�����";
	frame->qrMain->DataSet = FMain->adoTable;
	FMain->adoTable->Active = true;
	// ���������� ��������� �����
	FMain->AddFrame(frame);
}
//---------------------------------------------------------------------------
void __fastcall TFReports::cmdDirClick(TObject *Sender)
{
	// ������ ����� (���������� �����)
	TFPayments* frame = new TFPayments(NULL);
	// ��������� ������ �� ���� � �����
	FMain->adoTable->Active = false;
	FMain->adoTable->TableName = "�������";
	frame->qrMain->DataSet = FMain->adoTable;
	FMain->adoTable->Active = true;
	// ���������� ��������� �����
	FMain->AddFrame(frame);
}
//---------------------------------------------------------------------------

