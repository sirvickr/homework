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
const double TLayerConfig::minH = 1.0;//0.01; // 1 см
const double TLayerConfig::maxH = 20.0;//0.20;
//---------------------------------------------------------------------------
TLayerConfig::TLayerConfig(TWinControl* owner, Classes::TNotifyEvent OnChange,
							int index, double h, double n)
: index(index), owner(owner)
, gbxFrame(NULL), txtN(NULL), txtH(NULL)
, top(0), height(0), bottom(0), angle(0)
{
	gbxFrame = new TGroupBox(owner);
	gbxFrame->Parent = owner;
	//gbxFrame->Caption = IntToStr(index);
	gbxFrame->Left = 10;
	gbxFrame->Top = 20 + index * 40;
	gbxFrame->Height = 40;
	gbxFrame->Width = owner->ClientWidth - 20;
	gbxFrame->ShowHint = true;

	int width = 40;//gbxFrame->ClientWidth - 20;
	int top[2] = { 12, 40 };
	int leftBorder[6] = { 10, 30, 80, 110, 160, 190 };

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
	setN(n);
	txtN->OnChange = OnChange;

	lblH = new TLabel(owner);
	lblH->Parent = gbxFrame;
	lblH->AutoSize = true;
	lblH->Left = leftBorder[2];
	lblH->Top = top[0];
	lblH->Caption = "h =";

	txtH = new TEdit(owner);
	txtH->Parent = gbxFrame;
	txtH->Left = leftBorder[3];
	txtH->Top = top[0];
	txtH->Width = width; // gbxFrame->ClientWidth - leftBorder[1] - 20;
	txtH->Hint = "Толщина " + IntToStr(index) + "-го слоя";
	setHsm(h);
	txtH->OnChange = OnChange;

	lblA = new TLabel(owner);
	lblA->Parent = gbxFrame;
	lblA->AutoSize = true;
	lblA->Left = leftBorder[4];
	lblA->Top = top[0];
	lblA->Caption = "угол:";

	txtA = new TEdit(owner);
	txtA->Parent = gbxFrame;
	txtA->Left = leftBorder[5];
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
void TLayerConfig::setN(double value)
{
	if(value < minN)
		value = minN;
	if(value > maxN)
		value = maxN;
	txtN->Text = FloatToStr(Round(value));
}
//---------------------------------------------------------------------------
double TLayerConfig::getHm() const
{
	return txtH->Text.ToDouble() / 100.0; // возвращаем в метрах
}
//---------------------------------------------------------------------------
double TLayerConfig::getHsm() const
{
	return txtH->Text.ToDouble();
}
//---------------------------------------------------------------------------
void TLayerConfig::setHsm(double value)
{
	if(value < minH)
		value = minH;
	if(value > maxH)
		value = maxH;
	txtH->Text = FloatToStr(Round(value));
}
//---------------------------------------------------------------------------
void TLayerConfig::setAngle(double value)
{
	angle = value;
	txtA->Text = FloatToStr(Round(value * 180.0 / M_PI));
}
//---------------------------------------------------------------------------
double Round(double value) {
	return int(value * 100.0 + (value < 0 ? -0.5 : 0.5)) / 100.0;
}
//---------------------------------------------------------------------------

