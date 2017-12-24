#include <vcl.h>
#include <vector>
#include <stack>
#include <algorithm> // std::max()
#pragma hdrstop

#include "Directories.h"
#include "Main.h"
#include "Staff.h"
#include "Post.h"
#include "Practice.h"
#include "Contract.h"
#include "Council.h"
#include "Ranks.h"
#include "Subj.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFDirectories *FDirectories;
//---------------------------------------------------------------------------
__fastcall TFDirectories::TFDirectories(TComponent* Owner)
	: TFrame(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFDirectories::cmdPostsClick(TObject *Sender)
{
	// ������ ����� (���������� �����)
	TFPost* frame = new TFPost(NULL);
	// ��������� ������ �� ���� � ADOTable
	FMain->adoTable->Active = false;
	FMain->adoTable->TableName = "���������";
	FMain->adoTable->Active = true;
	frame->tblMain->DataSource = FMain->dataSource;
	frame->dbnMain->DataSource = FMain->dataSource;
	// ���������� ��������� �����
	FMain->AddFrame(frame);
}
//---------------------------------------------------------------------------
void __fastcall TFDirectories::cmdContractClick(TObject *Sender)
{
	// ������ ����� (���������� �����)
	TFContract* frame = new TFContract(NULL);
	// ��������� ������ �� ���� � ADOTable
	FMain->adoTable->Active = false;
	FMain->adoTable->TableName = "��� ���������";
	FMain->adoTable->Active = true;
	// ���������� �������� ���������� � ��������� ������
	frame->tblMain->DataSource = FMain->dataSource;
	frame->dbnMain->DataSource = FMain->dataSource;
	// ���������� ��������� �����
	FMain->AddFrame(frame);
}
//---------------------------------------------------------------------------
void __fastcall TFDirectories::cmdCouncilClick(TObject *Sender)
{
	// ������ ����� (���������� �����)
	TFCouncil* frame = new TFCouncil(NULL);
	// ��������� ������ �� ���� � ADOTable
	FMain->adoTable->Active = false;
	FMain->adoTable->TableName = "������� ������";
	FMain->adoTable->Active = true;
	// ���������� �������� ���������� � ��������� ������
	frame->tblMain->DataSource = FMain->dataSource;
	frame->dbnMain->DataSource = FMain->dataSource;
	// ���������� ��������� �����
	FMain->AddFrame(frame);
}
//---------------------------------------------------------------------------
void __fastcall TFDirectories::cmdPracticeClick(TObject *Sender)
{
	// ������ ����� (���������� �����)
	TFPractice* frame = new TFPractice(NULL);
	// ��������� ������ �� ���� � ADOTable
	FMain->adoTable->Active = false;
	FMain->adoTable->TableName = "��� ��������";
	FMain->adoTable->Active = true;
	// ���������� �������� ���������� � ��������� ������
	frame->tblMain->DataSource = FMain->dataSource;
	frame->dbnMain->DataSource = FMain->dataSource;
	// ���������� ��������� �����
	FMain->AddFrame(frame);
}
//---------------------------------------------------------------------------
void __fastcall TFDirectories::cmdRanksClick(TObject *Sender)
{
	// ������ ����� (���������� �����)
	TFRanks* frame = new TFRanks(NULL);
	// ��������� ������ �� ���� � ADOTable
	FMain->adoTable->Active = false;
	FMain->adoTable->TableName = "������ ���";
	FMain->adoTable->Active = true;
	// ���������� �������� ���������� � ��������� ������
	frame->tblMain->DataSource = FMain->dataSource;
	frame->dbnMain->DataSource = FMain->dataSource;
	// ���������� ��������� �����
	FMain->AddFrame(frame);
}
//---------------------------------------------------------------------------
void __fastcall TFDirectories::cmdSubjectsClick(TObject *Sender)
{
	// ������ ����� (���������� �����)
	TFSubj* frame = new TFSubj(NULL);
	// ��������� ������ �� ���� � ADOTable
	FMain->adoTable->Active = false;
	FMain->adoTable->TableName = "����� ���������";
	FMain->adoTable->Active = true;
	// ���������� �������� ���������� � ��������� ������
	frame->tblMain->DataSource = FMain->dataSource;
	frame->dbnMain->DataSource = FMain->dataSource;
	// ���������� ��������� �����
	FMain->AddFrame(frame);
}
//---------------------------------------------------------------------------

