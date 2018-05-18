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
	static const int rayInc = 10;
	static const TColor BackColor = RGB(120, 120, 255);
	typedef std::vector< TLayerConfig* > TLayers;
	TLayers layers;
	double rayLength;
	double alpha, beta, l;
	// вспомогательная функция
	void __fastcall Line(int x1, int y1, int x2, int y2, TColor color, TPenStyle style, int width = 1);
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
	inline void __fastcall L(double value) {
		l = value;
	}
	inline double __fastcall L() const {
		return l;
	}
	double __fastcall DrawLayers();
	double __fastcall DrawRay();
};
//---------------------------------------------------------------------------
extern PACKAGE TFView *FView;
//---------------------------------------------------------------------------
#endif
