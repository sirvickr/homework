//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "TFClientInfo.h"
#include "TFDB.h"
#include "TFMenu.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFClientInfo *FClientInfo;
//---------------------------------------------------------------------------
__fastcall TFClientInfo::TFClientInfo(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFClientInfo::FormShow(TObject *Sender)
{
	FMainMenu->PbrMain->Min = 0;
	FMainMenu->PbrMain->Max = 7000;
	FMainMenu->PbrMain->Step = 50;
	int i = 0;
	for( FDB->Q_user_id_list->Open(); !FDB->Q_user_id_list->Eof; FDB->Q_user_id_list->Next(), i++)
	{
		CboUserID->Items->Add(FDB->Q_user_id_list->FieldByName("active_user_id")->AsString);
		if (i / 50 == (float)i / 50)
			FMainMenu->PbrMain->Position = i;
	}
	FDB->Q_user_id_list->Close();
	FMainMenu->PbrMain->Position = 0;
}
//---------------------------------------------------------------------------
void __fastcall TFClientInfo::CboUserIDClick(TObject *Sender)
{
	int idx = CboUserID->ItemIndex;
	user_id = CboUserID->Text;
	RequeryMain();
}
//---------------------------------------------------------------------------
void __fastcall TFClientInfo::CboUserIDKeyPress(TObject *Sender, char &Key)
{
	if ( (Key == VK_RETURN) && ( CboUserID->Text.Length() == 7) )
		CboUserIDClick(Sender);
}
//---------------------------------------------------------------------------
void TFClientInfo::RequeryMain()
{
	TParameter *p = FDB->Proc_Read_User_Info2->Parameters->CreateParameter("@selectID", ftString, pdInput, CboUserID->Text.Length(), CboUserID->Text);
	FDB->Proc_Read_User_Info2->Parameters->ParamByName("@selectID")->Value = user_id;
	FDB->Proc_Read_User_Info2->Open();
	EdName->Text			= FDB->Proc_Read_User_Info2->Fields->Fields[1]->AsString;
	EdPwd->Text				= FDB->Proc_Read_User_Info2->Fields->Fields[2]->AsString;
	EdTarif->Text			= FDB->Proc_Read_User_Info2->Fields->Fields[3]->AsString;
	EdDateInit->Text		= FDB->Proc_Read_User_Info2->Fields->Fields[4]->AsString;
	EdTimeNowOnline->Text	= FDB->Proc_Read_User_Info2->Fields->Fields[5]->AsString;
	EdCurrentSession->Text	= FDB->Proc_Read_User_Info2->Fields->Fields[6]->AsString;
	EdEndByInit->Text		= FDB->Proc_Read_User_Info2->Fields->Fields[7]->AsString;
	delete p;   
	if ( FDB->Proc_Read_User_Info2->Fields->Fields[8]->AsString == "2")    //  статус клиента
		ChkLocked->State = cbChecked;
	else
	{
		ChkLocked->State = cbUnchecked;
//		priz=1;
	}
	FDB->Proc_Read_User_Info2->Close();
/*
	if ( QGrid->Active && (( QGrid->Bof) || ( QGrid->Eof))  )
		QGrid->Recordset->Close();
	QGrid->Active = false;
	QGrid->Parameters->ParamByName("ID")->Value = CBox_ID->Text;
	QCount->Parameters->ParamByName("ID")->Value = CBox_ID->Text;
	QCount->Active = false;
	QCount->Active = true;
	ses = QCountrr->Value + 1;
	QGrid->Active = true;     */
}
//---------------------------------------------------------------------------
