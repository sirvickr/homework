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
	TLabel *Label4;
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall cmdViewClick(TObject *Sender);
	void __fastcall txtAlphaChange(TObject *Sender);
private:	// User declarations
	typedef std::vector< TLayerConfig* > TLayers;
	TLayers layers;
	String exeFullPath, iniFileName;
	bool init;
public:		// User declarations
	__fastcall TFMain(TComponent* Owner);
	void __fastcall RecalcLayers();
};
//---------------------------------------------------------------------------
extern PACKAGE TFMain *FMain;
//---------------------------------------------------------------------------
#endif
