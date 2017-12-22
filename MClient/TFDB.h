//---------------------------------------------------------------------------

#ifndef TFDBH
#define TFDBH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ADODB.hpp>
#include <DB.hpp>
//---------------------------------------------------------------------------
class TFDB : public TDataModule
{
__published:	// IDE-managed Components
	TADOConnection *radusers;
	TADODataSet *Q_user_id_list;
	TADOStoredProc *Proc_Read_User_Info2;
	TADODataSet *FSeries_CboProfile;
	TADOStoredProc *Proc_Activate_SerieNew;
	TADOStoredProc *P_GetCardsWithProfileInfo;
private:	// User declarations
public:		// User declarations
	__fastcall TFDB(TComponent* Owner);
	AnsiString	ServerName;
	AnsiString	UserLogin;
	AnsiString	UserPassword;
	//
	TIniFile* ini;
	AnsiString exe_ini;  // полный путь файла *.INI
	char znak[2];
	char buf[200];
	AnsiString login;
	AnsiString Prov;    //имя фирмы-провайдера, выводимое в документах Word
	AnsiString dataReport; // дата формирования отчетов
	AnsiString TipDB;      // тип подключаемой базы данных
	void PutIniFile(TForm* F);
	void GetIniFile(TForm* F);
	void SaveTemplate(void);
	double Konv_Left1;
	double Konv_Top1;
	double Konv_Left2;
	double Konv_Top2;
	double Konv_Left3;
	double Konv_Top3;
	double Kon_Left1;
	double Kon_Top1;
	double Kon_Left2;
	double Kon_Top2;
	double Kon_Left3;
	double Kon_Top3;
};
//---------------------------------------------------------------------------
extern PACKAGE TFDB *FDB;
//---------------------------------------------------------------------------
#endif
