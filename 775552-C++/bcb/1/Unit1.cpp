//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
bool flag = false;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormCreate(TObject *Sender)
{
	Randomize();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button1Click(TObject *Sender)
{
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si,sizeof(si));
	si.cb=sizeof(si);
	ZeroMemory(&pi,sizeof(pi));
	if(!CreateProcess(NULL, "Parikmaher_prg", NULL, NULL, false, 0, NULL, NULL, &si, &pi)) {
		ShowMessage("Не удалось создать процесс: " + IntToStr(GetLastError()));
		Close();
	}
	flag = true;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Timer1Timer(TObject *Sender)
{
	float vero;
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si,sizeof(si));
	si.cb=sizeof(si);
	if(flag) {
		vero = random(100) / 100.0;
		if(vero > 0.3) {
			if(!CreateProcess(NULL,PChar("Klient"),NULL,NULL,false,0,NULL,NULL,&si,&pi)) {
				ShowMessage("Не удалось создать процесс");
				Close();
			}
		}
	}

}
//---------------------------------------------------------------------------
