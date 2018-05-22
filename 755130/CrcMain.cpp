#include "stdafx.h"
#pragma hdrstop
//---------------------------------------------------------------------------
#include "CrcMain.h"
#include "CRC8.h"
#include "CRC16.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmCrcMain *frmCrcMain;

using namespace std;

//---------------------------------------------------------------------------
__fastcall TfrmCrcMain::TfrmCrcMain(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmCrcMain::FormCreate(TObject *Sender)
{
	char szExeFullName[MAX_PATH];
	char szBuffer[MAX_PATH];
	GetModuleFileName(NULL, szExeFullName, MAX_PATH);
	exePath = ExtractFileDir(szExeFullName);
	iniFileName = exePath + "\\config.ini";

	GetPrivateProfileString("common", "file", "", szBuffer, MAX_PATH, iniFileName.c_str());
	txtFileName->Text = szBuffer;
#if 1 // генерируем тестовый файл
	ofstream of;
	of.open((exePath + "\\data.bin").c_str());
	of << 1 << 2 << 3 << 4 << 5 << 6 << 7 << 8
		<< 9 << 10 << 11 << 12 << 13 << 14 << 15 << 16
		<< 17 << 18 << 19 << 20;
	of.close();
#endif
}
//---------------------------------------------------------------------------
void __fastcall TfrmCrcMain::FormDestroy(TObject *Sender)
{
	WritePrivateProfileString("common", "file", txtFileName->Text.c_str(), iniFileName.c_str());
}
//---------------------------------------------------------------------------
void __fastcall TfrmCrcMain::cmdOpenFileClick(TObject *Sender)
{
	openDlg->InitialDir = exePath;
	if(!openDlg->Execute()) {
		return;
	}
	txtFileName->Text = openDlg->FileName;
}
//---------------------------------------------------------------------------
void TfrmCrcMain::ShowContent(const string& content, size_t padding)
{
	string::const_iterator it;
	for(it = content.begin(); it != content.end() - padding; ++it) {
		Log->Lines->Add(IntToHex((int)*it, 2));
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmCrcMain::cmdProtectClick(TObject *Sender)
{
	Log->Lines->Add("----------------------------------");
	if(!txtFileName->Text.Length()) {
		return;
	}
	string fileName = txtFileName->Text.c_str();
	string content;
	CrcBase *pCRC = GetCrcCalc(rgCrcWidth->ItemIndex);
	if(!pCRC) {
		return;
	}
	ReadFromFile(content, fileName);
	if(content.empty()) {
		return;
	}
	int crc = pCRC->Calculate(content.data(), content.size());
	Log->Lines->Add("CRC = " + IntToHex(crc, pCRC->GetWidth() >> 2) + " (" + IntToStr(crc) + ")");
	AppendCRC(fileName, crc, pCRC->GetWidth() >> 3);
	delete pCRC;
}
//---------------------------------------------------------------------------
void __fastcall TfrmCrcMain::cmdVerifyClick(TObject *Sender)
{
	Log->Lines->Add("----------------------------------");
	if(!txtFileName->Text.Length()) {
		return;
	}
	string fileName = txtFileName->Text.c_str();
	string content;
	CrcBase *pCRC = GetCrcCalc(rgCrcWidth->ItemIndex);
	if(!pCRC) {
		return;
	}
	int crcBytes = pCRC->GetWidth() >> 3; // сдвиг на 3 вправо = быстрое целочисленное деление на 8
	ReadFromFile(content, fileName);
	// как минимум, файл должен содержать CRC тело большей длины
	if(content.size() < (crcBytes << 1) + 1) { // сдвиг на 2 вправо = быстрое целочисленное деление на 4
		return;
	}
	int crc1 = pCRC->Calculate(content.data(), content.size() - crcBytes);
	int crc2 = 0;
	memcpy(&crc2, content.end() - crcBytes, crcBytes);
	String result;
	if(crc1 == crc2)
		result = "CRC OK";
	else
		result = "CRC FAILURE";
	Log->Lines->Add("CRC1 = " + IntToHex(crc1, crcBytes << 1) + " (" + IntToStr(crc1) + ")");
	Log->Lines->Add("CRC2 = " + IntToHex(crc2, crcBytes << 1) + " (" + IntToStr(crc2) + ")");
	Log->Lines->Add(result);
	delete pCRC;
}
//---------------------------------------------------------------------------
void TfrmCrcMain::ReadFromFile(string& content, const string& fileName) {
	ifstream file;
	file.open(fileName.c_str());
	if (file) {
		string line;
		ostringstream oss;
		oss << file.rdbuf();
		content = oss.str();
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmCrcMain::cmdShowContentClick(TObject *Sender)
{
	if(!txtFileName->Text.Length()) {
		return;
	}
	string fileName = txtFileName->Text.c_str();
	string content;
	ReadFromFile(content, fileName);
	if(content.empty()) {
		return;
	}
	ShowContent(content);
}
//---------------------------------------------------------------------------
void __fastcall TfrmCrcMain::cmdCalcCRCClick(TObject *Sender)
{
	Log->Lines->Add("----------------------------------");
	if(!txtFileName->Text.Length()) {
		return;
	}
	string fileName = txtFileName->Text.c_str();
	string content;
	CrcBase *pCRC = GetCrcCalc(rgCrcWidth->ItemIndex);
	if(!pCRC) {
		return;
	}
	ReadFromFile(content, fileName);
	if(content.empty()) {
		return;
	}
	int crc = pCRC->Calculate(content.data(), content.size());
	Log->Lines->Add("CRC = " + IntToHex(crc, pCRC->GetWidth() >> 2) + " (" + IntToStr(crc) + ")");
	delete pCRC;
}
//---------------------------------------------------------------------------
CrcBase* TfrmCrcMain::GetCrcCalc(int index)
{
	CrcBase* result = NULL;
	switch(index) {
	case 0:
		result = new CrcCalc8;
		break;
	case 1:
		result = new CrcCalc16;
		break;
	}
	return result;
}
//---------------------------------------------------------------------------

