//---------------------------------------------------------------------------

#ifndef TFLoginH
#define TFLoginH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TFLogin : public TForm
{
__published:	// IDE-managed Components
        TGroupBox *GBAccessCode;
	TEdit *EdtCode;
	TButton *BNewCode;
	TButton *BContinue;
	TGroupBox *GBDBAccess;
	TEdit *EdServerName;
	TEdit *EdLogin;
	TEdit *EdPwd;
	TLabel *Label1;
	TLabel *Label2;
	TLabel *Label3;
	TButton *BConnect;
	TButton *BExit;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall BContinueClick(TObject *Sender);
	void __fastcall EdtCodeKeyPress(TObject *Sender, char &Key);
	void __fastcall BConnectClick(TObject *Sender);
	void __fastcall BExitClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TFLogin(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFLogin *FLogin;
//---------------------------------------------------------------------------
#endif
