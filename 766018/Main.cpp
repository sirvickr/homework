#include <vcl.h>
#include <math.h>
#include <list>
#pragma hdrstop

#include "Main.h"
#include "View.h"
#include "LayerConfig.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

using namespace std;

TFMain *FMain;
static const int m = 5;
double _n[] = { 2.0, 1.2, 3.5, 3.1, 4.5 };
double _h[] = { 8.0, 12.0, 2.0, 5.0, 4.0 }; // см
//---------------------------------------------------------------------------
__fastcall TFMain::TFMain(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFMain::FormCreate(TObject *Sender)
{
	for(int i = 0; i < m; ++i) {
		layers.push_back(new TLayerConfig(pnlConfig, i, _h[i] / 100.0, _n[i], (rand() % 60) - 30));
	}
}
//---------------------------------------------------------------------------
void __fastcall TFMain::FormDestroy(TObject *Sender)
{
	for(TLayers::iterator it = layers.begin(); it != layers.end(); it++) {
		delete *it;
	}
}
//---------------------------------------------------------------------------
void __fastcall TFMain::cmdViewClick(TObject *Sender)
{
	#if 0
	TColor oldColor = Canvas->Brush->Color;
	Canvas->Brush->Color = RGB(255, 0, 0);
	Canvas->Ellipse(100, 100, 150, 150);
	Canvas->MoveTo(200, 100);
	Canvas->LineTo(200, 200);
	Canvas->LineTo(300, 100);
	Canvas->LineTo(200, 100);
	Canvas->Rectangle(70, 100, 90, 180);
	Canvas->Brush->Color = oldColor;
	Canvas->TextOutA(100, 160, "Picture");
	#else
	if(FView) {
		FView->SetLayers(layers);
		FView->ShowModal();
	}
	#endif

}
//---------------------------------------------------------------------------
