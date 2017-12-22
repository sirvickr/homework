//---------------------------------------------------------------------------

#ifndef TFMenuH
#define TFMenuH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Menus.hpp>
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
class TFMainMenu : public TForm
{
__published:	// IDE-managed Components
	TMainMenu *MainMenu;
	TMenuItem *MnuClients;
	TMenuItem *MnuDealers;
	TMenuItem *N3;
	TMenuItem *N4;
	TMenuItem *N5;
	TMenuItem *N6;
	TMenuItem *MnuClientsInfo;
	TMenuItem *MnuClientsSeries;
	TMenuItem *MnuClientsStore;
	TStatusBar *SbrMain;
	TProgressBar *PbrMain;
	TMenuItem *MnuClientsSep1;
	TMenuItem *MnuClientsExit;
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall MnuClientsInfoClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall MnuClientsExitClick(TObject *Sender);
	void __fastcall MnuClientsSeriesClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TFMainMenu(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFMainMenu *FMainMenu;
//---------------------------------------------------------------------------
#endif
