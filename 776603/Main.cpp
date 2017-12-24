#include <vcl.h>
#include <vector>
#include <stack>
#include <algorithm> // std::max()
#pragma hdrstop

#include "Main.h"
#include "StaffBtns.h"
#include "Directories.h"
#include "Reports.h"
#include "Staff.h"
#include "StaffReport.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFMain *FMain;
/*
��� ����������� � �� ������������ ������ ADOConnection.
������������ � ��, ����������� � ����� main.accdb ������� ��������.
������������ ��������� ADOConnection, ������������ � ��, ������������ �����������
���������� ��������������� - ������������ ADOTable ��� ADOQuery
� ��������� �������������� ADO ��� ����� ������������ ���������� ��������
����������� - ��������, ��������� ������� DBGrid.
��� ����� ��������� ���������� � ��������� ������������� ADO ������������
����������� ��������� - "�������� ������" (DataSource).
��� ��������� �� �������� � �������������� ������ ������������ �����������
������� DBNavigator.
��� ������ � ������ ������������ ������������ ��������� ����� �� ������
����������� ���������� ����������. �� �� ����� �������� � ������ � ������
"����������".
��� �������� ���������� ��������������� � �� ��������� �� ������� �����.
��������� ����� ������������ �����������:
ADOConnection <- ADOTable1 <- DataSource1 <- DBGRid1
						   <- DataSource2 <- DBGRid1
			  <- ADOTable2 <- DataSource3 <- DBGRid4
			  <- ADOQuery1 <- DataSource4 <- DBGRid5
			  � �.�.
�.�. ��� ��������� ������������� �������� �� ���� ADOConnection,
��������� �������� DataSource ����� ���� �������� �� ���� ��������� �������������
�, �������, ��������� ��������� ����������������� ���������� ���� ������� ������
�� ������ ������� DataSource.
*/
//---------------------------------------------------------------------------
__fastcall TFMain::TFMain(TComponent* Owner)
	: TForm(Owner)//, activeFrame(NULL)
{
}
//---------------------------------------------------------------------------
void __fastcall TFMain::cmdExitClick(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TFMain::cmdStaffClick(TObject *Sender)
{
	// ������ ����� (���������� �����)
	TFStaff* frame = new TFStaff(NULL);
	// ��������� ������ �� ���� � ADOTable
	//adoPost->Active = true;
	adoTable->Active = false;
	adoTable->TableName = "�����";
	adoTable->Active = true;
	// ���������� ����������� �������� ������ � ADOTable
	//dataSource->DataSet = adoTable;
	// ���������� �������� ���������� � ��������� ������
	frame->tblMain->DataSource = dataSource;
	frame->dbnMain->DataSource = dataSource;
	// ���������� ��������� �����
	AddFrame(frame);
}
//---------------------------------------------------------------------------
void __fastcall TFMain::cmdDirClick(TObject *Sender)
{
	AddFrame(new TFDirectories(NULL));
}
//---------------------------------------------------------------------------
void __fastcall TFMain::cmdReportsClick(TObject *Sender)
{
	AddFrame(new TFReports(NULL));
}
//---------------------------------------------------------------------------
void __fastcall TFMain::cmdBackClick(TObject *Sender)
{
	DelFrame();
}
//---------------------------------------------------------------------------
void __fastcall TFMain::AddFrame(TFrame *frame)
{
	frame->Parent = pnlSubForm;
	frame->Align = alClient;
	activeFrames.push(frame);
	cmdBack->Enabled = true;
	lblBack->Enabled = true;
}
//---------------------------------------------------------------------------
void __fastcall TFMain::DelFrame()
{
	if(!activeFrames.empty()) {
		delete activeFrames.top();
		activeFrames.pop();
		if(activeFrames.empty()) {
			cmdBack->Enabled = false;
			lblBack->Enabled = false;
		}
	}
}
//---------------------------------------------------------------------------

