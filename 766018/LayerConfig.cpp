#include <vcl.h>
#include <math.h>
#include <list>
#pragma hdrstop

#include "LayerConfig.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
const double TLayerConfig::minN = 1.20;
const double TLayerConfig::maxN = 4.70;
const double TLayerConfig::minH = 0.01; // 1см
const double TLayerConfig::maxH = 0.20;
//---------------------------------------------------------------------------
TLayerConfig::TLayerConfig(TWinControl* owner, int index, double h, double n, double a)
: index(index), owner(owner)
, gbxFrame(NULL), txtN(NULL), txtH(NULL), top(0), height(0), bottom(0), angle(a)
//, current(false)
{
	//angle = a * M_PI / 180.0;
	if(n < minN)
		n = minN;
	if(n > maxN)
		n = maxN;
	if(h < minH)
		h = minH;
	if(h > maxH)
		h = maxH;

	gbxFrame = new TGroupBox(owner);
	gbxFrame->Parent = owner;
	gbxFrame->Caption = IntToStr(index);
	gbxFrame->Left = 10;
	gbxFrame->Top = 20 + index * 110;
	gbxFrame->Height = 100;
	gbxFrame->Width = owner->ClientWidth - 20;
	gbxFrame->ShowHint = true;

	int topStart = 20;
	int left = 10;
	int width = gbxFrame->ClientWidth - 20;

	txtN = new TEdit(owner);
	txtN->Parent = gbxFrame;
	txtN->Left = left;
	txtN->Top = topStart;
	txtN->Width = width;
	txtN->Hint = "Показатель преломления " + IntToStr(index) + "-го слоя";
	txtN->Text = FloatToStr(n);

	txtH = new TEdit(owner);
	txtH->Parent = gbxFrame;
	txtH->Left = left;
	txtH->Top = topStart + txtN->Top + 15;
	txtH->Width = width;
	txtH->Hint = "Толщина " + IntToStr(index) + "-го слоя";
	txtH->Text = FloatToStr(h);
}
//---------------------------------------------------------------------------
double TLayerConfig::getN() const
{
	return txtN->Text.ToDouble();
}
//---------------------------------------------------------------------------
double TLayerConfig::getH() const
{
	return txtH->Text.ToDouble();
}
//---------------------------------------------------------------------------

