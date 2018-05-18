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
	: TForm(Owner), rayLength(0.0), alpha(0.0), beta(0.0)
{
}
//---------------------------------------------------------------------------
void __fastcall TFView::Layers(const TLayers& value)
{
	layers = value;
}
//---------------------------------------------------------------------------
void __fastcall TFView::FormActivate(TObject *Sender)
{
	rayLength = 0;
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
	DrawLayers();
	DrawRay();
	//Graphics::TBitmap * gr = new Graphics::TBitmap;
	//gr->LoadFromFile("C:\\1.bmp");
	//Canvas->Draw(100, 180, gr);
}
//---------------------------------------------------------------------------
double __fastcall TFView::DrawLayers()
{
	int center = ClientWidth / 2;
	// рисуем слои пластины
	const int width = ClientWidth - leftMargin * 2; // 200;
	int x1 = leftMargin, y1 = topMargin, x2 = leftMargin + width, y2 = topMargin;
	for(TLayers::iterator it = layers.begin(); it != layers.end(); it++) {
		TLayerConfig* item = *it;
		TColor oldColor = Canvas->Brush->Color;
		Canvas->Brush->Color = BackColor;
		int height = 120.0 * item->getHsm() / TLayerConfig::maxH;
		y2 += height;
		Canvas->Rectangle(x1, y1, x2, y2);
		int yCenter = item->getTop() + item->getHeight() / 2 - 5;
		Canvas->TextOut(center + (item->getAngle() > 0 ? -45 : 5),
			yCenter, "h = " + FloatToStr(Round(item->getHsm())));
		Canvas->TextOut(leftMargin + 5, yCenter,
			"n = " + FloatToStr(Round(item->getN())));
		item->SetBounds(y1, height);
		y1 += height;
		Canvas->Brush->Color = oldColor;
	}
	// рисуем ось Y
	Line(center, 5, center, ClientHeight - 5, clAqua, psDashDot);
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
	double x = center - topMargin * tan(alpha), y = 0;
	double dx = 0, dy = 0;;
	Canvas->MoveTo(x, 0);
	double angleRad = alpha;
	if(rayLength <= topMargin) { // луч проходит вакуум
		// вакуум (анимация луча)
		y = rayLength;
		dx = y * tan(angleRad);
		x += dx;
		Canvas->LineTo(x, y);
	} else {  // луч проходит один из слоёв или уже вышел из пластины
		// вакуум (рисуем сразу весь луч до входа в пластину)
		y = topMargin;
		dx = y * tan(angleRad);
		x += dx;
		Canvas->LineTo(x, y);
		int left = x; // запоминаем точку входа в пластину

		bool footer = true;
		// прохождение луча через пластину
		for(TLayers::iterator it = layers.begin(); it != layers.end(); it++) {
			TLayerConfig* item = *it;
			double height = item->getHeight();
			angleRad = item->getAngle();
			int top = item->getTop();
			int bottom = item->getBottom();
			if(rayLength > bottom) {
				// отрисовать весь луч, прошедший через этот слой
				///item->current = false;
				dx = height * tan(angleRad);
				dy = height;
				x += dx;
				y += dy;
				Canvas->LineTo(x, y);
				// отметить параметры луча в слое
				Line(left, top + 10, left, bottom - 1, clAqua, psDot); // вертикальная проекция
				Line(left, bottom, x, bottom, clSilver, psSolid, 3); // коризонтальная проекция
				// надписи
				TColor backColor = Canvas->Brush->Color;
				if(item->getIndex() < layers.size() - 1)
					Canvas->Brush->Color = BackColor;
				else {
					Canvas->Brush->Color = clGray;
					// L
					Line(x, y, x, y + 35, clAqua, psDot);
					Line(center + 1, y + 30, x - 1, y + 30, clAqua, psSolid);
					Canvas->TextOut(center + (x - center) / 2 - 5, y + 35, FloatToStr(fabs(l)));
				}
				int offset = (angleRad > 0) ? -23 : 3;
				TColor clr = Canvas->Font->Color;
				Canvas->Font->Color = clPurple;
				Canvas->TextOut(x + offset, bottom + 3, FloatToStr(fabs(Round(item->getHsm() * tan(angleRad)))));
				Canvas->Font->Color = clr;
				Canvas->Brush->Color = backColor;
				// вернуться на луч
				Canvas->MoveTo(x, y);
				left = int(x + 0.5);
			} else {
				// анимация луча, проходящего через этот слой
				///item->current = true;
				#if 0
				dy = 1;
				dx = dy * tan(angleRad);
				for(int i = 0; i < int(height + 0.5); ++i) {
					x += dx;
					y += dy;
					if(y > rayLength)
						break;
					Canvas->LineTo(x, y);
				}
				#else
				dy = rayLength - top;
				dx = dy * tan(angleRad);
				x += dx;
				y += dy;
				Canvas->LineTo(x, y);
				#endif
				footer = false;
				break;
			}
		} // for(layer)
		if(footer) { // снова луч проходит вакуум, уже после пластины
			 // показатель преломления последнего слоя
			double nPrev = layers.back()->getN();
			 // показатель преломления в вакууме
			double nCurr = 1.0;
			double height = rayLength - y;
			dy = height;
			dx = dy * tan(beta);
			x += dx;
			//y = rayLength;
			Canvas->LineTo(x, rayLength);
		}
	}

	Canvas->Pen->Color = oldColor;
	Canvas->Pen->Width = oldWidth;
	return rayLength;
}
//---------------------------------------------------------------------------
void __fastcall TFView::tmrViewTimer(TObject *Sender)
{
	rayLength += rayInc;
	if(rayLength < Height) {
		Repaint();
	} else {
		tmrView->Enabled = false;
	}
}
//---------------------------------------------------------------------------
void  __fastcall TFView::Line(int x1, int y1, int x2, int y2, TColor color, TPenStyle style, int width)
{
	TColor oldColor = Canvas->Pen->Color;
	int oldWidth = Canvas->Pen->Width;
	TPenStyle oldStype = Canvas->Pen->Style;

	Canvas->Pen->Style = style;
	Canvas->Pen->Color = color;
	Canvas->Pen->Width = width;

	Canvas->MoveTo(x1, y1);
	Canvas->LineTo(x2, y2);

	Canvas->Pen->Color = oldColor;
	Canvas->Pen->Width = oldWidth;
	Canvas->Pen->Style = oldStype;
}
//---------------------------------------------------------------------------
