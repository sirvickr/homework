//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "TFLogin.h"
#include "TFMenu.h"
#include "TFDB.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFLogin *FLogin;
//---------------------------------------------------------------------------
__fastcall TFLogin::TFLogin(TComponent* Owner)
        : TForm(Owner)
{      
}
//---------------------------------------------------------------------------
void __fastcall TFLogin::FormShow(TObject *Sender)
{
	Height = 105;
}
//---------------------------------------------------------------------------
void __fastcall TFLogin::BContinueClick(TObject *Sender)
{
	Height = 270;
	BContinue->Default = false;
	BConnect->Default = true;
	EdLogin->SetFocus();
}
//---------------------------------------------------------------------------

void __fastcall TFLogin::EdtCodeKeyPress(TObject *Sender, char &Key)
{
	if ( Key == '\r' )
		if ( EdtCode->Text.Length() )
			if ( EdtCode->Text == "z" )
			{
				BNewCode->Visible = true;
				BContinue->Visible = true;
				BContinue->Default = true;
			}
			else
			{
				EdtCode->Clear();
			}
}
//---------------------------------------------------------------------------
void __fastcall TFLogin::BConnectClick(TObject *Sender)
{
	AnsiString Srv, Usr, Pwd, ConnectionString;
	Srv = FDB->ServerName	= EdServerName->Text;
	Usr = FDB->UserLogin	= EdLogin->Text;
	Pwd = FDB->UserPassword	= EdPwd->Text;
	ConnectionString =
		"Provider=SQLOLEDB.1;Password=" + Pwd			+ ";" +
		"Persist Security Info=True;User ID=" + Usr		+ ";" +
		"Initial Catalog=radusers;Data Source=" + Srv;
	FDB->radusers->Connected = false;
	FDB->radusers->ConnectionString = ConnectionString;
	FDB->radusers->Connected = true;

	FMainMenu->Show();
	Hide();

}
//---------------------------------------------------------------------------
void __fastcall TFLogin::BExitClick(TObject *Sender)
{
	FDB->radusers->Connected = false;
	Close();	
}
//---------------------------------------------------------------------------
