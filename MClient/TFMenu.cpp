//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "TFMenu.h"
#include "TFLogin.h"
#include "TFDB.h"
#include "TFClientInfo.h"
#include "TFSeries.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFMainMenu *FMainMenu;
//---------------------------------------------------------------------------
__fastcall TFMainMenu::TFMainMenu(TComponent* Owner)
	: TForm(Owner)
{
	Left = 0;
	Top = 0;
}
//---------------------------------------------------------------------------
void __fastcall TFMainMenu::FormClose(TObject *Sender,
      TCloseAction &Action)
{
	FLogin->BExitClick(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TFMainMenu::MnuClientsInfoClick(TObject *Sender)
{
	FClientInfo->Caption = MnuClientsInfo->Caption;
	FClientInfo->Show();
}
//---------------------------------------------------------------------------
void __fastcall TFMainMenu::FormShow(TObject *Sender)
{
	PbrMain->Parent	= SbrMain;
	PbrMain->Left	= 0;
	PbrMain->Top	= 2;
	PbrMain->Width	= SbrMain->Panels->Items[0]->Width;
	PbrMain->Height	= SbrMain->Height - 2;
}
//---------------------------------------------------------------------------
void __fastcall TFMainMenu::MnuClientsExitClick(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TFMainMenu::MnuClientsSeriesClick(TObject *Sender)
{
	FSeries->Show();
}
//---------------------------------------------------------------------------
