#include <vcl.h>
#include <vector>
#include <stack>
#include <algorithm>
#pragma hdrstop

#include "Staff.h"
#include "Main.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFStaff *FStaff;
//---------------------------------------------------------------------------
__fastcall TFStaff::TFStaff(TComponent* Owner)
	: TFrame(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFStaff::txtIDChange(TObject *Sender)
{
	if(txtID->Text.Trim().IsEmpty())
		return;
	FMain->adoPersRanks->Active = false;
	FMain->adoPersRanks->SQL->Clear();
	FMain->adoPersRanks->SQL->Add(
		"SELECT [���������� �����������].������������, [���������� �����������].�����, "
		"[������ ���].������, [������ ���].��������1� "
		"FROM [������ ���] INNER JOIN [���������� �����������] "
		"ON [������ ���].����� = [���������� �����������].����� "
		"WHERE [���������� �����������].������������ = " + txtID->Text
	);
	FMain->adoPersRanks->Active = true;

	FMain->adoPersPractice->Active = false;
	FMain->adoPersPractice->SQL->Clear();
	FMain->adoPersPractice->SQL->Add(
		"SELECT [����������� ���������].������������, [����������� ���������].�����, "
		"[����������� ���������].�������������� AS [���������], [��� ��������].��������, [��� ��������].�������� "
		"FROM [��� ��������] INNER JOIN [����������� ���������] "
		"ON [��� ��������].����� = [����������� ���������].����� "
		"WHERE [����������� ���������].������������ = " + txtID->Text
	);
	FMain->adoPersPractice->Active = true;

	FMain->adoPersSubj->Active = false;
	FMain->adoPersSubj->SQL->Clear();
	FMain->adoPersSubj->SQL->Add(
		"SELECT [�������� �����].������������, [�������� �����].�����, [�������� �����].��������, "
		"[����� ���������].����������, [����� ���������].�������� "
		"FROM [����� ���������] INNER JOIN [�������� �����] "
		"ON [����� ���������].����� = [�������� �����].����� "
		"WHERE [�������� �����].������������ = " + txtID->Text
	);
	FMain->adoPersSubj->Active = true;
}
//---------------------------------------------------------------------------

