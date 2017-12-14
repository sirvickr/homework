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
	// ������ ���� ��������
	const int width = ClientWidth - leftMargin * 2; // 200;
	int x1 = leftMargin, y1 = topMargin, x2 = leftMargin + width, y2 = topMargin;
	for(TLayers::iterator it = layers.begin(); it != layers.end(); it++) {
		TLayerConfig* item = *it;
		TColor oldColor = Canvas->Brush->Color;
		Canvas->Brush->Color = RGB(120, 120, 255);
		int height = 120.0 * item->getH() / TLayerConfig::maxH;
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
	// ������ ���
	TColor oldColor = Canvas->Pen->Color;
	int oldWidth = Canvas->Pen->Width;
	Canvas->Pen->Color = RGB(255, 200, 100);
	Canvas->Pen->Width = 2;

	double x = ClientWidth / 2, y = 0;
	double dx = 0, dy = 0;;
	Canvas->MoveTo(x, 0);
	double angleRad = alpha;
	if(rayLength <= topMargin) { // ��� �������� ������
		// ������ (�������� ����)
		y = rayLength;
		dx = y * tan(angleRad);
		x += dx;
		Canvas->LineTo(x, y);
	} else {  // ��� �������� ���� �� ���� ��� ��� ����� �� ��������
		// ������ (����� ���� ��� �� ����� � ��������)
		y = topMargin;
		dx = y * tan(angleRad);
		x += dx;
		Canvas->LineTo(x, y);

		bool footer = true;
		// ����������� ���� ����� ��������
		for(TLayers::iterator it = layers.begin(); it != layers.end(); it++) {
			TLayerConfig* item = *it;
			double height = item->getHeight();
			angleRad = item->getAngle();
			if(rayLength > item->getBottom()) {
				// ���������� ���� ���, ��������� ����� ���� ����
				///item->current = false;
				dx = height * tan(angleRad);
				dy = height;
				x += dx;
				y += dy;
				Canvas->LineTo(x, y);
			} else {
				// �������� ����, ����������� ����� ���� ����
				///item->current = true;
				dy = 1;
				dx = dy * tan(angleRad);
				for(int i = 0; i < int(height + 0.5); ++i) {
					x += dx;
					y += dy;
					if(y > rayLength)
						break;
					Canvas->LineTo(x, y);
				}
				footer = false;
				break;
			}
		} // for(layer)
		if(footer) { // ����� ��� �������� ������, ��� ����� ��������
			double height = rayLength - y;//80;
			dy = 1;
			 // ���������� ����������� ���������� ����
			double nPrev = layers.back()->getN();
			 // ���������� ����������� � �������
			double nCurr = 1.0;
			dx = dy * tan(beta);
			for(int i = 0; i < height; ++i) {
				x += dx;
				y += dy;
				Canvas->LineTo(x, y);
			}
		}
	}

	Canvas->Pen->Color = oldColor;
	Canvas->Pen->Width = oldWidth;
	return rayLength;
}
//---------------------------------------------------------------------------
void __fastcall TFView::tmrViewTimer(TObject *Sender)
{
	rayLength += 10;
	if(rayLength < Height - 10) {
		Repaint();
	} else {
		tmrView->Enabled = false;
	}
}
//---------------------------------------------------------------------------
