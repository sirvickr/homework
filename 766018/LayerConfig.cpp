#include <vcl.h>
#include <math.h>
#include <vector>
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

	int width = 30;//gbxFrame->ClientWidth - 20;
	int top[2] = { 20, 50 };
	int leftBorder[4] = { 10, 30, 80, 110 };

	lblN = new TLabel(owner);
	lblN->Parent = gbxFrame;
	lblN->AutoSize = true;
	lblN->Left = leftBorder[0];
	lblN->Top = top[0];
	lblN->Caption = "n =";

	txtN = new TEdit(owner);
	txtN->Parent = gbxFrame;
	txtN->Left = leftBorder[1];
	txtN->Top = top[0];
	txtN->Width = width; // gbxFrame->ClientWidth - leftBorder[1] - 20;
	txtN->Hint = "Показатель преломления " + IntToStr(index) + "-го слоя";
	txtN->Text = FloatToStr(n);

	lblH = new TLabel(owner);
	lblH->Parent = gbxFrame;
	lblH->AutoSize = true;
	lblH->Left = leftBorder[0];
	lblH->Top = top[1];
	lblH->Caption = "h =";

	txtH = new TEdit(owner);
	txtH->Parent = gbxFrame;
	txtH->Left = leftBorder[1];
	txtH->Top = top[1];
	txtH->Width = width; // gbxFrame->ClientWidth - leftBorder[1] - 20;
	txtH->Hint = "Толщина " + IntToStr(index) + "-го слоя";
	txtH->Text = FloatToStr(h);

	lblA = new TLabel(owner);
	lblA->Parent = gbxFrame;
	lblA->AutoSize = true;
	lblA->Left = leftBorder[2];
	lblA->Top = top[0];
	lblA->Caption = "угол:";

	txtA = new TEdit(owner);
	txtA->Parent = gbxFrame;
	txtA->Left = leftBorder[3];
	txtA->Top = top[0];
	txtA->Width = width;
	txtA->Hint = "Угол";
	txtA->Text = FloatToStr(angle);
	txtA->ReadOnly = true;
	txtA->Color = clBtnFace; 
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

