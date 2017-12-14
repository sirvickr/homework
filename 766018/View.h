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
	double rayLength;
	double alpha, beta;
public:		// User declarations
	__fastcall TFView(TComponent* Owner);
	void __fastcall Layers(const TLayers&);
	inline void __fastcall Alpha(double value) {
		alpha = value;
	}
	inline double __fastcall Alpha() const {
		return alpha;
	}
	inline void __fastcall Beta(double value) {
		beta = value;
	}
	inline double __fastcall Beta() const {
		return beta;
	}
	double __fastcall DrawLayers();
	double __fastcall DrawRay();
};
//---------------------------------------------------------------------------
extern PACKAGE TFView *FView;
//---------------------------------------------------------------------------
#endif
