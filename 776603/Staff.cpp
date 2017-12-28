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
	// выбрали в таблице сотрудника, обновляем все элементы управления
	FMain->adoPersRanks->Active = false;
	FMain->adoPersRanks->SQL->Clear();
	FMain->adoPersRanks->SQL->Add(
		"SELECT [Подготовка разрядников].КодСотрудник, [Подготовка разрядников].КодРТ, "
		"[Разряд тип].Разряд, [Разряд тип].Надбавка1р "
		"FROM [Разряд тип] INNER JOIN [Подготовка разрядников] "
		"ON [Разряд тип].КодРТ = [Подготовка разрядников].КодРТ "
		"WHERE [Подготовка разрядников].КодСотрудник = " + txtID->Text
	);
	FMain->adoPersRanks->Active = true;

	FMain->adoPersPractice->Active = false;
	FMain->adoPersPractice->SQL->Clear();
	FMain->adoPersPractice->SQL->Add(
		"SELECT [Руководство практикой].КодСотрудник, [Руководство практикой].КодВП, "
		"[Руководство практикой].ЧислоСтудентов AS [Студентов], [Вид практики].ВидПракт, [Вид практики].Надбавка "
		"FROM [Вид практики] INNER JOIN [Руководство практикой] "
		"ON [Вид практики].КодВП = [Руководство практикой].КодВП "
		"WHERE [Руководство практикой].КодСотрудник = " + txtID->Text
	);
	FMain->adoPersPractice->Active = true;

	FMain->adoPersSubj->Active = false;
	FMain->adoPersSubj->SQL->Clear();
	FMain->adoPersSubj->SQL->Add(
		"SELECT [Проверка работ].КодСотрудник, [Проверка работ].КодВЗ, [Проверка работ].Студенты, "
		"[Объем дисциплин].Дисциплина, [Объем дисциплин].Надбавка "
		"FROM [Объем дисциплин] INNER JOIN [Проверка работ] "
		"ON [Объем дисциплин].КодВЗ = [Проверка работ].КодВЗ "
		"WHERE [Проверка работ].КодСотрудник = " + txtID->Text
	);
	FMain->adoPersSubj->Active = true;

	FMain->adoEmployee->Active = false;
	FMain->adoEmployee->SQL->Clear();
	FMain->adoEmployee->SQL->Add(
	//	"SELECT * FROM [Сотрудник] WHERE КодСотрудник = " + frmEmployee->txtID->Text
		"SELECT Сотрудник.КодСотрудник, Сотрудник.Фамилия, Сотрудник.Имя,"
			  " Сотрудник.Отчество, Сотрудник.ДатаРождения, Сотрудник.ДатаНайма,"
			  " Сотрудник.КодКонтракта, Сотрудник.КодДолжности, Сотрудник.КодУМС,"
			  " [Тип контракта].Название, Должность.Должность,"
			  " [Уровень совета].[Методический совет] "
		"FROM [Уровень совета] INNER JOIN "
			"([Тип контракта] INNER JOIN (Должность INNER JOIN Сотрудник "
			"ON Должность.КодДолжность = Сотрудник.КодДолжности) "
			"ON [Тип контракта].КодКонтракта = Сотрудник.КодКонтракта) "
			"ON [Уровень совета].КодУМС = Сотрудник.КодУМС "
		"WHERE Сотрудник.КодСотрудник = " + txtID->Text
	);
	FMain->adoEmployee->Active = true;
}
//---------------------------------------------------------------------------
void __fastcall TFStaff::txtAddEmployeeClick(TObject *Sender)
{
	int result = FEditEmployee->ShowModal();

	if(result == mrOk) {
		// Список полей:
		// Фамилия, Имя, Отчество, ДатаРождения, ДатаНайма, КодКонтракта, КодДолжности, КодУМС
		String strFields = "Фамилия, Имя, Отчество, ДатаРождения, ДатаНайма";
		String strValues = "'" + FEditEmployee->txtSurname->Text + "'";
		strValues += ", '" + FEditEmployee->txtName->Text + "'";
		strValues += ", '" + FEditEmployee->txtPatronimic->Text + "'";
		strValues += ", '" + FEditEmployee->txtBirthDate->Text + "'";
		strValues += ", '" + FEditEmployee->txtHireDate->Text + "'";
		if(!FEditEmployee->cbxContract->KeyValue.IsNull()) {
			strFields += ", КодКонтракта";
			strValues += ", " + IntToStr((int)FEditEmployee->cbxContract->KeyValue);
		}
		if(!FEditEmployee->cbxPost->KeyValue.IsNull()) {
			strFields += ", КодДолжности";
			strValues += ", " + IntToStr((int)FEditEmployee->cbxPost->KeyValue);
		}
		if(!FEditEmployee->cbxCouncil->KeyValue.IsNull()) {
			strFields += ", КодУМС";
			strValues += ", " + IntToStr((int)FEditEmployee->cbxCouncil->KeyValue);
		}
		String strSQL = "INSERT INTO Сотрудник ( " + strFields + " ) ";
		strSQL       += "VALUES (" + strValues + ")";
		/*
		#if 1 // оба варианта работают
			"INSERT INTO Сотрудник ( Фамилия, Имя, Отчество, ДатаРождения, ДатаНайма, КодКонтракта, КодДолжности, КодУМС )"
			"VALUES ('test1', 'test2', 'testZ', '21.01.1977', '21.01.1998', 1, 1, 1)";
		#else
			"INSERT INTO Сотрудник ( Фамилия, Имя, Отчество, ДатаРождения, ДатаНайма, КодКонтракта, КодДолжности, КодУМС )"
			"SELECT 'test' AS Выражение1, 'test' AS Выражение2, 'test' AS Выражение3, '21.01.1977' AS Выражение4, '21.01.1998' AS Выражение5, 1 AS Выражение6, 1 AS Выражение7, 1 AS Выражение8";
		#endif
		*/
		FMain->adoCmd->CommandText = strSQL;
		FMain->adoCmd->Execute();
		tblMain->DataSource->DataSet->Active = false;
		tblMain->DataSource->DataSet->Active = true;
	}
}
//---------------------------------------------------------------------------
