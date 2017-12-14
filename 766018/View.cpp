#include <vcl.h>
#include <math.h>
#include <list>
#pragma hdrstop

#include "View.h"
#include "LayerConfig.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFView *FView;
//---------------------------------------------------------------------------
__fastcall TFView::TFView(TComponent* Owner)
	: TForm(Owner), rayX(0.0), rayY(0.0)
{
}
//---------------------------------------------------------------------------
void __fastcall TFView::SetLayers(const TLayers& value)
{
	layers = value;
}
//---------------------------------------------------------------------------
void __fastcall TFView::FormActivate(TObject *Sender)
{
	rayX = ClientWidth / 2;
	rayY = 0;
	Height = DrawLayers() + 100;
	tmrView->Enabled = true;
}
//---------------------------------------------------------------------------
void __fastcall TFView::FormDeactivate(TObject *Sender)
{
	tmrView->Enabled = false;
}
//---------------------------------------------------------------------------
void __fastcall TFView::FormPaint(TObject *Sender)
{
	TColor oldColor = Canvas->Brush->Color;
	#if 1
	DrawLayers();
	DrawRay();
	#else
	Canvas->Brush->Color = RGB(255, 0, 0);
	Canvas->Ellipse(100, 100, 250, 150);
	Canvas->MoveTo(200, 100);
	Canvas->LineTo(200, 200);
	Canvas->LineTo(300, 100);
	Canvas->LineTo(200, 100);
	Canvas->Rectangle(70, 100, 90, 180);
	Canvas->TextOutA(100, 160, "Picture");
	//Graphics::TBitmap * gr = new Graphics::TBitmap;
	//gr->LoadFromFile("C:\\1.bmp");
	//Canvas->Draw(100, 180, gr);
	#endif
	Canvas->Brush->Color = oldColor;
}
//---------------------------------------------------------------------------
double __fastcall TFView::DrawLayers()
{
	// рисуем слои пластины
	Canvas->Brush->Color = RGB(120, 120, 255);
	const int width = ClientWidth - leftMargin * 2; // 200;
	int x1 = leftMargin, y1 = topMargin, x2 = leftMargin + width, y2 = topMargin;
	for(TLayers::iterator it = layers.begin(); it != layers.end(); it++) {
		TLayerConfig* item = *it;
		int height = 200.0 * item->getH() / TLayerConfig::maxH;
		y2 += height;
		Canvas->Rectangle(x1, y1, x2, y2);
		item->SetBounds(y1, height);
		y1 += height;
	}
	return y1;
}
//---------------------------------------------------------------------------
double __fastcall TFView::DrawRay()
{
	// рисуем луч
	TColor oldColor = Canvas->Pen->Color;
	Canvas->Pen->Color = RGB(255, 100, 100);

	int center = ClientWidth / 2;
	Canvas->MoveTo(center, 0);
	if(rayY > topMargin) {
		double y = topMargin;
		Canvas->LineTo(center, y);
		for(TLayers::const_iterator it = layers.begin(); it != layers.end(); it++) {
			const TLayerConfig* item = *it;
			double angle = item->getAngle();
			double height = item->getHeight();
			if(rayY > item->getBottom()) {
				double a = angle * M_PI / 180.0;
				double dx = height * tan(a);
				double dy = height;
				rayX += dx;
				y += dy;
				Canvas->LineTo(rayX, y);
			} else {
				/*
				for(int i = 0; i < int(height + 0.5); ++i) {
					double dx = ;
					double dy = ;
					rayX += dx;
					int y = int( + 0.5);
					if(y > rayY)
						break;
					Canvas->LineTo(rayX, y);
				}
				//int height = 200.0 * item->getH() / TLayerConfig::maxH;
				//y2 += height;
				//Canvas->Rectangle(x1, y1, x2, y2);
				//y1 += height;
				*/
			}
		}
		#if 0
		for(int y = 10; y < rayY; ++y) {
			Canvas->LineTo(rayX, y);
		}
		#endif
	} else {
		Canvas->LineTo(rayX, rayY);
	}

	Canvas->Pen->Color = oldColor;
	return rayY;
}
//---------------------------------------------------------------------------
void __fastcall TFView::tmrViewTimer(TObject *Sender)
{
	rayY += 10;
	if(rayY < Height - 40) {
		Repaint();
	} else {
		tmrView->Enabled = false;
	}
}
//---------------------------------------------------------------------------
