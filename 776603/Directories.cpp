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
	// создаём фрейм (подчинённую форму)
	TFPost* frame = new TFPost(NULL);
	// загружаем данные из базы в ADOTable
	FMain->adoTable->Active = false;
	FMain->adoTable->TableName = "Должность";
	FMain->adoTable->Active = true;
	frame->tblMain->DataSource = FMain->dataSource;
	frame->dbnMain->DataSource = FMain->dataSource;
	// отображаем созданный фрейм
	FMain->AddFrame(frame);
}
//---------------------------------------------------------------------------
void __fastcall TFDirectories::cmdContractClick(TObject *Sender)
{
	// создаём фрейм (подчинённую форму)
	TFContract* frame = new TFContract(NULL);
	// загружаем данные из базы в ADOTable
	FMain->adoTable->Active = false;
	FMain->adoTable->TableName = "Тип контракта";
	FMain->adoTable->Active = true;
	// подключаем элементы интерфейса к источнику данных
	frame->tblMain->DataSource = FMain->dataSource;
	frame->dbnMain->DataSource = FMain->dataSource;
	// отображаем созданный фрейм
	FMain->AddFrame(frame);
}
//---------------------------------------------------------------------------
void __fastcall TFDirectories::cmdCouncilClick(TObject *Sender)
{
	// создаём фрейм (подчинённую форму)
	TFCouncil* frame = new TFCouncil(NULL);
	// загружаем данные из базы в ADOTable
	FMain->adoTable->Active = false;
	FMain->adoTable->TableName = "Уровень совета";
	FMain->adoTable->Active = true;
	// подключаем элементы интерфейса к источнику данных
	frame->tblMain->DataSource = FMain->dataSource;
	frame->dbnMain->DataSource = FMain->dataSource;
	// отображаем созданный фрейм
	FMain->AddFrame(frame);
}
//---------------------------------------------------------------------------
void __fastcall TFDirectories::cmdPracticeClick(TObject *Sender)
{
	// создаём фрейм (подчинённую форму)
	TFPractice* frame = new TFPractice(NULL);
	// загружаем данные из базы в ADOTable
	FMain->adoTable->Active = false;
	FMain->adoTable->TableName = "Вид практики";
	FMain->adoTable->Active = true;
	// подключаем элементы интерфейса к источнику данных
	frame->tblMain->DataSource = FMain->dataSource;
	frame->dbnMain->DataSource = FMain->dataSource;
	// отображаем созданный фрейм
	FMain->AddFrame(frame);
}
//---------------------------------------------------------------------------
void __fastcall TFDirectories::cmdRanksClick(TObject *Sender)
{
	// создаём фрейм (подчинённую форму)
	TFRanks* frame = new TFRanks(NULL);
	// загружаем данные из базы в ADOTable
	FMain->adoTable->Active = false;
	FMain->adoTable->TableName = "Разряд тип";
	FMain->adoTable->Active = true;
	// подключаем элементы интерфейса к источнику данных
	frame->tblMain->DataSource = FMain->dataSource;
	frame->dbnMain->DataSource = FMain->dataSource;
	// отображаем созданный фрейм
	FMain->AddFrame(frame);
}
//---------------------------------------------------------------------------
void __fastcall TFDirectories::cmdSubjectsClick(TObject *Sender)
{
	// создаём фрейм (подчинённую форму)
	TFSubj* frame = new TFSubj(NULL);
	// загружаем данные из базы в ADOTable
	FMain->adoTable->Active = false;
	FMain->adoTable->TableName = "Объем дисциплин";
	FMain->adoTable->Active = true;
	// подключаем элементы интерфейса к источнику данных
	frame->tblMain->DataSource = FMain->dataSource;
	frame->dbnMain->DataSource = FMain->dataSource;
	// отображаем созданный фрейм
	FMain->AddFrame(frame);
}
//---------------------------------------------------------------------------

