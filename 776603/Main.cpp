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
Для подключения к БД используется объект ADOConnection.
Подключаемся к БД, находящейся в файле main.accdb текущем каталоге.
Единственный компонент ADOConnection, подключенный к БД, используется несколькими
табличными представлениями - компонентами ADOTable или ADOQuery
К табличным представлениям ADO уже могут подключаться визуальные элементы
отображения - например, табличный элемент DBGrid.
Для связи элементов интерфейса и табличных представлений ADO используется
стандартный компонент - "источник данных" (DataSource).
Для навигации по таблицам и редактирования данных используется стандартный
элемент DBNavigator.
Для работы с каждым справочником используется отдельный фрейм со своими
визуальными элементами управления. То же самое какается и работы с формой
"Сотрудники".
Все элементы управления взаимодействием с БД находятся на главной форме.
Примерная схема взаимосвязей компонентов:
ADOConnection <- ADOTable1 <- DataSource1 <- DBGRid1
						   <- DataSource2 <- DBGRid1
			  <- ADOTable2 <- DataSource3 <- DBGRid4
			  <- ADOQuery1 <- DataSource4 <- DBGRid5
			  и т.д.
Т.е. все табличные представления завязаны на один ADOConnection,
несколько объектов DataSource могут быть завязаны на одно табличное представление
и, наконец, несколько элементов пользовательского интерфейса могу черпать данные
из одного объекта DataSource.
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
	// создаём фрейм (подчинённую форму)
	TFStaff* frame = new TFStaff(NULL);
	// загружаем данные из базы в ADOTable
	//adoPost->Active = true;
	adoTable->Active = false;
	adoTable->TableName = "Кадры";
	adoTable->Active = true;
	// подключаем стандартный источник данных к ADOTable
	//dataSource->DataSet = adoTable;
	// подключаем элементы интерфейса к источнику данных
	frame->tblMain->DataSource = dataSource;
	frame->dbnMain->DataSource = dataSource;
	// отображаем созданный фрейм
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

