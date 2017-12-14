//---------------------------------------------------------------------------

#ifndef ViewH
#define ViewH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TLayerConfig;
//---------------------------------------------------------------------------
class TFView : public TForm
{
__published:	// IDE-managed Components
	TTimer *tmrView;
	void __fastcall FormActivate(TObject *Sender);
	void __fastcall FormDeactivate(TObject *Sender);
	void __fastcall FormPaint(TObject *Sender);
	void __fastcall tmrViewTimer(TObject *Sender);
private:	// User declarations
	static const int leftMargin = 10;
	static const int topMargin = 50;
	typedef std::vector< TLayerConfig* > TLayers;
	TLayers layers;
	double rayX, rayY;
public:		// User declarations
	__fastcall TFView(TComponent* Owner);
	void __fastcall SetLayers(const TLayers&);
	double __fastcall DrawLayers();
	double __fastcall DrawRay();
};
//---------------------------------------------------------------------------
extern PACKAGE TFView *FView;
//---------------------------------------------------------------------------
#endif
