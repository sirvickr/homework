#include <vcl.h>
#include <vector>
#include <stack>
#include <algorithm>
#pragma hdrstop

#include "Staff.h"
#include "Main.h"
#include "Employee.h"
#include "EditEmployee.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFStaff *FStaff;
//---------------------------------------------------------------------------
__fastcall TFStaff::TFStaff(TComponent* Owner)
	: TFrame(Owner), idContract(0), idPost(0), idCouncil(0)
{
	//frmEmployee = new TFEmployee(this);
	//frmEmployee->Parent = pnlDetails;
	//frmEmployee->Align = alClient;
}
//---------------------------------------------------------------------------
void __fastcall TFStaff::txtIDChange(TObject *Sender)
{
	if(txtID->Text.Trim().IsEmpty())
		return;
	// ������� � ������� ����������, ��������� ��� �������� ����������
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

	FMain->adoEmployee->Active = false;
	FMain->adoEmployee->SQL->Clear();
	FMain->adoEmployee->SQL->Add(
	//	"SELECT * FROM [���������] WHERE ������������ = " + frmEmployee->txtID->Text
		"SELECT ���������.������������, ���������.�������, ���������.���,"
			  " ���������.��������, ���������.������������, ���������.���������,"
			  " ���������.������������, ���������.������������, ���������.������,"
			  " [��� ���������].��������, ���������.���������,"
			  " [������� ������].[������������ �����] "
		"FROM [������� ������] INNER JOIN "
			"([��� ���������] INNER JOIN (��������� INNER JOIN ��������� "
			"ON ���������.������������ = ���������.������������) "
			"ON [��� ���������].������������ = ���������.������������) "
			"ON [������� ������].������ = ���������.������ "
		"WHERE ���������.������������ = " + txtID->Text
	);
	FMain->adoEmployee->Active = true;
}
//---------------------------------------------------------------------------
void __fastcall TFStaff::txtAddEmployeeClick(TObject *Sender)
{
	int result = FEditEmployee->ShowModal();

	if(result == mrOk) {
		// ������ �����:
		// �������, ���, ��������, ������������, ���������, ������������, ������������, ������
		String strFields = "�������, ���, ��������, ������������, ���������";
		String strValues = "'" + FEditEmployee->txtSurname->Text + "'";
		strValues += ", '" + FEditEmployee->txtName->Text + "'";
		strValues += ", '" + FEditEmployee->txtPatronimic->Text + "'";
		strValues += ", '" + FEditEmployee->txtBirthDate->Text + "'";
		strValues += ", '" + FEditEmployee->txtHireDate->Text + "'";
		if(!FEditEmployee->cbxContract->KeyValue.IsNull()) {
			strFields += ", ������������";
			strValues += ", " + IntToStr((int)FEditEmployee->cbxContract->KeyValue);
		}
		if(!FEditEmployee->cbxPost->KeyValue.IsNull()) {
			strFields += ", ������������";
			strValues += ", " + IntToStr((int)FEditEmployee->cbxPost->KeyValue);
		}
		if(!FEditEmployee->cbxCouncil->KeyValue.IsNull()) {
			strFields += ", ������";
			strValues += ", " + IntToStr((int)FEditEmployee->cbxCouncil->KeyValue);
		}
		String strSQL = "INSERT INTO ��������� ( " + strFields + " ) ";
		strSQL       += "VALUES (" + strValues + ")";
		/*
		#if 1 // ��� �������� ��������
			"INSERT INTO ��������� ( �������, ���, ��������, ������������, ���������, ������������, ������������, ������ )"
			"VALUES ('test1', 'test2', 'testZ', '21.01.1977', '21.01.1998', 1, 1, 1)";
		#else
			"INSERT INTO ��������� ( �������, ���, ��������, ������������, ���������, ������������, ������������, ������ )"
			"SELECT 'test' AS ���������1, 'test' AS ���������2, 'test' AS ���������3, '21.01.1977' AS ���������4, '21.01.1998' AS ���������5, 1 AS ���������6, 1 AS ���������7, 1 AS ���������8";
		#endif
		*/
		FMain->adoCmd->CommandText = strSQL;
		FMain->adoCmd->Execute();
		tblMain->DataSource->DataSet->Active = false;
		tblMain->DataSource->DataSet->Active = true;
	}
}
//---------------------------------------------------------------------------
