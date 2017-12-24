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
}
//---------------------------------------------------------------------------

