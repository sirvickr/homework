//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "TFSeries.h"
#include "TFDB.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFSeries *FSeries;
//---------------------------------------------------------------------------
__fastcall TFSeries::TFSeries(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFSeries::FormShow(TObject *Sender)
{
	FDB->FSeries_CboProfile->Open();
}
//---------------------------------------------------------------------------
void __fastcall TFSeries::FormClose(TObject *Sender, TCloseAction &Action)
{
	FDB->FSeries_CboProfile->Close();
}
//---------------------------------------------------------------------------
void __fastcall TFSeries::CboProfileCloseUp(TObject *Sender)
{
	profile_cod = FDB->FSeries_CboProfile->FieldByName("profile_cod")->AsString;
}
//---------------------------------------------------------------------------
void __fastcall TFSeries::BGenerateClick(TObject *Sender)
{
	TADOCommand *Cmd	= new TADOCommand (this);
	Cmd->Connection		= FDB->radusers;
	Cmd->CommandType	= cmdText;

	TParameter *Tip		= FDB->Proc_Activate_SerieNew->Parameters->CreateParameter("@ActivProfile", ftString, pdInput, 50, profile_cod);
	TParameter *Quant	= FDB->Proc_Activate_SerieNew->Parameters->CreateParameter("@N_ID", ftInteger, pdInput, 0, atoi(EdQuant->Text.c_str()) );
	TParameter *MinP	= FDB->Proc_Activate_SerieNew->Parameters->CreateParameter("@AMinID", ftString, pdOutput, 7, NULL);
	TParameter *MaxP	= FDB->Proc_Activate_SerieNew->Parameters->CreateParameter("@MaxID", ftString, pdOutput, 7, NULL);

	if ( ( profile_cod.Length() ) && (atoi(EdQuant->Text.c_str()) ))
	{
		FDB->Proc_Activate_SerieNew->ExecProc();
		EdMin->Text = MinP->Value.bstrVal;
		EdMax->Text = MaxP->Value.bstrVal;
		// вставка новой записи в таблицу STORE (склад)
		AnsiString s =
		"INSERT INTO STORE(store_date, store_tip_profile, store_ID1, store_ID2) ";
		"VALUES(GETDATE(), '" + profile_cod + "', '" + EdMin->Text + "', '" + EdMax->Text + "')";
		Cmd->CommandText = s;
		Cmd->Execute();
		Application->MessageBox("Запись о серии добавлена в таблицу актов склада", "Информация", MB_OK);
	}
	FDB->Proc_Activate_SerieNew->Active = false;
	delete MaxP;
	delete MinP;
	delete Quant;
	delete Tip;
	delete Cmd;

}
//---------------------------------------------------------------------------
