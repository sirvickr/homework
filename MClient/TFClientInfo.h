//---------------------------------------------------------------------------

#ifndef TFClientInfoH
#define TFClientInfoH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <DBCtrls.hpp>
#include <DB.hpp>
//---------------------------------------------------------------------------
class TFClientInfo : public TForm
{
__published:	// IDE-managed Components
	TGroupBox *GBUserID;
	TLabel *Label1;
	TComboBox *CboUserID;
	TEdit *EdName;
	TEdit *EdPwd;
	TLabel *Label2;
	TLabel *Label3;
	TGroupBox *GBUserProperties;
	TEdit *EdDateInit;
	TEdit *EdTarif;
	TEdit *EdTimeNowOnline;
	TEdit *EdCurrentSession;
	TEdit *EdEndByInit;
	TLabel *Label4;
	TLabel *Label5;
	TLabel *Label6;
	TLabel *Label7;
	TLabel *Label8;
	TCheckBox *ChkLocked;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall CboUserIDClick(TObject *Sender);
	void __fastcall CboUserIDKeyPress(TObject *Sender, char &Key);
private:	// User declarations
	AnsiString user_id;
public:		// User declarations
	__fastcall TFClientInfo(TComponent* Owner);

	void RequeryMain(void);
};
//---------------------------------------------------------------------------
extern PACKAGE TFClientInfo *FClientInfo;
//---------------------------------------------------------------------------
#endif
