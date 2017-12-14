#include <vcl.h>
#include <math.h>
#include <vector>
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
	const int width = ClientWidth - leftMargin * 2; // 200;
	int x1 = leftMargin, y1 = topMargin, x2 = leftMargin + width, y2 = topMargin;
	for(TLayers::iterator it = layers.begin(); it != layers.end(); it++) {
		TLayerConfig* item = *it;
		TColor oldColor = Canvas->Brush->Color;
		//if(item->current)
		//	Canvas->Brush->Color = RGB(20, 20, 255);
		//else
			Canvas->Brush->Color = RGB(120, 120, 255);
		int height = 200.0 * item->getH() / TLayerConfig::maxH;
		y2 += height;
		Canvas->Rectangle(x1, y1, x2, y2);
		item->SetBounds(y1, height);
		y1 += height;
		Canvas->Brush->Color = oldColor;
	}
	return y1;
}
//---------------------------------------------------------------------------
double __fastcall TFView::DrawRay()
{
	// рисуем луч
	TColor oldColor = Canvas->Pen->Color;
	int oldWidth = Canvas->Pen->Width;
	Canvas->Pen->Color = RGB(255, 200, 100);
	Canvas->Pen->Width = 2;

	int center = ClientWidth / 2;
	double x = center;
	Canvas->MoveTo(x, 0);
	if(rayY > topMargin) {
		double y = topMargin;
		Canvas->LineTo(x, y);
		bool footer = true;
		for(TLayers::iterator it = layers.begin(); it != layers.end(); it++) {
			TLayerConfig* item = *it;
			double angle = item->getAngle();
			double height = item->getHeight();
			double a = angle * M_PI / 180.0;
			if(rayY > item->getBottom()) {
				///item->current = false;
				double dx = height * tan(a);
				double dy = height;
				x += dx;
				y += dy;
				Canvas->LineTo(x, y);
			} else {
				///item->current = true;
				double dy = 1;
				double dx = dy * tan(a);
				for(int i = 0; i < int(height + 0.5); ++i) {
					x += dx;
					y += dy;
					if(y > rayY)
						break;
					Canvas->LineTo(x, y);
				}
				footer = false;
				break;
			}
		} // for(layer)
		if(footer) {
			double angle = -10.0;
			double height = rayY - y;//80;
			double a = angle * M_PI / 180.0;
			double dy = 1;
			double dx = dy * tan(a);
			for(int i = 0; i < height; ++i) {
				x += dx;
				y += dy;
				Canvas->LineTo(x, y);
			}
		}
	} else { // header
		Canvas->LineTo(x, rayY);
	}

	Canvas->Pen->Color = oldColor;
	Canvas->Pen->Width = oldWidth;
	return rayY;
}
//---------------------------------------------------------------------------
void __fastcall TFView::tmrViewTimer(TObject *Sender)
{
	rayY += 10;
	if(rayY < Height - 10) {
		Repaint();
	} else {
		tmrView->Enabled = false;
	}
}
//---------------------------------------------------------------------------
