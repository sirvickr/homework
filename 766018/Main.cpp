#include <vcl.h>
#include <math.h>
#include <vector>
#pragma hdrstop

#include "Main.h"
#include "View.h"
#include "LayerConfig.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

using namespace std;

TFMain *FMain;
static const int m = 10;
//double _n[] = { 2.0, 4.5, 1.2, 3.5, 3.1 };
//double _h[] = { 8.0, 12.0, 2.0, 5.0, 4.0 }; // см
//---------------------------------------------------------------------------
__fastcall TFMain::TFMain(TComponent* Owner)
	: TForm(Owner), init(false)
{
	char szExeFileName[MAX_PATH];
	GetModuleFileName(NULL, szExeFileName, MAX_PATH);
	exeFullPath = ExtractFileDir(szExeFileName);
	iniFileName = exeFullPath + "\\config.ini";
	srand(time(NULL));
}
//---------------------------------------------------------------------------
void __fastcall TFMain::FormCreate(TObject *Sender)
{
	char szTemp[MAX_PATH] = "";

	if(GetPrivateProfileString("layout", "left", "20", szTemp, MAX_PATH, iniFileName.c_str()))
		Left = StrToFloat(szTemp);
	if(GetPrivateProfileString("layout", "top", "20", szTemp, MAX_PATH, iniFileName.c_str()))
		Top = StrToFloat(szTemp);

	double alpha = (rand() % 60) - 30;
	if(GetPrivateProfileString("common", "alpha", FloatToStr(alpha).c_str(), szTemp, MAX_PATH, iniFileName.c_str()))
		alpha = StrToFloatDef(szTemp, alpha);
	txtAlpha->Text = FloatToStr(Round(alpha));

	double angle = alpha * M_PI / 180.0; // градусы -> радианы
	double n = 1.0, h;
	String strI;
	for(int i = 0; i < m; ++i) {
		strI = IntToStr(i);

		n = ((rand() % (int(TLayerConfig::maxN * 100 - TLayerConfig::minN * 100))) + TLayerConfig::minN * 100) / 100.0;
		if(GetPrivateProfileString(strI.c_str(), "n", "", szTemp, MAX_PATH, iniFileName.c_str()))
			n = StrToFloatDef(szTemp, n);

		h = ((rand() % (int(TLayerConfig::maxH * 100 - TLayerConfig::minH * 100))) + TLayerConfig::minH * 100) / 100.0;
		if(GetPrivateProfileString(strI.c_str(), "h", "", szTemp, MAX_PATH, iniFileName.c_str()))
			h = StrToFloatDef(szTemp, h);

		layers.push_back(new TLayerConfig(pnlConfig, &txtAlphaChange, i, h, n));

	}

	init = true;

	RecalcLayers();
}
//---------------------------------------------------------------------------
void __fastcall TFMain::FormDestroy(TObject *Sender)
{
	WritePrivateProfileString("common", "alpha", txtAlpha->Text.c_str(), iniFileName.c_str());
	String strI, strVal;
	for(size_t i = 0; i < layers.size(); ++i) {
		TLayerConfig* item = layers[i];
		strI = IntToStr(i);

		strVal = FloatToStr(item->getN());
		WritePrivateProfileString(strI.c_str(), "n", strVal.c_str(), iniFileName.c_str());

		strVal = FloatToStr(item->getHsm());
		WritePrivateProfileString(strI.c_str(), "h", strVal.c_str(), iniFileName.c_str());

		delete item;
	}
	WritePrivateProfileString("layout", "left", IntToStr(Left).c_str(), iniFileName.c_str());
	WritePrivateProfileString("layout", "top", IntToStr(Top).c_str(), iniFileName.c_str());
}
//---------------------------------------------------------------------------
void __fastcall TFMain::RecalcLayers()
{
	if(!init)
		return;
	try {
		double angle = txtAlpha->Text.ToDouble() * M_PI / 180.0;
		double nPrev = 1.0, nCurr = 1.0;
		double L = 0;
		for(TLayers::iterator it = layers.begin(); it != layers.end(); it++) {
			TLayerConfig* item = *it;
			double nCurr = item->getN();
			angle = asin(nPrev * sin(angle) / nCurr);
			item->setAngle(angle);
			double dx = item->getHm() * tan(angle);
			L += dx;

			nPrev = nCurr;
		}
		nCurr = 1.0;
		angle = asin(nPrev * sin(angle) / nCurr);
		txtBeta->Text = FloatToStr(int(angle * 180.0 / M_PI * 100.0 + 0.5) / 100.0);
		txtL->Text = FloatToStr(Round(L * 100));
	} catch(Exception&) {
	}
}
//---------------------------------------------------------------------------
void __fastcall TFMain::cmdViewClick(TObject *Sender)
{
	if(FView) {
		FView->Left = Left + Width - 20;
		FView->Top = 20;
		FView->Alpha(txtAlpha->Text.ToDouble() * M_PI / 180.0);
		FView->Beta(txtBeta->Text.ToDouble() * M_PI / 180.0);
		FView->L(txtL->Text.ToDouble());
		FView->Layers(layers);
		FView->ShowModal();
	}
}
//---------------------------------------------------------------------------
void __fastcall TFMain::txtAlphaChange(TObject *Sender)
{
	RecalcLayers();
}
//---------------------------------------------------------------------------

