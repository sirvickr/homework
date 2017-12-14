#ifndef MainH
#define MainH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TLayerConfig;
//---------------------------------------------------------------------------
class TFMain : public TForm
{
__published:	// IDE-managed Components
	TPanel *pnlConfig;
	TEdit *txtAlpha;
	TLabel *Label2;
	TButton *cmdView;
	TEdit *txtBeta;
	TLabel *Label1;
	TEdit *txtL;
	TLabel *Label3;
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall cmdViewClick(TObject *Sender);
private:	// User declarations
	typedef std::vector< TLayerConfig* > TLayers;
	TLayers layers;
public:		// User declarations
	__fastcall TFMain(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFMain *FMain;
//---------------------------------------------------------------------------
#endif
