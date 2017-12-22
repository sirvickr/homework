//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "TFPintInstr.h"
#include "TFDB.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFPintInstr *FPintInstr;
//---------------------------------------------------------------------------
__fastcall TFPintInstr::TFPintInstr(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFPintInstr::BPrintClick(TObject *Sender)
{
	AnsiString profile_period, profile_trafik, profile_description;
	AnsiString s1, strTimeBegin, strTimeEnd;
	bool is_night;
	if ( (ChkMin->Checked) && (min_id.Length()) )
	{
		TParameter *p1	= NULL;
		TParameter *p2	= NULL;
		p1 = FDB->P_GetCardsWithProfileInfo->Parameters->CreateParameter("@IDPrintmin", ftString, pdInput, min_id.Length(), min_id);
		if ( (ChkMax->Checked) &&  (max_id.Length()) )
			p2 = FDB->P_GetCardsWithProfileInfo->Parameters->CreateParameter("@IDPrintmax", ftString, pdInput, max_id.Length(), max_id);
		FDB->P_GetCardsWithProfileInfo->Open();
		if (RbInstruction->Checked) // печать инструкций
			while (!FDB->P_GetCardsWithProfileInfo->Eof)
			{	// определение карт ночного типа
				usr_id = FDB->P_GetCardsWithProfileInfo->FieldByName("active_user_id")->AsString.TrimRight();
				s1 = FDB->P_GetCardsWithProfileInfo->FieldByName("s1")->AsString.TrimRight();
				profile_period = FDB->P_GetCardsWithProfileInfo->FieldByName("profile_period")->AsString.TrimRight();
				profile_trafik = FDB->P_GetCardsWithProfileInfo->FieldByName("profile_trafik")->AsString.TrimRight();
				is_night = (s1 == "N");
				strTimeBegin	= FDB->P_GetCardsWithProfileInfo->FieldByName("profile_time1")->AsString.TrimRight();
				strTimeEnd		= FDB->P_GetCardsWithProfileInfo->FieldByName("profile_time2")->AsString.TrimRight();
				Print_Instruction( is_night,
					strTimeBegin,
					strTimeEnd,
					FDB->P_GetCardsWithProfileInfo->FieldByName("active_username")->AsString.TrimRight(),
					FDB->P_GetCardsWithProfileInfo->FieldByName("active_password")->AsString.TrimRight(),
					usr_id,
					profile_period,
					profile_trafik );
				FDB->P_GetCardsWithProfileInfo->Next();
			}
		else	// печать конвертов
			while (!FDB->P_GetCardsWithProfileInfo->Eof)
			{
				// определение названия типа карт
				usr_id = FDB->P_GetCardsWithProfileInfo->FieldByName("active_user_id")->AsString.TrimRight();
				profile_description = FDB->P_GetCardsWithProfileInfo->FieldByName("profile_description")->AsString.TrimRight();
				Print_Envelope(profile_description, usr_id);
				FDB->P_GetCardsWithProfileInfo->Next();
			}
		FDB->P_GetCardsWithProfileInfo->First();
		FDB->P_GetCardsWithProfileInfo->Close();
		if (p1) delete p1;
		if (p2) delete p2;
	}
	else ShowMessage("Выберите как минимум начальный помер серии");
}
//---------------------------------------------------------------------------
void TFPintInstr::Print_Instruction( bool fNight,
				const AnsiString& strTime1, const AnsiString& strTime2,
				AnsiString &login, AnsiString &pw, AnsiString &id,
				AnsiString &per, AnsiString &traf )
{
	AnsiString tt;
    tt = strTime1 + ".00-" + strTime2 + ".00";
	Printer()->BeginDoc();
	// печать рамки вокруг левого верха
	Printer()->Canvas->Rectangle(950, 460, 2550, 740);
	// печать левого верха
	Printer()->Canvas->Font->Size = 8;
	Printer()->Canvas->Font->Name =  "Courier New CYR"; //"Times New Roman";  //"";  // "Courier New Cyr";
	Printer()->Canvas->Font->Style = TFontStyles()<< fsItalic << fsBold << fsUnderline;
	Printer()->Canvas->TextOut(1400, 480, "Для данного пакета");
	Printer()->Canvas->Font->Size = 7;
	Printer()->Canvas->TextOut(1050, 560, "Время действия с " + tt + " все дни недели");
	Printer()->Canvas->TextOut(1000, 640, "Срок действия " + per + " с момента подключения");
	// печать строки под рамкой  объема трафика
	Printer()->Canvas->Font->Size = 9;
	Printer()->Canvas->Font->Style = TFontStyles()<< fsItalic << fsBold;
	Printer()->Canvas->TextOut(1400, 780, traf);
	if (fNight)
	{
		// печать низа   для ночных
		Printer()->Canvas->Font->Size = 8;
		Printer()->Canvas->Font->Style = TFontStyles()<< fsItalic << fsBold;
		Printer()->Canvas->TextOut(1250, 960, "ВНИМАНИЕ!");
		Printer()->Canvas->Font->Size = 8;
		Printer()->Canvas->Font->Style = TFontStyles()<< fsItalic;
		Printer()->Canvas->TextOut(1750, 960, "В случае выхода в сеть в другое время тарификация");
		Printer()->Canvas->TextOut(1000, 1030, "осуществляется по часовому тарифу 0.7$ час.");
		Printer()->Canvas->Font->Size = 8;
		Printer()->Canvas->Font->Style = TFontStyles()<< fsItalic << fsBold;
		Printer()->Canvas->TextOut(2800, 1030, "Ваше время закончится быстрее!");
	}
	else
	{
		// печать низа  для обычных
		Printer()->Canvas->Font->Size = 12;
		Printer()->Canvas->Font->Style = TFontStyles()<< fsItalic << fsBold;
		Printer()->Canvas->TextOut(1450, 1000, "Спасибо за Ваш выбор и доверие!");
	}

	// печать заголовков Логин, Пароль, Номер
	Printer()->Canvas->Font->Size = 12;
	Printer()->Canvas->Font->Style = TFontStyles()<< fsItalic << fsBold;
	Printer()->Canvas->TextOut(2700, 380, "Логин");
	Printer()->Canvas->TextOut(2650, 580, "Пароль");
	Printer()->Canvas->TextOut(2700, 780, "Номер");

	// печать рамки вокруг Логин, Пароль, Номер
	//  Printer()->Canvas->Font->Style = TFontStyles()<< fsBold;
	// Printer()->Canvas->Rectangle(2700, 280, 3400, 850);
	// Printer()->Canvas->RoundRect(3200, 360, 3900, 930, 20, 20);

	// печать заполнения Логин, Пароль, Номер
	Printer()->Canvas->Font->Size = 14;
	Printer()->Canvas->Font->Name = "Times New Roman";
	Printer()->Canvas->Font->Style = TFontStyles()<< fsBold;
	Printer()->Canvas->TextOut(3300, 360, login);
	Printer()->Canvas->TextOut(3280, 560, pw);
	Printer()->Canvas->TextOut(3300, 760, id);

	Printer()->EndDoc();
}
//---------------------------------------------------------------------------
void TFPintInstr::Print_Envelope(AnsiString &s, AnsiString &id)
{
	AnsiString s1, s2;
	int pos;

	FDB->Konv_Left1 = FDB->Kon_Left1 * 28;
	FDB->Konv_Top1 = FDB->Kon_Top1 * 28;
	FDB->Konv_Left2 = FDB->Kon_Left2 * 28;
	FDB->Konv_Top2 = FDB->Kon_Top2 * 28;
	FDB->Konv_Left3 = FDB->Kon_Left3 * 28;
	FDB->Konv_Top3 = FDB->Kon_Top3 * 28;
	pos = s.AnsiPos('-');
	s1 = s.SubString(1,pos -2);     //1 строка
	s2 = s.SubString(pos+2, s.Length());   //2 строка
	Printer()->BeginDoc();
	Printer()->Canvas->Font->Name = "Courier New CYR"; //"Arial";
	Printer()->Canvas->Font->Size = 12;
	Printer()->Canvas->Font->Style = TFontStyles()<< fsItalic << fsBold;
	// Printer()->Canvas->TextOut(3030 - ((s1.Length()/2) * 43), 775, s1);  //  Раньше было
	Printer()->Canvas->TextOut(FDB->Konv_Left1 - ((s1.Length()/2) * 43), FDB->Konv_Top1, s1);   // (13 -  s1.Length()) * 43),
	Printer()->Canvas->Font->Size = 16;
	//  Printer()->Canvas->TextOut(3030 - ((s2.Length()/2) * 60), 955, s2);   //  Раньше было
	Printer()->Canvas->TextOut(FDB->Konv_Left2 - ((s2.Length()/2) * 60), FDB->Konv_Top2, s2);   // (10 - s2.Length()) * 40),

	// ПЕЧАТЬ id
	Printer()->Canvas->Font->Style = TFontStyles();
	Printer()->Canvas->Font->Size = 12;
	// Printer()->Canvas->TextOut(3400 + ((13 - id.Length()) * 43),2370, id);     //  Раньше было
	Printer()->Canvas->TextOut(FDB->Konv_Left3 + ((13 - id.Length()) * 43),FDB->Konv_Top3, id);
	Printer()->EndDoc();
}
//---------------------------------------------------------------------------

